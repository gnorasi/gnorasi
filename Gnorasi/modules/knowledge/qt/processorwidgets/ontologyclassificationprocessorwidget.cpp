#include "ontologyclassificationprocessorwidget.h"

#include <QDomDocument>
#include <QDomElement>

#include "voreen/qt/voreenapplicationqt.h"

#include "doublespinboxdelegate.h"
#include "comboboxdelegate.h"
#include "lineeditfornumbersdelegate.h"

#include "classhierarchywidget.h"

#include "../fuzzy/fuzzyoperatormanager.h"
#include "../fuzzy/fuzzyattribute.h"
#include "../fuzzy/fuzzyfunctionfactory.h"
#include "../fuzzy/fuzzyfunction.h"
#include "../utils/fuzzylabelmaputility.h"
#include "../fuzzy/fuzzyrule.h"
#include "../fuzzy/fuzzyrulemanager.h"


#include "../utils/owlparser.h"
#include "../utils/owlhelperitem.h"
#include "../utils/objectattributemanager.h"
#include "../utils/objectattribute.h"
#include "../utils/objectlevelmanager.h"
#include "../utils/objectlevel.h"
#include "../utils/ontologyclassificationmanager.h"
#include "../utils/owlwriter.h"

#include "../models/ontologyclass.h"

using namespace otb;
//using namespace itiviewer;

