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
#include "../utils/ontologyclassificationfilereader.h"

#include "voreen/core/voreenapplication.h"

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
    m_pPushButtonSave = new QPushButton(tr("Save"),this);
    m_pPushButtonLoad = new QPushButton(tr("Load"),this);
    QHBoxLayout *hboxlayout = new QHBoxLayout;
    hboxlayout->addWidget(m_pPushButtonLoad);
    hboxlayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout->addWidget(m_pPushButtonSave);
    hboxlayout->addWidget(m_pPushButtonCalculate);

    m_pClassHierarchyWidget = new ClassHierarchyWidget(this);

    QVBoxLayout *vboxlayout = new QVBoxLayout;
    vboxlayout->addLayout(hboxlayout);
    vboxlayout->addWidget(m_pClassHierarchyWidget);

    setLayout(vboxlayout);

    connect(m_pPushButtonCalculate,SIGNAL(clicked()),this,SLOT(calculate()));
    connect(m_pPushButtonSave,SIGNAL(clicked()),this,SLOT(save()));
    connect(m_pPushButtonLoad,SIGNAL(clicked()),this,SLOT(load()));

    hide();
}

//!
void OntologyClassificationProcessorWidget::updateFromProcessor(){

    processPortData();
}

void OntologyClassificationProcessorWidget::processAttributeList(int levelid, const QStringList &list){

    QString shapeString = QLatin1String("SHAPE");
    QStringList::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        QString name = *i;

        ObjectAttribute *oa = new ObjectAttribute(OBJECTATTRIBUTEMANAGER);
        oa->setId(name);
        oa->setName(name);
        oa->setlevelId(levelid);

        if(name.startsWith(shapeString))
            oa->setotype(2);

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

                    OBJECTATTRIBUTEMANAGER->createPreDefinedAttributesForLevel(h);
                }
            }
        }
    }
}


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

                        ObjectAttribute *pAttr = OBJECTATTRIBUTEMANAGER->objectAttributeOfLevelById(h,attname);
                        if(pAttr->otype() == 3)
                            continue;

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

    ////
    /// \brief xmltext, the xml constructed text
    ///
    QString xmltext = constructXmlFile();

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

}


QString OntologyClassificationProcessorWidget::getFilePath(){

    QString path;

    OntologyClassificationProcessor *ocProcessor                              = dynamic_cast<OntologyClassificationProcessor*>(processor_);
    if(!ocProcessor)
        return path;

    //
    // This may not work on Linux
    //
    path = QString::fromStdString(ocProcessor->getFilePath());
    QString workspacepath = QString::fromStdString(VoreenApplication::app()->getResourcePath("workspaces"));
    path.remove(workspacepath);
    path.remove(0,1);

    qDebug() << "path : " << path << "workspacepath : " << workspacepath;

    return path;
}

void OntologyClassificationProcessorWidget::save(){

    QString path = getFilePath();

    if(path.isEmpty()){
        QMessageBox::critical(this,tr("Error"),tr("Could not save the file.\nError message : Path is empty.\mPlease select a file path and try again"));
        return;
    }

    QString text = constructXmlFile();

    QFile file(path);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        out << text;

        file.close();

        QMessageBox::information(this,tr("Save"),tr("Data saved successfully in the selected file"));
    }
}


void OntologyClassificationProcessorWidget::load(){

    QString path = getFilePath();

    if(path.isEmpty()){
        QMessageBox::critical(this,tr("Error"),tr("Could not save the file.\nError message : Path is empty.\mPlease select a file path and try again"));
        return;
    }

    qDebug() << "loading document path : " << path;

    ONTOLOGYCLASSIFICATIONMANAGER->clear();
    FUZZYRULEMANAGER->clear();

    OntologyClassificationFileReader reader;
    reader.parse(path);

    m_pClassHierarchyWidget->setupModel();
}

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

    OwlWriter writter;
    writter.createDocumentVersion2();

    QList<OntologyClass*> list = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassList();
    QList<OntologyClass*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){

        OntologyClass *pClass = *i;
        if(!pClass->parent())
            writter.appendData(pClass);
    }

    QList<ObjectLevel*> llist = OBJECTLEVELMANAGER->objectLevelList();
    QList<ObjectLevel*>::const_iterator l;
    for(l = llist.constBegin(); l != llist.constEnd(); l++){
        ObjectLevel *level = *l;

        writter.processObjectLevel(level);

    }

    writter.appendRulesData();

    writter.appendSpatialData();

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

OntologyClassificationProcessorWidget::~OntologyClassificationProcessorWidget(){
//    FuzzyOntologyManager::deleteInstance();
}

} //namespace voreen
