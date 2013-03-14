#include "fuzzyrulesprocessorwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "comboboxdelegate.h"
#include "../utils/owlparser.h"
#include "../utils/owlhelperitem.h"
#include "../processorwidgets/doublespinboxdelegate.h"

#include <QDomDocument>

#define MINOPERATOR "min"
#define MAXOPERATOR "max"

using namespace otb;
//using namespace itiviewer;

namespace voreen {

const std::string FuzzyRulesProcessorWidget::loggerCat_("voreen.FuzzyRulesProcessorWidget");

FuzzyRulesProcessorWidget::FuzzyRulesProcessorWidget(QWidget *parent, FuzzyRulesProcessor *fuzzyDefinitionProcessor)
    : m_pCurrentRule(NULL), QProcessorWidget(fuzzyDefinitionProcessor, parent)
{
    tgtAssert(fuzzyDefinitionProcessor, "No FuzzyRulesProcessorWidget processor");

    setWindowTitle(QString::fromStdString(fuzzyDefinitionProcessor->getName()));
    resize(600, 500);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void FuzzyRulesProcessorWidget::initialize(){
    QProcessorWidget::initialize();

    QGroupBox *pGroupBox = new QGroupBox(tr("Fuzzy Rules"),this);

    QLabel *pLabel = new QLabel(tr("Ontology Class"),this);

    m_pOntologyClassComboBox = new QComboBox(this);

    m_pRulesModel = new QStandardItemModel(this);

    m_pMinRadioButton = new QRadioButton(this);
    m_pMaxRadioButton = new QRadioButton(this);

    m_pMaxRadioButton->setText(tr("Max"));
    m_pMinRadioButton->setText(tr("Min"));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_pMinRadioButton);
    buttonGroup->addButton(m_pMaxRadioButton);
    m_pMaxRadioButton->setChecked(true);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(m_pOntologyClassComboBox);
    hboxLayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    m_pAddPushButton = new QPushButton(tr("Add Restriction"),this);
    m_pRemovePushButton = new QPushButton(tr("Remove Restriction"),this);

    QLabel *pLabelFuzzOperator = new QLabel(tr("Fuzzy Operator : "),this);

    QHBoxLayout *hboxlayout1 = new QHBoxLayout;
    hboxlayout1->addWidget(m_pAddPushButton);
    hboxlayout1->addWidget(m_pRemovePushButton);
    hboxlayout1->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout1->addWidget(pLabelFuzzOperator);
    hboxlayout1->addWidget(m_pMinRadioButton);
    hboxlayout1->addWidget(m_pMaxRadioButton);

    m_pRulesTableView = new QTableView(this);
    QStringList headers;
    headers << tr("Name") << tr("Operator") << tr("Value");
    m_pRulesModel->setHorizontalHeaderLabels(headers);

    m_pRulesTableView->setModel(m_pRulesModel);

    m_pExportPushButton = new QPushButton(tr("Update Processor"),this);

    QHBoxLayout *hboxlayout2 = new QHBoxLayout;
    hboxlayout2->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout2->addWidget(m_pExportPushButton);

    QLabel *pLabelRules = new QLabel(tr("Restrictions"),this);

    QVBoxLayout *vboxlayout = new QVBoxLayout;
    vboxlayout->addWidget(pLabel);
    vboxlayout->addLayout(hboxLayout);
    vboxlayout->addLayout(hboxlayout1);
    vboxlayout->addWidget(pLabelRules);
    vboxlayout->addWidget(m_pRulesTableView);
    vboxlayout->addLayout(hboxlayout2);

    pGroupBox->setLayout(vboxlayout);

    QVBoxLayout *vboxlayout1 = new QVBoxLayout;
    vboxlayout1->addWidget(pGroupBox);
    setLayout(vboxlayout1);

    setupOperatorField();
    setupValueField();

    connect(m_pAddPushButton,SIGNAL(clicked()),this,SLOT(onAddButtonClicked()));
    connect(m_pRemovePushButton,SIGNAL(clicked()),this,SLOT(onRemoveButtonClicked()));
    connect(m_pOntologyClassComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(onComboboxCurrentIndexChanged(QString)));
    connect(m_pMinRadioButton,SIGNAL(pressed()),this,SLOT(onRadioButtonMixMaxChanged()));
    connect(m_pMaxRadioButton,SIGNAL(pressed()),this,SLOT(onRadioButtonMixMaxChanged()));
    connect(m_pExportPushButton,SIGNAL(clicked()),this,SLOT(updateOutPortTextData()));
    connect(m_pRulesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));