namespace voreen {

const std::string OntologyClassificationProcessorWidget::loggerCat_("voreen.OntologyClassificationProcessorWidget");

OntologyClassificationProcessorWidget::OntologyClassificationProcessorWidget(QWidget *parent, OntologyClassificationProcessor *ocProcessor)
    : QProcessorWidget(ocProcessor, parent)
{
    tgtAssert(ocProcessor, "No OntologyClassificationProcessorWidget processor");

    setWindowTitle(QString::fromStdString(ocProcessor->getName()));
    resize(600, 500);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void OntologyClassificationProcessorWidget::initialize(){
    QProcessorWidget::initialize();

    //
    m_pFuzzyLabelMapUtility = new FuzzyLabelMapUtility(this);

    //
    m_pFuzzyFunctionFactory = new FuzzyFunctionFactory(this);

    /////////////////////////////////////////////////////////////////////
    /// \brief gui staff
    ///
    ///

    m_pPushButtonCalculate = new QPushButton(tr("Calculate"),this);
    QHBoxLayout *hboxlayout = new QHBoxLayout;
    hboxlayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout->addWidget(m_pPushButtonCalculate);

    m_pClassHierarchyWidget = new ClassHierarchyWidget(this);

    QVBoxLayout *vboxlayout = new QVBoxLayout;
    vboxlayout->addLayout(hboxlayout);
    vboxlayout->addWidget(m_pClassHierarchyWidget);

    setLayout(vboxlayout);

    connect(m_pPushButtonCalculate,SIGNAL(clicked()),this,SLOT(calculate()));

    hide();
}

//void OntologyClassificationProcessorWidget::setupItemDelegates(){

//}

//!
void OntologyClassificationProcessorWidget::updateFromProcessor(){
//    FuzzyLabelMapUtility::LabelMapType *mapT = getMapFromPort();
//    if(mapT){
//        // parse the fetched data by the port
//        m_pFuzzyLabelMapUtility->parse(mapT);

//        // setup the data by the parsed map
//        QStringList namesList = m_pFuzzyLabelMapUtility->getAttributeListNames();

//        qDebug() << namesList;
//    }
    processPortData();
}

void OntologyClassificationProcessorWidget::processAttributeList(int levelid, const QStringList &list){
    QStringList::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        QString name = *i;

        ObjectAttribute *oa = new ObjectAttribute(OBJECTATTRIBUTEMANAGER);
        oa->setId(name);
        oa->setName(name);
        oa->setlevelId(levelid);

        OBJECTATTRIBUTEMANAGER->addObjectAttribute(levelid,oa);
    }
}

void OntologyClassificationProcessorWidget::processPortData(){
    OntologyClassificationProcessor* ocProcessor = dynamic_cast<OntologyClassificationProcessor*>(processor_);

    if(!ocProcessor)
        return;

    std::vector<Port*> l = ocProcessor->getInports();
    if(l.empty()){
        qDebug() << "label map port list is empty..";

    }else{
        //! get the first port
        OBJECTATTRIBUTEMANAGER->clear();
        for(int i = 0; i < l.size(); i++){

            OBJECTLEVELMANAGER->clear();

            Port *port = l.at(i);
            std::vector<const Port*> l1 = port->getConnected();
            for(int h = 0; h < l1.size(); h++){

                ObjectLevel *oLevel = new ObjectLevel(OBJECTLEVELMANAGER);
                oLevel->setId(h);
                oLevel->setName(QString("Level %1").arg(QString::number(h)));
                OBJECTLEVELMANAGER->addObjectLevel(oLevel);

                const Port *port1 = l1.at(h);
                const OTBLabelMapPort *lblMapPort = static_cast<const OTBLabelMapPort*>(port1);
                if(lblMapPort){
                    OTBLabelMapPort::LabelMapType* dataMap = lblMapPort->getData();

                    m_pFuzzyLabelMapUtility->parse(dataMap);

                    // setup the data by the parsed map
                    QStringList namesList = m_pFuzzyLabelMapUtility->getAttributeListNames();

                    processAttributeList(h,namesList);
                }
            }
        }
    }
}

//void OntologyClassificationProcessorWidget::setupAvailableAttributesListViewByList(const QStringList &list){
//    m_pAvailableAttributesModel->removeRows(0,m_pAvailableAttributesModel->rowCount());

//    QStringList::const_iterator i;
//    for(i = list.constBegin(); i != list.constEnd(); i++){
//        QString name = *i;

//        QStandardItem *pItem = new QStandardItem();
//        pItem->setData(name,Qt::DisplayRole);

//        m_pAvailableAttributesModel->appendRow(pItem);
//    }
//}


//void OntologyClassificationProcessorWidget::createANewAttribute(){
//    Q_ASSERT(m_pCurrentFuzzyOntologyClass);

//    QModelIndex index = m_pAvailableAttributesListView->currentIndex();

//    if(!index.isValid())
//        return;

//    QString name = m_pAvailableAttributesModel->data(index).toString();

//    int lastIndexOfSemicolon = name.lastIndexOf(":");
//    QString newName = name;
//    newName = newName.insert(lastIndexOfSemicolon+1,QLatin1String("Fuzzy"));

//    // parse all the class and attributes in order to get the number of attriutes already assigned to this kind of available attribute
//    int counter = 1;
//    QList<FuzzyOntologyClass*> oclasslist = FUZZYONTOLOGYCLASSMANAGER->fuzzyOntologyClasses();
//    QList<FuzzyOntologyClass*>::const_iterator o;
//    for(o = oclasslist.constBegin(); o != oclasslist.constEnd(); o++){
//        FuzzyOntologyClass *pClass = *o;
//        QList<FuzzyAttribute*> list = pClass->fuzzyAttributes();
//        QList<FuzzyAttribute*>::const_iterator i;

//        // now parse the attributes in order check for same name
//        for(i = list.constBegin(); i != list.constEnd(); i++){
//            FuzzyAttribute *pAttribute = *i;

//            QString atName = pAttribute->valueName();

//            if(atName.contains(newName))
//                counter++;
//        }
//    }

//    // then just append the counter number to the string
//    newName = newName.append(QString::number(counter));

//    // create a new attribute
//    FuzzyAttribute *pFuzzyAttribute = new FuzzyAttribute(name,newName,this);
//    m_pCurrentFuzzyOntologyClass->addFuzzyAttribute(pFuzzyAttribute);

//    // create a new item
//    QStandardItem *pItem = new QStandardItem();
//    pItem->setText(name);
//    pItem->setData(newName,Qt::UserRole);

//    // append a new row to the model based on the item previously created
//    m_pFuzzyAttributesModel->appendRow(pItem);

//    m_pFuzzyAttributesTableView->resizeColumnToContents(0);
//}


//void OntologyClassificationProcessorWidget::removeSelection(){
//    QModelIndex index = m_pFuzzyAttributesTableView->currentIndex();

//    if(!index.isValid())
//        return;

//    QString name = m_pFuzzyAttributesModel->data(index,Qt::DisplayRole).toString();

//    if(m_pCurrentFuzzyOntologyClass->removeFuzzyAttribute(name))
//        m_pFuzzyAttributesModel->removeRow(index.row());
//}


QString OntologyClassificationProcessorWidget::constructCSVText(){

    OntologyClassificationProcessor *ocProcessor                              = dynamic_cast<OntologyClassificationProcessor*>(processor_);
    if(!ocProcessor)
        return QString();

    std::vector<Port*> l = ocProcessor->getInports();
    if(l.empty())
        return QString();

    QStringList lines;
    QString firstline = QString("id;level");

    QList<FuzzyRule*> fuzzyRuleList = FUZZYRULEMANAGER->fuzzyRuleList();
    QList<FuzzyRule*>::const_iterator k;
    for(k = fuzzyRuleList.constBegin(); k != fuzzyRuleList.constEnd(); k++){
        FuzzyRule *pFuzzyRule = *k;

        QString name = pFuzzyRule->name();
        QString fname = name.remove("::");

        fname = QString("Fuzzy%1%2").arg(fname).arg(QString::number(pFuzzyRule->id()));
        firstline.append(";");
        firstline.append(fname);
    }

    lines << firstline;

    //! get the first port
    for(int i = 0; i < l.size(); i++){

        Port *port = l.at(i);
        std::vector<const Port*> l1 = port->getConnected();
        for(int h = 0; h < l1.size(); h++){

            const Port *port1 = l1.at(h);
            const OTBLabelMapPort *lblMapPort = static_cast<const OTBLabelMapPort*>(port1);
            if(lblMapPort){
                OTBLabelMapPort::LabelMapType* dataMap = lblMapPort->getData();

                // iterate through all the objects in the map
                for(unsigned int i = 1; i < dataMap->GetNumberOfLabelObjects(); i++){
                    FuzzyLabelMapUtility::LabelObjectType* lblObject    = dataMap->GetLabelObject(i);

                    QString line;
                    line.append(QString::number(i));
                    line.append(";");
                    line.append(QString::number(h));

                    for(k = fuzzyRuleList.constBegin(); k != fuzzyRuleList.constEnd(); k++){
                        FuzzyRule *pFuzzyRule = *k;

                        FuzzyFunction *ffunction = pFuzzyRule->funzzyFunction();
                        if(!ffunction)
                            continue;

                        QString attname = pFuzzyRule->name();
                        double val                                      = (double)lblObject->GetAttribute(attname.toLatin1().constData());
                        double calcval = ffunction->calculate(val);

                        line.append(";");
                        line.append(QString::number(calcval,'f',3));
                    }

                    lines << line;
                }
            }
        }
    }

    return lines.join("\n");
}

/*!
 * \brief OntologyClassificationProcessorWidget::calculate
 */
void OntologyClassificationProcessorWidget::calculate(){
    OntologyClassificationProcessor *ocProcessor                              = dynamic_cast<OntologyClassificationProcessor*>(processor_);
    if(!ocProcessor)
        return;

    ////
    /// \brief csvText, the csv text constructed
    ///
    QString csvText = constructCSVText();

//    QFile file(QFileDialog::getSaveFileName(this,tr("Ok"),QDir::homePath()));
//    file.open(QIODevice::WriteOnly);
//    QTextStream out(&file);
//    out << csvText;
//    file.close();

    ////
    /// \brief xmltext, the xml constructed text
    ///
    QString xmltext = constructXmlFile();
//    QFile file1(QFileDialog::getSaveFileName(this,tr("Ok"),QDir::homePath()));
//    file1.open(QIODevice::WriteOnly);
//    QTextStream out1(&file1);
//    out1 << xmltext;
//    file1.close();

    std::vector<Port*> outv = ocProcessor->getOutports();
    for(int i = 0; i < outv.size(); i++){
        Port *port = outv.at(i);
        TextPort *textPort = dynamic_cast<TextPort*>(port);
        if(!textPort)
            continue;

        QString desc = QString::fromStdString(port->getDescription());

        if(!desc.compare(QLatin1String("1"))) { // this is the text port
            textPort->setData(csvText.toStdString());
        }else if(!desc.compare(QLatin1String("2"))) { // this is the xml port
            textPort->setData(xmltext.toStdString());
        }
    }



//    // get all the avaialable attributes
//    QList<FuzzyAttribute*> aList                            = FUZZYONTOLOGYCLASSMANAGER->getAllAttributes();

//    // get the label map
//    FuzzyLabelMapUtility::LabelMapType *lblMap              = getMapFromPort();

//    // iterate through all the objects in the map
//    for(unsigned int i = 1; i < lblMap->GetNumberOfLabelObjects(); i++){
//        FuzzyLabelMapUtility::LabelObjectType* lblObject    = lblMap->GetLabelObject(i);

//        // iterate through all all the attributes
//        QList<FuzzyAttribute*>::const_iterator a;
//        for(a = aList.constBegin(); a != aList.constEnd(); a++){
//            FuzzyAttribute *pAtr = *a;

//            if(!pAtr->isReady())
//                continue;

//            QString propName                                = pAtr->displayName();

//            double val                                      = (double)lblObject->GetAttribute(propName.toLatin1().constData());
//            double calcval                                  = pAtr->calculateMembershipValue(val);

//            int idx                                         = propName.lastIndexOf("::");
//            idx                                             += 2;

//            lblObject->SetAttribute(pAtr->valueName().toLatin1().constData(),calcval);
//        }
//    }

    // construct csv text data
//    QString csvtextdata                                     = m_pFuzzyLabelMapUtility->constructCsvFromLabelMap(lblMap);
//    ocProcessor->setTextOutputData(csvtextdata.toStdString());

    // START OF TEST
//    QFile file(QFileDialog::getSaveFileName(this,tr("Save"),QDir::homePath()));
//    if(!file.fileName().isEmpty()){
//        if(file.open(QIODevice::WriteOnly)){
//            QTextStream out(&file);
//            out << csvtextdata;
//            file.close();
//        }
//    }
    // END OF TEST

    // set the fuzzy rules data to the processor in xml format
//    QString xmldata                                         = constructXmlFile();
//    fProcessor->setFuzzyRuleTextOutputData(xmldata.toStdString());

}


//void OntologyClassificationProcessorWidget::processOntologyItem(OWLHelperItem *item, QStringList &list){
//    QList<OWLHelperItem*> childList = item->owlChildren();
//    QList<OWLHelperItem*>::const_iterator i;
//    for(i = childList.constBegin(); i != childList.constEnd(); i++){
//        OWLHelperItem *pItem = *i;
//        list << pItem->label();
//        processOntologyItem(pItem,list);
//    }
//}

//void OntologyClassificationProcessorWidget::setupOntologyClassItems(const QStringList &list){
//    // clear all previously added ontolgogy classes
//    FUZZYONTOLOGYCLASSMANAGER->clear();
//    m_pOntologyClassComboBox->clear();


//    QStringList::const_iterator i;
//    int counter = 1;
//    for(i = list.constBegin(); i!= list.constEnd(); i++){
//        QString text = *i;

//        m_pOntologyClassComboBox->addItem(text);
//        m_pOntologyClassComboBox->setCurrentIndex(m_pOntologyClassComboBox->count()-1);

//        FuzzyOntologyClass *pClass = new FuzzyOntologyClass(this);
//        pClass->setClassName(text);
//        pClass->setId(counter++);

//        FUZZYONTOLOGYCLASSMANAGER->addFuzzyOntologyClass(pClass);

//        m_pCurrentFuzzyOntologyClass = pClass;
//    }
//}

//void OntologyClassificationProcessorWidget::onRadioButtonMinMaxChanged(){
//    if(m_pMaxRadioButton->isChecked()){
//        if(m_pCurrentFuzzyOntologyClass)
//            m_pCurrentFuzzyOntologyClass->setMinMaxType(FuzzyOntologyClass::MMT_MIN);
//    }
//    else{
//        if(m_pCurrentFuzzyOntologyClass)
//            m_pCurrentFuzzyOntologyClass->setMinMaxType(FuzzyOntologyClass::MMT_MAX);
//    }
//}

void OntologyClassificationProcessorWidget::updateOutPortTextData(){
    OntologyClassificationProcessor *ocProcessor = dynamic_cast<OntologyClassificationProcessor*>(processor_);
    if(!ocProcessor)
        return;

    QString text = constructXmlFile();

    qDebug() << text;

//    ocProcessor->setTextOutputData(text.toStdString());
}


QString OntologyClassificationProcessorWidget::constructXmlFile(){
    QString text;

//    QDomDocument doc = QDomDocument();

//    QDomProcessingInstruction xmlDeclaration = doc.createProcessingInstruction("xml", "version=\"1.0\"");
//    doc.appendChild(xmlDeclaration);

//    QDomElement rootElement = doc.createElement(QLatin1String("fuzzy"));
//    doc.appendChild(rootElement);


//    text = doc.toString(4);

//    // START OF TEST
//    QFile file(QFileDialog::getSaveFileName(this,tr("Save"),QDir::homePath()));
//    if(!file.fileName().isEmpty()){
//        if(file.open(QIODevice::WriteOnly)){
//            QTextStream out(&file);
//            out << text;
//            file.close();
//        }
//    }
//    // END OF TEST


    OwlWriter writter;
    writter.createDocumentVersion2();

    QList<OntologyClass*> helperlist;
    QList<OntologyClass*> list = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassList();
    QList<OntologyClass*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        OntologyClass *pClass = *i;
        if(pClass->childCount())
            continue;
        else
            helperlist << pClass;
    }

