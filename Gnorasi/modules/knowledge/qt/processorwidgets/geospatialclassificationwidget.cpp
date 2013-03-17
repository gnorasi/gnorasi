#include "geospatialclassificationwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "../utils/owlhelperitem.h"

#include "../utils/owlparser.h"
#include "../utils/rulewriter.h"
#include "../utils/ruleutility.h"

namespace voreen {

const std::string GeoSpatialClassificationWidget::loggerCat_("voreen.GeospatialClassificationWidget");

GeoSpatialClassificationWidget::GeoSpatialClassificationWidget(QWidget *parent, GeospatialClassificationProcessor *geospatialClassificationProcessor)
    : QProcessorWidget(geospatialClassificationProcessor, parent)
{
    tgtAssert(geospatialClassificationProcessor, "No GeospatialClassificationWidget processor");

    setWindowTitle(QString::fromStdString(geospatialClassificationProcessor->getName()));
    resize(400, 400);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void GeoSpatialClassificationWidget::initialize(){
    QProcessorWidget::initialize();

    m_pOntologyClassModel = new OntologyClassModel( this, false);

    QLabel *pLabelHeader = new QLabel(tr("Ontology Classes"),this);

    m_pOntologyClassView = new OntologyClassView(this);
    m_pOntologyClassView->setModel(m_pOntologyClassModel);

    m_pRuleWidget = new RuleWidget(QString(),this);
    m_pRuleWidget->hide();

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(pLabelHeader);
    vboxLayout->addWidget(m_pOntologyClassView);
    setLayout(vboxLayout);

    //! setup the functionality of the dropdown widgets shown in the RuleItemView records..
    setupPropertyFeatures();
    setupLevels();

    //! update the RuleWidget on every double click event
    connect(m_pOntologyClassView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(onTreeViewDoubleClicked(QModelIndex)));
    connect(m_pRuleWidget,SIGNAL(dataChanged()),this,SLOT(onRuleWidgetJChanged()));

    initialized_ = true;

    hide();
}

//!
void GeoSpatialClassificationWidget::onTreeViewDoubleClicked(const QModelIndex &index){
    OntologyClassItem *item = static_cast<OntologyClassItem*>(index.internalPointer());

    if(!item)
        return;

    //! get all the available Ontology Class names
    QStringList ontologyClassIdList = m_pOntologyClassModel->classIdList(m_pOntologyClassModel->getRootItem());
    //! remove the name of the current OntologyClassItem
    ontologyClassIdList.removeOne(item->id());

    //! set the list of the names to the RuleWidget
    m_pRuleWidget->setupByOntologyClassItem(item, ontologyClassIdList);

    m_pRuleWidget->show();
}

//!
void GeoSpatialClassificationWidget::updateFromProcessor(){
    GeospatialClassificationProcessor* gcProcessor = dynamic_cast<GeospatialClassificationProcessor*>(processor_);

    if(!gcProcessor)
        return;

    RuleUtility *ru = RuleUtility::instance();

    QString commanstring = QString::fromStdString(gcProcessor->getCommandName());
    if(!commanstring.compare("save")){ // this means the save Button has been clicked
        //
        m_ruleWriter.createDocument();
        m_ruleWriter.appendData();
        m_ruleWriter.setNamespaces(ru->namespaceXmlns(),ru->namespaceXmlBase());

        //! save the QDomDocument content to a file location
        QString path = QFileDialog::getSaveFileName(this,tr("Save file"),QDir::homePath(),tr("Xml Files (*.xml)"));
        if(!path.isEmpty()){
            if(!path.contains(".xml"))
                path.append(".xml");

            QFile file(path);
            if(!file.open(QIODevice::WriteOnly))
                return;

            QTextStream out(&file);
            out << m_ruleWriter.docToText();

            file.close();
        }


        return;

    }else if(!commanstring.compare("open")){ // this means the open Button has been clicked

        // get the path from the port
        QString filePath = QString::fromStdString(gcProcessor->getFilePath());

        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly))
            return;

        //
        QString error;
        m_ruleParser.parseFile(filePath,&error);
        if(!error.isEmpty())
            qDebug() << error;

        //
        // now validate the content of the rules file given an existent OntologyClassModel
        //
        validateContent();

    }else{ // this means that the OntologyClassModel in the OntologyCreator processor has been modified
        qDebug() << "updating from processor..";

        std::vector<Port*> l = processor_->getInports();
        if(l.empty()){
            m_pOntologyClassModel->removeRows(0,m_pOntologyClassModel->rowCount());
            return;
        }

        OntologyPort *pP = dynamic_cast<OntologyPort*>(l.at(0));

        if(!pP)
            return;

        QString text = QString::fromStdString(pP->getData());

        m_owlParser.parseContent(text);

        OWLHelperItem *rootItem = m_owlParser.rootOWLHelperItem();
        if(rootItem){
            QModelIndex index = m_pOntologyClassModel->index(0,0);
            if(index.isValid())
                m_pOntologyClassModel->removeRows(0,m_pOntologyClassModel->rowCount(index.parent()),index.parent());

            m_pOntologyClassModel->traverseHierarchy(rootItem,m_pOntologyClassModel->getRootItem());
            m_pOntologyClassView->expandAll();
        }

        ru->setNamespaces(m_owlParser.namespaceXmlns(),m_owlParser.namespaceXmlBase());
    }
}

//
void GeoSpatialClassificationWidget::setupPropertyFeatures(){
    QFile file(":/voreenve/text/GeospatialFunctions.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();

    QStringList list = text.split("\n",QString::SkipEmptyParts);

    m_pRuleWidget->setFeaturePropertyList(list);
}

//
void GeoSpatialClassificationWidget::setupLevels(){
    QStringList list;
    for(int i=0; i<10; i++){
        list << QString::number(i+1);
    }

    m_pRuleWidget->setLevelList(list);
}

//!
//! Every class mentioned in the Rule utility must apply to a valid
//! OntologyClassItem item . This function is called after a namespace checking has been passed
//!
void GeoSpatialClassificationWidget::validateContent(){
    //! helper
    bool val = false;

    //! first check , check for namespaces
    RuleUtility *ru = RuleUtility::instance();
    QStringList names = ru->classNamesList();
    QStringList::const_iterator i;
    for(i = names.constBegin(); i != names.constEnd(); i++){
        QString name = *i;

        QModelIndexList list = m_pOntologyClassModel->match(m_pOntologyClassModel->index(0,0),Qt::DisplayRole,name,-1,Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
        if(list.isEmpty())
            break;
        else{
            bool helper = false;
            foreach(QModelIndex index, list){
                if(!index.column())
                    helper = true;
            }
            if(!helper){
                val = false;
                break;
            }else
                val = true;
        }
    }

    if(!val){
        ru->clear();
        QMessageBox::information(this,tr("Validation"),tr("Could not load the rules from the file.No appropriate Ontology hierrarchy found.Load first the correct Ontology and try again."));
    }
}

void GeoSpatialClassificationWidget::onRuleWidgetJChanged(){
    //
}

} //namespace voreen
