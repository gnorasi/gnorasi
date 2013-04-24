#include "fuzzyprocessorwidget.h"

#include <QDomDocument>
#include <QDomElement>

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
    QLabel *pLabelMinTitle = new QLabel(tr("Min : "),this);
    QLabel *pLabelMaxTitle = new QLabel(tr("Max : "),this);

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
        int code = 97; // 'a'
        code += i;

        QString lstr = QString("%1").arg((char)code);

        headers1 << lstr;
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
    m_pOntologyClassComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    m_pMinRadioButton = new QRadioButton(this);
    m_pMaxRadioButton = new QRadioButton(this);


    QButtonGroup *pGroup = new QButtonGroup(this);
    pGroup->addButton(m_pMinRadioButton);
    pGroup->addButton(m_pMaxRadioButton);

    QVBoxLayout *vboxLayout4 = new QVBoxLayout;
    vboxLayout4->addWidget(pLabelMinTitle);
    vboxLayout4->addWidget(pLabelMaxTitle);

    QVBoxLayout *vboxLayout5 = new QVBoxLayout;
    vboxLayout5->addWidget(m_pMinRadioButton);
    vboxLayout5->addWidget(m_pMaxRadioButton);


    QHBoxLayout *hboxLayout1 = new QHBoxLayout();
    hboxLayout1->addWidget(m_pOntologyClassComboBox);
    hboxLayout1->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxLayout1->addLayout(vboxLayout4);
    hboxLayout1->addLayout(vboxLayout5);


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

    //
    m_pFuzzyLabelMapUtility = new FuzzyLabelMapUtility(this);

    //
    setupItemDelegates();

    //
    m_pFuzzyFunctionFactory = new FuzzyFunctionFactory(this);

    connect(m_pPushButtonAdd,SIGNAL(clicked()),this,SLOT(createANewAttribute()));
    connect(m_pPushButtonCalculate,SIGNAL(clicked()),this,SLOT(calculate()));
    connect(m_pPushButtonRemove,SIGNAL(clicked()),this,SLOT(removeSelection()));
    connect(m_pOntologyClassComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(onComboboxCurrentIndexChanged(QString)));
    connect(m_pMinRadioButton,SIGNAL(pressed()),this,SLOT(onRadioButtonMixMaxChanged()));
    connect(m_pMaxRadioButton,SIGNAL(pressed()),this,SLOT(onRadioButtonMixMaxChanged()));
    connect(m_pFuzzyAttributesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onFuzzyAttributeModelChanged(QStandardItem*)));

    hide();
}