    QList<OntologyClass*>::const_iterator h;
    for(h = helperlist.constBegin(); h != helperlist.constEnd(); h++){
        OntologyClass *pClass = *h;

        writter.appendData(pClass);
    }

    writter.appendRulesData();

    text = writter.docToText();

    ////////////////////////////
    /// \brief testing
    ///
//    QFile file(QFileDialog::getSaveFileName(this,tr("save"),QDir::homePath()));
//    if(file.open(QIODevice::WriteOnly)){
//        QTextStream out(&file);
//        out << text;

//        file.close();
//    }

    return text;
}


//void OntologyClassificationProcessorWidget::onComboboxCurrentIndexChanged(const QString &text){
//    FuzzyOntologyClass *pClass = FUZZYONTOLOGYCLASSMANAGER->fuzzyByOntologyClassByName(text);
//    if(pClass)
//    {
//        m_pCurrentFuzzyOntologyClass = pClass;

//        setupWidgetByCurrentOntologyClass();
//    }
//}


//void OntologyClassificationProcessorWidget::onFuzzyAttributeModelChanged(QStandardItem *pItem){
//    // sanity check
//    Q_ASSERT(m_pCurrentFuzzyOntologyClass);

//    QVariant val = pItem->data(Qt::DisplayRole);

//    int index = pItem->column();