    hide();
}


void FuzzyRulesProcessorWidget::onAddButtonClicked(){

    if(m_pCurrentRule){
        FuzzyRestriction *pRestriction = new FuzzyRestriction;
        m_pCurrentRule->addRestriction(pRestriction);

        int row = m_pRulesModel->rowCount();

        m_pRulesModel->insertRows(row,1);

        QStandardItem *item0 = new QStandardItem;

        m_pRulesModel->setItem(row,0,item0);
    }
}


void FuzzyRulesProcessorWidget::onRemoveButtonClicked(){
    if(!m_pCurrentRule)
        return;

    // checking for validation of the index is not needed
    QModelIndex index = m_pRulesTableView->selectionModel()->currentIndex();

    int row = index.row();

    FuzzyRestriction *rItem = m_pCurrentRule->restrictionAt(row);
    if(!rItem)
        return;

    m_pCurrentRule->removeRestriction(rItem);
    m_pRulesModel->removeRows(row,1);
}


FuzzyRule* FuzzyRulesProcessorWidget::fuzzyRuleByOntologyClass(const QString &text){
    QList<FuzzyRule*>::const_iterator i;
    for(i = m_fuzzyRuleList.constBegin(); i != m_fuzzyRuleList.constEnd(); i++)
    {
        FuzzyRule *pRule = *i;
        QString className = pRule->className();
        if(!className.compare(text))
            return pRule;
    }

    return 0;
}

void FuzzyRulesProcessorWidget::onComboboxCurrentIndexChanged(const QString &text){
    FuzzyRule *pRule = fuzzyRuleByOntologyClass(text);
    if(pRule)
    {
        m_pCurrentRule = pRule;

        setupWidgetByCurrentRule();
    }
}

QString FuzzyRulesProcessorWidget::getSymbolOperatorFromText(const QString &text){
    QString symbol;

    if(!text.compare("le")){
        symbol = QLatin1String("<=");
    }else if(!text.compare("l")){
        symbol = QLatin1String("<");
    }else if(!text.compare("ge")){
        symbol = QLatin1String(">=");
    }else if(!text.compare("g")){
        symbol = QLatin1String(">");
    }else
        qDebug() << "mouliaka mpouliaka !!";

    return symbol;
}

QString FuzzyRulesProcessorWidget::getTextOperatorFromSymbol(QString &text){
    QString symbol;

    if(!text.compare("<=")){
        symbol = QLatin1String("le");
    }else if(!text.compare("<")){
        symbol = QLatin1String("l");
    }else if(!text.compare(">=")){
        symbol = QLatin1String("ge");
    }else if(!text.compare(">")){
        symbol = QLatin1String("g");
    }else
        qDebug() << "mouliaka mpouliaka !!";

    return symbol;
}


void FuzzyRulesProcessorWidget::setupWidgetByCurrentRule(){
    if(!m_pCurrentRule)
        return;

    m_pRulesModel->removeRows(0,m_pRulesModel->rowCount());

    QString opr = m_pCurrentRule->opr();
    if(opr.compare(QString::fromAscii(MINOPERATOR)))
        m_pMinRadioButton->setChecked(true);
    else
        m_pMaxRadioButton->setChecked(true);

    QList<FuzzyRestriction*> list = m_pCurrentRule->restrictions();
    QList<FuzzyRestriction*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        FuzzyRestriction *pRestriction = *i;

        QStandardItem *item0 = new QStandardItem;
        QStandardItem *item1 = new QStandardItem;
        QStandardItem *item2 = new QStandardItem;

        item0->setData(pRestriction->fuzzyProperty,Qt::DisplayRole);
        item1->setData(pRestriction->opr,Qt::DisplayRole);
        item2->setData(QString::number(pRestriction->val,'f',2),Qt::DisplayRole);

        QList<QStandardItem*> l;
        l << item0 << item1 << item2;

        m_pRulesModel->appendRow(l);
    }
}


