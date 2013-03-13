#include "fuzzydefinitionprocessorwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "comboboxdelegate.h"
#include "../utils/owlparser.h"
#include "../utils/owlhelperitem.h"

#include <QDomDocument>

#define MINOPERATOR "min"
#define MAXOPERATOR "max"

using namespace otb;
//using namespace itiviewer;

namespace voreen {

const std::string FuzzyDefinitionProcessorWidget::loggerCat_("voreen.FuzzyDefinitionProcessorWidget");

FuzzyDefinitionProcessorWidget::FuzzyDefinitionProcessorWidget(QWidget *parent, FuzzyDefinitionProcessor *fuzzyDefinitionProcessor)
    : m_pCurrentRule(NULL), QProcessorWidget(fuzzyDefinitionProcessor, parent)
{
    tgtAssert(fuzzyDefinitionProcessor, "No FuzzyDefinitionProcessorWidget processor");

    setWindowTitle(QString::fromStdString(fuzzyDefinitionProcessor->getName()));
    resize(600, 500);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void FuzzyDefinitionProcessorWidget::initialize(){
    QProcessorWidget::initialize();

    QGroupBox *pGroupBox = new QGroupBox(tr("Fuzzy Definition"),this);

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
    hboxLayout->addWidget(m_pMinRadioButton);
    hboxLayout->addWidget(m_pMaxRadioButton);
    hboxLayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    m_pAddPushButton = new QPushButton(tr("Add"),this);
    m_pRemovePushButton = new QPushButton(tr("Remove"),this);

    QHBoxLayout *hboxlayout1 = new QHBoxLayout;
    hboxlayout1->addWidget(m_pAddPushButton);
    hboxlayout1->addWidget(m_pRemovePushButton);
    hboxlayout1->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    m_pRulesTableView = new QTableView(this);
    QStringList headers;
    headers << tr("Name") << tr("Operator") << tr("Value");
    m_pRulesModel->setHorizontalHeaderLabels(headers);

    m_pRulesTableView->setModel(m_pRulesModel);

    m_pExportPushButton = new QPushButton(tr("Update Processor"),this);

    QHBoxLayout *hboxlayout2 = new QHBoxLayout;
    hboxlayout2->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout2->addWidget(m_pExportPushButton);

    QVBoxLayout *vboxlayout = new QVBoxLayout;
    vboxlayout->addWidget(pLabel);
    vboxlayout->addLayout(hboxLayout);
    vboxlayout->addLayout(hboxlayout1);
    vboxlayout->addWidget(m_pRulesTableView);
    vboxlayout->addLayout(hboxlayout2);

    pGroupBox->setLayout(vboxlayout);

    QVBoxLayout *vboxlayout1 = new QVBoxLayout;
    vboxlayout1->addWidget(pGroupBox);
    setLayout(vboxlayout1);

    setupOperatorField();

    connect(m_pAddPushButton,SIGNAL(clicked()),this,SLOT(onAddButtonClicked()));
    connect(m_pRemovePushButton,SIGNAL(clicked()),this,SLOT(onRemoveButtonClicked()));
    connect(m_pOntologyClassComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(onComboboxCurrentIndexChanged(QString)));
    connect(m_pMinRadioButton,SIGNAL(pressed()),this,SLOT(onRadioButtonMixMaxChanged()));
    connect(m_pMaxRadioButton,SIGNAL(pressed()),this,SLOT(onRadioButtonMixMaxChanged()));
    connect(m_pExportPushButton,SIGNAL(clicked()),this,SLOT(updateOutPortTextData()));
    connect(m_pRulesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));
}


void FuzzyDefinitionProcessorWidget::onAddButtonClicked(){

    if(m_pCurrentRule){
        FuzzyRestriction *pRestriction = new FuzzyRestriction;
        m_pCurrentRule->addRestriction(pRestriction);

        int row = m_pRulesModel->rowCount();

        m_pRulesModel->insertRows(row,1);

        QStandardItem *item0 = new QStandardItem;

        m_pRulesModel->setItem(row,0,item0);
    }
}


void FuzzyDefinitionProcessorWidget::onRemoveButtonClicked(){
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


FuzzyRule* FuzzyDefinitionProcessorWidget::fuzzyRuleByOntologyClass(const QString &text){
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

void FuzzyDefinitionProcessorWidget::onComboboxCurrentIndexChanged(const QString &text){
    FuzzyRule *pRule = fuzzyRuleByOntologyClass(text);
    if(pRule)
    {
        m_pCurrentRule = pRule;

        setupWidgetByCurrentRule();
    }
}

QString FuzzyDefinitionProcessorWidget::getSymbolOperatorFromText(const QString &text){
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


void FuzzyDefinitionProcessorWidget::setupWidgetByCurrentRule(){
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


void FuzzyDefinitionProcessorWidget::setupNameField(){
    ComboBoxDelegate *pComboBoxDelegate = new ComboBoxDelegate(m_fuzzyAttributesList,this);
    m_pRulesTableView->setItemDelegateForColumn(0,pComboBoxDelegate);
}


void FuzzyDefinitionProcessorWidget::setupOperatorField(){
    QStringList list;
    list << ">=" << ">" << "<=" << "<";
    ComboBoxDelegate *pComboBoxDelegate = new ComboBoxDelegate(list,this);
    m_pRulesTableView->setItemDelegateForColumn(1,pComboBoxDelegate);
}


//!
void FuzzyDefinitionProcessorWidget::updateFromProcessor(){
    FuzzyLabelMapUtility::LabelMapType *mapT = getMapFromPort();
    if(!mapT)
        return;

    setupFuzzyAtributes(mapT,QLatin1String("Fuzzy"));

    QStringList list = getOntologyClassesFromPort();

    setupOntologyClassItems(list);

    setupNameField();
}


FuzzyLabelMapUtility::LabelMapType* FuzzyDefinitionProcessorWidget::getMapFromPort(){
    FuzzyDefinitionProcessor* fProcessor = dynamic_cast<FuzzyDefinitionProcessor*>(processor_);

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


void FuzzyDefinitionProcessorWidget::setupFuzzyAtributes(FuzzyLabelMapUtility::LabelMapType *mapT, const QString &prepkey){

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

void FuzzyDefinitionProcessorWidget::updateOutPortTextData(){
    FuzzyDefinitionProcessor *fProcessor = dynamic_cast<FuzzyDefinitionProcessor*>(processor_);
    if(!fProcessor)
        return;

    QString text = constructXmlFile();

    qDebug() << text;

    fProcessor->setTextOutputData(text.toStdString());
}

QString FuzzyDefinitionProcessorWidget::constructXmlFile() const{
    QString text;

    QDomDocument doc = QDomDocument();

    QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(xmlDeclaration);

    QDomElement rootElement = doc.createElement(QLatin1String("fuzzy"));

    QList<FuzzyRule*>::const_iterator i;
    for(i = m_fuzzyRuleList.constBegin(); i != m_fuzzyRuleList.constEnd(); i++){
        FuzzyRule *pRule = *i;
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
            fuzzyRestrictionElement.setAttribute(QLatin1String("operator"),pRestriction->opr);
            bodyElement.appendChild(fuzzyRestrictionElement);
        }

        QDomElement headElement = doc.createElement(QLatin1String("head"));
        headElement.setAttribute(QLatin1String("class"),pRule->className());
        fuzzyRuleElemet.appendChild(headElement);

    }

    text = doc.toString(4);

    return text;
}


void FuzzyDefinitionProcessorWidget::setupOntologyClassItems(const QStringList &list){
    m_fuzzyRuleList.clear();
    m_pOntologyClassComboBox->clear();
    QStringList::const_iterator i;
    int counter = 1;
    for(i = list.constBegin(); i!= list.constEnd(); i++){
        QString text = *i;

        m_pOntologyClassComboBox->addItem(text);

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

void FuzzyDefinitionProcessorWidget::onRadioButtonMixMaxChanged(){
    if(m_pMaxRadioButton->isChecked()){
        if(m_pCurrentRule)
            m_pCurrentRule->setOpr(QString::fromAscii(MAXOPERATOR));
    }
    else{
        if(m_pCurrentRule)
            m_pCurrentRule->setOpr(QString::fromAscii(MINOPERATOR));
    }
}


void FuzzyDefinitionProcessorWidget::processOntologyItem(OWLHelperItem *item, QStringList &list){
    QList<OWLHelperItem*> childList = item->owlChildren();
    if(!childList.count())
        list << item->label();
    else{
        QList<OWLHelperItem*>::const_iterator i;
        for(i = childList.constBegin(); i != childList.constEnd(); i++){
            OWLHelperItem *pItem = *i;
            processOntologyItem(pItem,list);
        }
    }
}


void FuzzyDefinitionProcessorWidget::onModelChanged(QStandardItem *item){
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


QStringList FuzzyDefinitionProcessorWidget::getOntologyClassesFromPort(){
    QStringList list;
    FuzzyDefinitionProcessor *fProcessor = dynamic_cast<FuzzyDefinitionProcessor*>(processor_);
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

FuzzyDefinitionProcessorWidget::~FuzzyDefinitionProcessorWidget(){
//    ItiOtbImageManager::deleteInstance();
}

} //namespace voreen