//    int row = pItem->row();

//    m_pCurrentFuzzyOntologyClass->updateAttribute(row,index,val,m_pFuzzyFunctionFactory);
//}


//void OntologyClassificationProcessorWidget::setupWidgetByCurrentOntologyClass(){
//    // sanity check
//    Q_ASSERT(m_pCurrentFuzzyOntologyClass);

//    m_pFuzzyAttributesModel->removeRows(0,m_pFuzzyAttributesModel->rowCount());

//    FuzzyOntologyClass::MINMAXTYPE mmt = m_pCurrentFuzzyOntologyClass->minMaxType();
//    if(mmt == FuzzyOntologyClass::MMT_MIN)
//        m_pMinRadioButton->setChecked(true);
//    else
//        m_pMaxRadioButton->setChecked(true);

//    QList<FuzzyAttribute*> list = m_pCurrentFuzzyOntologyClass->fuzzyAttributes();
//    QList<FuzzyAttribute*>::const_iterator i;
//    for(i = list.constBegin(); i != list.constEnd(); i++){
//        FuzzyAttribute *pFuzzyAttribute = *i;

//        QList<QStandardItem*> l;

//        QStandardItem *item0 = new QStandardItem;
//        QStandardItem *item1 = new QStandardItem;

//        item0->setData(pFuzzyAttribute->displayName(),Qt::DisplayRole);
//        item0->setData(pFuzzyAttribute->valueName(),Qt::UserRole);

//        FuzzyFunction *pFuzzyFunction = pFuzzyAttribute->fuzzyFunction();
//        if(pFuzzyFunction)
//            item1->setData(pFuzzyFunction->name(),Qt::DisplayRole);

//        l << item0 << item1;

//        for(int j = 0; j < pFuzzyFunction->MAXPARAMETERSCOUNT; j++){
//            QStandardItem *pItem_ = new QStandardItem;

//            if(pFuzzyFunction && j < pFuzzyFunction->parametersCount())
//                pItem_->setData(pFuzzyFunction->parameterValueForIndex(j),Qt::DisplayRole);

//            l << pItem_;
//        }

//        QStandardItem *item2 = new QStandardItem;
//        item2->setData(pFuzzyAttribute->fuzzyOperatorName(),Qt::DisplayRole);

//        l << item2;

//        QStandardItem *item3 = new QStandardItem;
//        item3->setData(QString::number(pFuzzyAttribute->threshold(),'f',2),Qt::DisplayRole);

//        l << item3;

//        m_pFuzzyAttributesModel->appendRow(l);
//    }
//}


OntologyClassificationProcessorWidget::~OntologyClassificationProcessorWidget(){
//    FuzzyOntologyManager::deleteInstance();
}

} //namespace voreen