void FuzzyRulesProcessorWidget::setupNameField(){
    ComboBoxDelegate *pComboBoxDelegate = new ComboBoxDelegate(m_fuzzyAttributesList,this);
    m_pRulesTableView->setItemDelegateForColumn(0,pComboBoxDelegate);
}


void FuzzyRulesProcessorWidget::setupOperatorField(){
    QStringList list;
    list << ">=" << ">" << "<=" << "<";
    ComboBoxDelegate *pComboBoxDelegate = new ComboBoxDelegate(list,this);
    m_pRulesTableView->setItemDelegateForColumn(1,pComboBoxDelegate);
}

void FuzzyRulesProcessorWidget::setupValueField(){
    DoubleSpinBoxDelegate *doubleSpinBoxDelegate = new DoubleSpinBoxDelegate(-99999999.0,9999999.0,2,this);
    m_pRulesTableView->setItemDelegateForColumn(2,doubleSpinBoxDelegate);
}


//!
void FuzzyRulesProcessorWidget::updateFromProcessor(){
    FuzzyLabelMapUtility::LabelMapType *mapT = getMapFromPort();
    if(!mapT)
        return;

    setupFuzzyAtributes(mapT,QLatin1String("Fuzzy"));

    QStringList list = getOntologyClassesFromPort();

    setupOntologyClassItems(list);

    setupNameField();
}


FuzzyLabelMapUtility::LabelMapType* FuzzyRulesProcessorWidget::getMapFromPort(){
    FuzzyRulesProcessor* fProcessor = dynamic_cast<FuzzyRulesProcessor*>(processor_);

    if(!fProcessor)
        return 0;

    std::vector<Port*> l = fProcessor->getInports();
    if(l.empty()){
        qDebug() << "label map port list is empty..";

    }else{
        //! get the first port
        for(int i = 0; i < l.size(); i++){
            Port *port = l.at(i);
            if(port->isConnected()){
                OTBLabelMapPort *lblMapPort = dynamic_cast<OTBLabelMapPort*>(port);
                if(lblMapPort){
                    FuzzyLabelMapUtility::LabelMapType *mapT = (FuzzyLabelMapUtility::LabelMapType*)lblMapPort->getData();

                    return mapT;
                }
            }
        }
    }
    return 0;
}


void FuzzyRulesProcessorWidget::setupFuzzyAtributes(FuzzyLabelMapUtility::LabelMapType *mapT, const QString &prepkey){

    m_fuzzyAttributesList.clear();

    for(unsigned int i = 1; i < mapT->GetNumberOfLabelObjects(); i++){
        FuzzyLabelMapUtility::LabelObjectType* lblObject = mapT->GetLabelObject(i);

        std::vector<std::string> attrList = lblObject->GetAvailableAttributes();
        if(!attrList.empty()){
            for(int j = 0; j < attrList.size(); j ++){
                QString name = QString::fromStdString(attrList.at(j));

                if(name.contains(prepkey)){
//                    m_pOntologyClassesComboBox->addItem(name);
                    m_fuzzyAttributesList << name;

                }
            }
        }

        break;
    }
}

void FuzzyRulesProcessorWidget::updateOutPortTextData(){
    FuzzyRulesProcessor *fProcessor = dynamic_cast<FuzzyRulesProcessor*>(processor_);
    if(!fProcessor)
        return;

    QString text = constructXmlFile();

    qDebug() << text;

    fProcessor->setTextOutputData(text.toStdString());
}