void FuzzyProcessorWidget::setupItemDelegates(){
    DoubleSpinBoxDelegate *spinBoxDelegate = new DoubleSpinBoxDelegate(0.0, 1.0, 2, this);
    m_pFuzzyAttributesTableView->setItemDelegateForColumn(m_pFuzzyAttributesModel->columnCount()-1,spinBoxDelegate);

    for(int i = 2;  i < FuzzyFunction::MAXPARAMETERSCOUNT+2; i++)
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


void FuzzyProcessorWidget::createANewAttribute(){
    Q_ASSERT(m_pCurrentFuzzyOntologyClass);

    QModelIndex index = m_pAvailableAttributesListView->currentIndex();

    if(!index.isValid())
        return;

    QString name = m_pAvailableAttributesModel->data(index).toString();

    int lastIndexOfSemicolon = name.lastIndexOf(":");
    QString newName = name;
    newName = newName.insert(lastIndexOfSemicolon+1,QLatin1String("Fuzzy"));

    // parse all the class and attributes in order to get the number of attriutes already assigned to this kind of available attribute
    int counter = 1;
    QList<FuzzyOntologyClass*> oclasslist = FUZZYONTOLOGYCLASSMANAGER->fuzzyOntologyClasses();
    QList<FuzzyOntologyClass*>::const_iterator o;
    for(o = oclasslist.constBegin(); o != oclasslist.constEnd(); o++){
        FuzzyOntologyClass *pClass = *o;
        QList<FuzzyAttribute*> list = pClass->fuzzyAttributes();
        QList<FuzzyAttribute*>::const_iterator i;

        // now parse the attributes in order check for same name
        for(i = list.constBegin(); i != list.constEnd(); i++){
            FuzzyAttribute *pAttribute = *i;

            QString atName = pAttribute->valueName();

            if(atName.contains(newName))
                counter++;
        }
    }

    // then just append the counter number to the string
    newName = newName.append(QString::number(counter));

    // create a new attribute
    FuzzyAttribute *pFuzzyAttribute = new FuzzyAttribute(name,newName,this);
    m_pCurrentFuzzyOntologyClass->addFuzzyAttribute(pFuzzyAttribute);

    // create a new item
    QStandardItem *pItem = new QStandardItem();
    pItem->setText(name);
    pItem->setData(newName,Qt::UserRole);

    // append a new row to the model based on the item previously created
    m_pFuzzyAttributesModel->appendRow(pItem);

    m_pFuzzyAttributesTableView->resizeColumnToContents(0);
}


void FuzzyProcessorWidget::removeSelection(){
    QModelIndex index = m_pFuzzyAttributesTableView->currentIndex();

    if(!index.isValid())
        return;

    QString name = m_pFuzzyAttributesModel->data(index,Qt::DisplayRole).toString();

    if(m_pCurrentFuzzyOntologyClass->removeFuzzyAttribute(name))
        m_pFuzzyAttributesModel->removeRow(index.row());
}

/*!
 * \brief FuzzyProcessorWidget::calculate
 */
void FuzzyProcessorWidget::calculate(){

    FuzzyProcessor *fProcessor                              = dynamic_cast<FuzzyProcessor*>(processor_);
    if(!fProcessor)
        return;


    // get all the avaialable attributes
    QList<FuzzyAttribute*> aList                            = FUZZYONTOLOGYCLASSMANAGER->getAllAttributes();

    // get the label map
    FuzzyLabelMapUtility::LabelMapType *lblMap              = getMapFromPort();

    // iterate through all the objects in the map
    for(unsigned int i = 1; i < lblMap->GetNumberOfLabelObjects(); i++){
        FuzzyLabelMapUtility::LabelObjectType* lblObject    = lblMap->GetLabelObject(i);

        // iterate through all all the attributes
        QList<FuzzyAttribute*>::const_iterator a;
        for(a = aList.constBegin(); a != aList.constEnd(); a++){
            FuzzyAttribute *pAtr = *a;

            QString propName                                = pAtr->displayName();

            double val                                      = (double)lblObject->GetAttribute(propName.toLatin1().constData());
            double calcval                                  = pAtr->calculateMembershipValue(val);

            int idx                                         = propName.lastIndexOf("::");
            idx                                             += 2;

            lblObject->SetAttribute(pAtr->valueName().toLatin1().constData(),calcval);
        }
    }

    // construct csv text data
    QString csvtextdata                                     = m_pFuzzyLabelMapUtility->constructCsvFromLabelMap(lblMap);
    fProcessor->setTextOutputData(csvtextdata.toStdString());

    // START OF TEST
    QFile file(QFileDialog::getSaveFileName(this,tr("Save"),QDir::homePath()));
    if(!file.fileName().isEmpty()){
        if(file.open(QIODevice::WriteOnly)){
            QTextStream out(&file);
            out << csvtextdata;
            file.close();
        }
    }
    // END OF TEST

    // set the fuzzy rules data to the processor in xml format
    QString xmldata                                         = constructXmlFile();
    fProcessor->setFuzzyRuleTextOutputData(xmldata.toStdString());

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

    int counter = 1;

    QList<FuzzyOntologyClass*>::const_iterator i;
    for(i = fuzzyOntologyList.constBegin(); i != fuzzyOntologyList.constEnd(); i++){
        FuzzyOntologyClass *pClass = *i;

        if(pClass->isEmpty())
            continue;

        QDomElement fuzzyRuleElement = doc.createElement(QLatin1String("fuzzyRule"));
        fuzzyRuleElement.setAttribute(QLatin1String("id"),QString::number(counter++));
        fuzzyRuleElement.setAttribute(QLatin1String("operator"),pClass->mixMaxName());
        rootElement.appendChild(fuzzyRuleElement);

        QDomElement bodyElement = doc.createElement(QLatin1String("body"));
        fuzzyRuleElement.appendChild(bodyElement);

        QList<FuzzyAttribute*> aList = pClass->fuzzyAttributes();
        QList<FuzzyAttribute*>::const_iterator j;
        for(j = aList.constBegin(); j != aList.constEnd(); j++){
            FuzzyAttribute *pAttr = *j;

            QDomElement fuzzyRestrictionElement = doc.createElement("restriction");
            fuzzyRestrictionElement.setAttribute(QLatin1String("property"),pAttr->valueName());
            fuzzyRestrictionElement.setAttribute(QLatin1String("value"),QString::number(pAttr->threshold(),'f',2));
            fuzzyRestrictionElement.setAttribute(QLatin1String("operator"),pAttr->fuzzyOperatorXmlName());
            bodyElement.appendChild(fuzzyRestrictionElement);
        }

        QDomElement headElement = doc.createElement(QLatin1String("head"));
        headElement.setAttribute(QLatin1String("class"),pClass->className());
        fuzzyRuleElement.appendChild(headElement);

    }

    text = doc.toString(4);


    // START OF TEST
    QFile file(QFileDialog::getSaveFileName(this,tr("Save"),QDir::homePath()));
    if(!file.fileName().isEmpty()){
        if(file.open(QIODevice::WriteOnly)){
            QTextStream out(&file);
            out << text;
            file.close();
        }
    }
    // END OF TEST

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
    // sanity check
    Q_ASSERT(m_pCurrentFuzzyOntologyClass);

    QVariant val = pItem->data(Qt::DisplayRole);

    int index = pItem->column();

    int row = pItem->row();

    m_pCurrentFuzzyOntologyClass->updateAttribute(row,index,val,m_pFuzzyFunctionFactory);
}


void FuzzyProcessorWidget::setupWidgetByCurrentOntologyClass(){
    // sanity check
    Q_ASSERT(m_pCurrentFuzzyOntologyClass);

    m_pFuzzyAttributesModel->removeRows(0,m_pFuzzyAttributesModel->rowCount());

    FuzzyOntologyClass::MINMAXTYPE mmt = m_pCurrentFuzzyOntologyClass->minMaxType();
    if(mmt == FuzzyOntologyClass::MMT_MIN)
        m_pMinRadioButton->setChecked(true);
    else
        m_pMaxRadioButton->setChecked(true);

    QList<FuzzyAttribute*> list = m_pCurrentFuzzyOntologyClass->fuzzyAttributes();
    QList<FuzzyAttribute*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        FuzzyAttribute *pFuzzyAttribute = *i;

        QList<QStandardItem*> l;

        QStandardItem *item0 = new QStandardItem;
        QStandardItem *item1 = new QStandardItem;

        item0->setData(pFuzzyAttribute->displayName(),Qt::DisplayRole);
        item0->setData(pFuzzyAttribute->valueName(),Qt::UserRole);

        FuzzyFunction *pFuzzyFunction = pFuzzyAttribute->fuzzyFunction();
        if(pFuzzyFunction)
            item1->setData(pFuzzyFunction->name(),Qt::DisplayRole);

        l << item0 << item1;

        for(int j = 0; j < pFuzzyFunction->MAXPARAMETERSCOUNT; j++){
            QStandardItem *pItem_ = new QStandardItem;

            if(pFuzzyFunction && j < pFuzzyFunction->parametersCount())
                pItem_->setData(pFuzzyFunction->parameterFunctionForIndex(j),Qt::DisplayRole);

            l << pItem_;
        }

        QStandardItem *item2 = new QStandardItem;
        item2->setData(pFuzzyAttribute->fuzzyOperatorName(),Qt::DisplayRole);

        l << item2;

        QStandardItem *item3 = new QStandardItem;
        item3->setData(QString::number(pFuzzyAttribute->threshold(),'f',2),Qt::DisplayRole);

        l << item3;

        m_pFuzzyAttributesModel->appendRow(l);
    }
}


FuzzyProcessorWidget::~FuzzyProcessorWidget(){
    FuzzyOntologyManager::deleteInstance();
}

} //namespace voreen
