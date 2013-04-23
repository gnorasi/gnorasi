#include "fuzzyprocessorwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "doublespinboxdelegate.h"
#include "comboboxdelegate.h"
#include "lineeditfornumbersdelegate.h"

#include "../fuzzy/fuzzyontologymanager.h"
#include "../fuzzy/fuzzyontologyclass.h"
#include "../fuzzy/fuzzyattribute.h"
#include "../fuzzy/fuzzyfunctionfactory.h"
#include "../fuzzy/fuzzyfunction.h"

#include "../utils/owlparser.h"
#include "../utils/owlhelperitem.h"

using namespace otb;
//using namespace itiviewer;

namespace voreen {


QStringList FuzzyProcessorWidget::parameterCharacters = QStringList() << QLatin1String("a") << QLatin1String("b") << QLatin1String("c") << QLatin1String("d") << QLatin1String("e");

const std::string FuzzyProcessorWidget::loggerCat_("voreen.FuzzyProcessorWidget");

FuzzyProcessorWidget::FuzzyProcessorWidget(QWidget *parent, FuzzyProcessor *fuzzyProcessor)
    : QProcessorWidget(fuzzyProcessor, parent)
{
    tgtAssert(fuzzyProcessor, "No FuzzyProcessorWidget processor");

    setWindowTitle(QString::fromStdString(fuzzyProcessor->getName()));
    resize(600, 500);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void FuzzyProcessorWidget::initialize(){
    QProcessorWidget::initialize();

    QLabel *pLabelAvailableAttributesTitle = new QLabel(tr("Available Attributes"),this);
    QLabel *pLabelFuzzyAttributesTitle = new QLabel(tr("Fuzzy Attributes"),this);
    QLabel *pLabelMinMaxTitle = new QLabel(tr("Min max : "),this);

    m_pPushButtonAdd = new QPushButton(this);
    m_pPushButtonRemove = new QPushButton(this);
    m_pPushButtonCalculate = new QPushButton(this);

    m_pPushButtonAdd->setText(tr("Add"));
    m_pPushButtonRemove->setText(tr("Remove"));
    m_pPushButtonCalculate->setText(tr("Calculate"));

    m_pAvailableAttributesModel = new QStandardItemModel(this);
    QStringList headers;
    headers << tr("Attribute Name");
    m_pAvailableAttributesModel->setHorizontalHeaderLabels(headers);

    m_pFuzzyAttributesModel = new QStandardItemModel(this);
    QStringList headers1;
    headers1 << tr("Attribute Name") << tr("Function name");
    int numberOfParameters = FuzzyFunction::MAXPARAMETERSCOUNT;
    for(int i = 0 ; i < numberOfParameters; i++){
        headers1 << parameterCharacters.at(i);
    }
    headers1 << tr("Operator") << tr("Value");
    m_pFuzzyAttributesModel->setHorizontalHeaderLabels(headers1);

    m_pAvailableAttributesListView= new QListView(this);

    m_pFuzzyAttributesTableView = new QTableView(this);
    m_pAvailableAttributesListView->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    m_pFuzzyAttributesTableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pAvailableAttributesListView->setAlternatingRowColors(true);
    m_pFuzzyAttributesTableView->setAlternatingRowColors(true);
    m_pAvailableAttributesListView->setModel(m_pAvailableAttributesModel);
    m_pFuzzyAttributesTableView->setModel(m_pFuzzyAttributesModel);


    QVBoxLayout *vboxLayout1 = new QVBoxLayout;
    vboxLayout1->addWidget(pLabelAvailableAttributesTitle);
    vboxLayout1->addWidget(m_pAvailableAttributesListView);


    QVBoxLayout *vboxLayout2 = new QVBoxLayout;
    vboxLayout2->addSpacerItem(new QSpacerItem(100,20,QSizePolicy::Fixed,QSizePolicy::Expanding));
    vboxLayout2->addWidget(m_pPushButtonAdd);
    vboxLayout2->addWidget(m_pPushButtonRemove);
    vboxLayout2->addSpacerItem(new QSpacerItem(100,20,QSizePolicy::Fixed,QSizePolicy::Expanding));


    QLabel *pLabelOntologyClassComboboxTitle = new QLabel(tr("Ontology Classes"));
    m_pOntologyClassComboBox = new QComboBox(this);
    m_pMinRadioButton = new QRadioButton(this);
    m_pMaxRadioButton = new QRadioButton(this);


    QButtonGroup *pGroup = new QButtonGroup(this);
    pGroup->addButton(m_pMinRadioButton);
    pGroup->addButton(m_pMaxRadioButton);


    QHBoxLayout *hboxLayout1 = new QHBoxLayout();
    hboxLayout1->addWidget(m_pOntologyClassComboBox);
    hboxLayout1->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxLayout1->addWidget(pLabelMinMaxTitle);
    hboxLayout1->addWidget(m_pMinRadioButton);
    hboxLayout1->addWidget(m_pMaxRadioButton);


    QHBoxLayout *hboxLayout2 = new QHBoxLayout();
    hboxLayout2->addWidget(pLabelOntologyClassComboboxTitle);
    hboxLayout2->addSpacerItem(new QSpacerItem(100,20,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxLayout2->addWidget(m_pPushButtonCalculate);

    QVBoxLayout *vboxLayout3 = new QVBoxLayout;
    vboxLayout3->addLayout(hboxLayout2);
    vboxLayout3->addLayout(hboxLayout1);
    vboxLayout3->addWidget(pLabelFuzzyAttributesTitle);
    vboxLayout3->addWidget(m_pFuzzyAttributesTableView);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addLayout(vboxLayout1);
    hLayout->addLayout(vboxLayout2);
    hLayout->addLayout(vboxLayout3);

    setLayout(hLayout);


    m_pFuzzyLabelMapUtility = new FuzzyLabelMapUtility(this);


    setupItemDelegates();


    connect(m_pPushButtonAdd,SIGNAL(clicked()),this,SLOT(addSelection()));
    connect(m_pPushButtonCalculate,SIGNAL(clicked()),this,SLOT(calculate()));
    connect(m_pPushButtonRemove,SIGNAL(clicked()),this,SLOT(removeSelection()));

    m_pFuzzuFunctionFactory = new FuzzyFunctionFactory(this);

    connect(m_pOntologyClassComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(onComboboxCurrentIndexChanged(QString)));
    connect(m_pMinRadioButton,SIGNAL(pressed()),this,SLOT(onRadioButtonMixMaxChanged()));
    connect(m_pMaxRadioButton,SIGNAL(pressed()),this,SLOT(onRadioButtonMixMaxChanged()));

    hide();
}

void FuzzyProcessorWidget::setupItemDelegates(){
    DoubleSpinBoxDelegate *spinBoxDelegate = new DoubleSpinBoxDelegate(0.0, 1.0, 2, this);
    m_pFuzzyAttributesTableView->setItemDelegateForColumn(m_pFuzzyAttributesModel->columnCount()-1,spinBoxDelegate);

    for(int i = 2;  i < FuzzyFunction::MAXPARAMETERSCOUNT; i++)
    {
        LineEditForNumbersDelegate *pDelegate = new LineEditForNumbersDelegate(2,this);
        m_pFuzzyAttributesTableView->setItemDelegateForColumn(i,pDelegate);
    }

    QStringList functionsList = FuzzyFunction::availableFunctions;
    ComboBoxDelegate *pComboBoxDelegate = new ComboBoxDelegate(functionsList,this);
    m_pFuzzyAttributesTableView->setItemDelegateForColumn(1,pComboBoxDelegate);

    QStringList operatorList = FuzzyAttribute::fuzzyOperatorNames;
    ComboBoxDelegate *pComboBoxDelegate1 = new ComboBoxDelegate(operatorList,this);
    m_pFuzzyAttributesTableView->setItemDelegateForColumn(m_pFuzzyAttributesModel->columnCount()-2,pComboBoxDelegate1);

}

//!
void FuzzyProcessorWidget::updateFromProcessor(){
    FuzzyLabelMapUtility::LabelMapType *mapT = getMapFromPort();
    if(mapT){
        // parse the fetched data by the port
        m_pFuzzyLabelMapUtility->parse(mapT);

        // setup the data by the parsed map
        QStringList namesList = m_pFuzzyLabelMapUtility->getAttributeListNames();

        setupAvailableAttributesListViewByList(namesList);
    }

    // now seutp the ontology classes
    QStringList list = getOntologyClassesFromPort();
    setupOntologyClassItems(list);
}


FuzzyLabelMapUtility::LabelMapType* FuzzyProcessorWidget::getMapFromPort(){
    FuzzyProcessor* fProcessor = dynamic_cast<FuzzyProcessor*>(processor_);

    if(!fProcessor)
        return 0;

    std::vector<Port*> l = fProcessor->getInports();
    if(l.empty()){
        qDebug() << "label map port list is empty..";

    }else{
        //! get the first port
        Port *port = l.at(0);
        if(port->isConnected()){
            OTBLabelMapPort *lblMapPort = dynamic_cast<OTBLabelMapPort*>(port);
            if(lblMapPort){
                FuzzyLabelMapUtility::LabelMapType *mapT = (FuzzyLabelMapUtility::LabelMapType*)lblMapPort->getData();

                return mapT;
            }
        }
    }
    return 0;
}

void FuzzyProcessorWidget::setupAvailableAttributesListViewByList(const QStringList &list){
    m_pAvailableAttributesModel->removeRows(0,m_pAvailableAttributesModel->rowCount());

    QStringList::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        QString name = *i;

        QStandardItem *pItem = new QStandardItem();
        pItem->setData(name,Qt::DisplayRole);

        m_pAvailableAttributesModel->appendRow(pItem);
    }
}

void FuzzyProcessorWidget::addSelection(){
    QModelIndex index = m_pAvailableAttributesListView->currentIndex();

    if(!index.isValid())
        return;

    QString name = m_pAvailableAttributesModel->data(index).toString();

    QStandardItem *pItem = new QStandardItem();
    pItem->setText(name);

    m_pFuzzyAttributesModel->appendRow(pItem);

    m_pFuzzyAttributesTableView->resizeColumnToContents(0);
}

void FuzzyProcessorWidget::removeSelection(){
    QModelIndex index = m_pFuzzyAttributesTableView->currentIndex();

    if(!index.isValid())
        return;

    m_pFuzzyAttributesModel->removeRow(index.row());
}

void FuzzyProcessorWidget::calculate(){

    FuzzyLabelMapUtility::LabelMapType *lblMap = getMapFromPort();

    for(int i = 0; i < m_pFuzzyAttributesModel->rowCount(); i++){
        QStandardItem *pItem = m_pFuzzyAttributesModel->item(i,0);
        QString atName = pItem->data(Qt::DisplayRole).toString();

        QStandardItem *pItemA =m_pFuzzyAttributesModel->item(i,1);
        if(!pItemA)
            continue;

        m_pFuzzyLabelMapUtility->updateMinValue(pItemA->data(Qt::DisplayRole).toDouble());

        QStandardItem *pItemB = m_pFuzzyAttributesModel->item(i,2);
        if(!pItemB)
            continue;

        m_pFuzzyLabelMapUtility->updateMaxValue(pItemB->data(Qt::DisplayRole).toDouble());

        m_pFuzzyLabelMapUtility->calculateValues(lblMap,atName);
    }

    FuzzyProcessor *fProcessor = dynamic_cast<FuzzyProcessor*>(processor_);
    if(!fProcessor)
        return;

    fProcessor->setOutputData(lblMap);

    QString text = m_pFuzzyLabelMapUtility->constructCsvFromLabelMap(lblMap);

    fProcessor->setTextOutputData(text.toStdString());

}

QStringList FuzzyProcessorWidget::getOntologyClassesFromPort(){
    QStringList list;
    FuzzyProcessor *fProcessor = dynamic_cast<FuzzyProcessor*>(processor_);
    if(!fProcessor)
        return list;

    std::string text = fProcessor->getOntologyData();
    QString text_ = QString::fromStdString(text);

    OWLParser parser;
    parser.parseContent(text_);

    OWLHelperItem *rootItem = parser.rootOWLHelperItem();

    if(rootItem)
        processOntologyItem(rootItem,list);

    return list;
}

void FuzzyProcessorWidget::processOntologyItem(OWLHelperItem *item, QStringList &list){
    QList<OWLHelperItem*> childList = item->owlChildren();
    QList<OWLHelperItem*>::const_iterator i;
    for(i = childList.constBegin(); i != childList.constEnd(); i++){
        OWLHelperItem *pItem = *i;
        list << pItem->label();
        processOntologyItem(pItem,list);
    }
}

void FuzzyProcessorWidget::setupOntologyClassItems(const QStringList &list){
    // clear all previously added ontolgogy classes
    FUZZYONTOLOGYCLASSMANAGER->clear();
    m_pOntologyClassComboBox->clear();


    QStringList::const_iterator i;
    int counter = 1;
    for(i = list.constBegin(); i!= list.constEnd(); i++){
        QString text = *i;

        m_pOntologyClassComboBox->addItem(text);
        m_pOntologyClassComboBox->setCurrentIndex(m_pOntologyClassComboBox->count()-1);

        FuzzyOntologyClass *pClass = new FuzzyOntologyClass(this);
        pClass->setClassName(text);
        pClass->setId(counter++);

        FUZZYONTOLOGYCLASSMANAGER->addFuzzyOntologyClass(pClass);

        m_pCurrentFuzzyOntologyClass = pClass;
    }
}

void FuzzyProcessorWidget::onRadioButtonMinMaxChanged(){
    if(m_pMaxRadioButton->isChecked()){
        if(m_pCurrentFuzzyOntologyClass)
            m_pCurrentFuzzyOntologyClass->setMinMaxType(FuzzyOntologyClass::MMT_MIN);
    }
    else{
        if(m_pCurrentFuzzyOntologyClass)
            m_pCurrentFuzzyOntologyClass->setMinMaxType(FuzzyOntologyClass::MMT_MAX);
    }
}

void FuzzyProcessorWidget::updateOutPortTextData(){
    FuzzyProcessor *fProcessor = dynamic_cast<FuzzyProcessor*>(processor_);
    if(!fProcessor)
        return;

    QString text = constructXmlFile();

    qDebug() << text;

    fProcessor->setTextOutputData(text.toStdString());
}


QString FuzzyProcessorWidget::constructXmlFile(){
    QString text;

    QDomDocument doc = QDomDocument();

    QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction("xml", "version=\"1.0\"");
    doc.appendChild(xmlDeclaration);

    QDomElement rootElement = doc.createElement(QLatin1String("fuzzy"));
    doc.appendChild(rootElement);

    QList<FuzzyOntologyClass*> fuzzyOntologyList = FUZZYONTOLOGYCLASSMANAGER->fuzzyOntologyClasses();

    QList<FuzzyOntologyClass*>::const_iterator i;
    for(i = fuzzyOntologyList.constBegin(); i != fuzzyOntologyList.constEnd(); i++){
        FuzzyOntologyClass *pClass = *i;

        if(pClass->isEmpty())
            continue;

//        DomElement fuzzyRuleElement = doc.createElement(QLatin1String("fuzzyRule"));
//        fuzzyRuleElement.setAttribute(QLatin1String("id"),pClass->id());
//        fuzzyRuleElement.setAttribute(QLatin1String("operator"),pClass->minMaxType());

//        QDomElement fuzzyRuleElemet = doc.createElement(QLatin1String("fuzzyRule"));
//        fuzzyRuleElemet.setAttribute(QLatin1String("id"),pRule->id());
//        fuzzyRuleElemet.setAttribute(QLatin1String("operator"),pRule->opr());
//        rootElement.appendChild(fuzzyRuleElemet);

//        QDomElement bodyElement = doc.createElement(QLatin1String("body"));
//        fuzzyRuleElemet.appendChild(bodyElement);

//        QList<FuzzyRestriction*> rlist = pRule->restrictions();
//        QList<FuzzyRestriction*>::const_iterator j;
//        for(j = rlist.constBegin(); j != rlist.constEnd(); j++){
//            FuzzyRestriction *pRestriction = *j;

//            QDomElement fuzzyRestrictionElement = doc.createElement("restriction");
//            fuzzyRestrictionElement.setAttribute(QLatin1String("property"),pRestriction->fuzzyProperty);
//            fuzzyRestrictionElement.setAttribute(QLatin1String("value"),QString::number(pRestriction->val,'f',2));
//            fuzzyRestrictionElement.setAttribute(QLatin1String("operator"),getTextOperatorFromSymbol(pRestriction->opr));
//            bodyElement.appendChild(fuzzyRestrictionElement);
//        }

//        QDomElement headElement = doc.createElement(QLatin1String("head"));
//        headElement.setAttribute(QLatin1String("class"),pRule->className());
//        fuzzyRuleElemet.appendChild(headElement);

    }

    text = doc.toString(4);

    return text;
}


void FuzzyProcessorWidget::onComboboxCurrentIndexChanged(const QString &text){
    FuzzyOntologyClass *pClass = FUZZYONTOLOGYCLASSMANAGER->fuzzyByOntologyClassByName(text);
    if(pClass)
    {
        m_pCurrentFuzzyOntologyClass = pClass;

        setupWidgetByCurrentOntologyClass();
    }
}


void FuzzyProcessorWidget::onFuzzyAttributeModelChanged(QStandardItem *pItem){
    Q_UNUSED(pItem);
}


void FuzzyProcessorWidget::setupWidgetByCurrentOntologyClass(){
    if(!m_pCurrentFuzzyOntologyClass)
        return;

    m_pFuzzyAttributesModel->removeRows(0,m_pFuzzyAttributesModel->rowCount());

//    QString opr = m_pCurrentRule->opr();
//    if(opr.compare(QString::fromAscii(MINOPERATOR)))
//        m_pMinRadioButton->setChecked(true);
//    else
//        m_pMaxRadioButton->setChecked(true);

//    QList<FuzzyRestriction*> list = m_pCurrentRule->restrictions();
//    QList<FuzzyRestriction*>::const_iterator i;
//    for(i = list.constBegin(); i != list.constEnd(); i++){
//        FuzzyRestriction *pRestriction = *i;

//        QStandardItem *item0 = new QStandardItem;
//        QStandardItem *item1 = new QStandardItem;
//        QStandardItem *item2 = new QStandardItem;

//        item0->setData(pRestriction->fuzzyProperty,Qt::DisplayRole);
//        item1->setData(pRestriction->opr,Qt::DisplayRole);
//        item2->setData(QString::number(pRestriction->val,'f',2),Qt::DisplayRole);

//        QList<QStandardItem*> l;
//        l << item0 << item1 << item2;

//        m_pRulesModel->appendRow(l);
//    }
}

FuzzyProcessorWidget::~FuzzyProcessorWidget(){
//    ItiOtbImageManager::deleteInstance();
}

} //namespace voreen