QString FuzzyRulesProcessorWidget::constructXmlFile(){
    QString text;

    QDomDocument doc = QDomDocument();

    QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(xmlDeclaration);

    QDomElement rootElement = doc.createElement(QLatin1String("fuzzy"));
    doc.appendChild(rootElement);

    QList<FuzzyRule*>::const_iterator i;
    for(i = m_fuzzyRuleList.constBegin(); i != m_fuzzyRuleList.constEnd(); i++){
        FuzzyRule *pRule = *i;

        if(pRule->isEmpty())
            continue;

        QDomElement fuzzyRuleElemet = doc.createElement(QLatin1String("fuzzyRule"));
        fuzzyRuleElemet.setAttribute(QLatin1String("id"),pRule->id());
        fuzzyRuleElemet.setAttribute(QLatin1String("operator"),pRule->opr());
        rootElement.appendChild(fuzzyRuleElemet);

        QDomElement bodyElement = doc.createElement(QLatin1String("body"));
        fuzzyRuleElemet.appendChild(bodyElement);

        QList<FuzzyRestriction*> rlist = pRule->restrictions();
        QList<FuzzyRestriction*>::const_iterator j;
        for(j = rlist.constBegin(); j != rlist.constEnd(); j++){
            FuzzyRestriction *pRestriction = *j;

            QDomElement fuzzyRestrictionElement = doc.createElement("restriction");
            fuzzyRestrictionElement.setAttribute(QLatin1String("property"),pRestriction->fuzzyProperty);
            fuzzyRestrictionElement.setAttribute(QLatin1String("value"),QString::number(pRestriction->val,'f',2));
            fuzzyRestrictionElement.setAttribute(QLatin1String("operator"),getTextOperatorFromSymbol(pRestriction->opr));
            bodyElement.appendChild(fuzzyRestrictionElement);
        }

        QDomElement headElement = doc.createElement(QLatin1String("head"));
        headElement.setAttribute(QLatin1String("class"),pRule->className());
        fuzzyRuleElemet.appendChild(headElement);

    }

    text = doc.toString(4);

    return text;
}


void FuzzyRulesProcessorWidget::setupOntologyClassItems(const QStringList &list){
    m_fuzzyRuleList.clear();
    m_pOntologyClassComboBox->clear();
    QStringList::const_iterator i;
    int counter = 1;
    for(i = list.constBegin(); i!= list.constEnd(); i++){
        QString text = *i;

        m_pOntologyClassComboBox->addItem(text);
        m_pOntologyClassComboBox->setCurrentIndex(m_pOntologyClassComboBox->count()-1);

        FuzzyRule *pRule =new FuzzyRule(this);
        pRule->setClassName(text);
        pRule->setId(counter++);
        if(m_pMaxRadioButton->isChecked())
            pRule->setOpr(QString::fromAscii(MAXOPERATOR));
        else
            pRule->setOpr(QString::fromAscii(MINOPERATOR));

        m_fuzzyRuleList.append(pRule);

        m_pCurrentRule = pRule;
    }
}

void FuzzyRulesProcessorWidget::onRadioButtonMixMaxChanged(){
    if(m_pMaxRadioButton->isChecked()){
        if(m_pCurrentRule)
            m_pCurrentRule->setOpr(QString::fromAscii(MAXOPERATOR));
    }
    else{
        if(m_pCurrentRule)
            m_pCurrentRule->setOpr(QString::fromAscii(MINOPERATOR));
    }
}


void FuzzyRulesProcessorWidget::processOntologyItem(OWLHelperItem *item, QStringList &list){
    QList<OWLHelperItem*> childList = item->owlChildren();
    QList<OWLHelperItem*>::const_iterator i;
    for(i = childList.constBegin(); i != childList.constEnd(); i++){
        OWLHelperItem *pItem = *i;
        list << pItem->label();
        processOntologyItem(pItem,list);
    }
}


void FuzzyRulesProcessorWidget::onModelChanged(QStandardItem *item){
    if(!m_pCurrentRule)
        return;

    int row = item->row();

    FuzzyRestriction *pRestriction = m_pCurrentRule->restrictionAt(row);
    if(!pRestriction)
        return;

    int column = item->column();
    if(column == 0) // this is the property
        pRestriction->fuzzyProperty = item->text();
    else if(column == 1)
        pRestriction->opr = item->text();
    else
        pRestriction->val = item->data(Qt::DisplayRole).toDouble();
}


QStringList FuzzyRulesProcessorWidget::getOntologyClassesFromPort(){
    QStringList list;
    FuzzyRulesProcessor *fProcessor = dynamic_cast<FuzzyRulesProcessor*>(processor_);
    if(!fProcessor)
        return list;

    std::string text = fProcessor->getOntologyData();
    QString text_ = QString::fromStdString(text);

    OWLParser parser;
    parser.parseContent(text_);

    OWLHelperItem *rootItem = parser.rootOWLHelperItem();

    processOntologyItem(rootItem,list);

    return list;
}

FuzzyRulesProcessorWidget::~FuzzyRulesProcessorWidget(){

}

} //namespace voreen
