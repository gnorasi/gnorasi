#include "ontologycreatorwidget.h"
#include "voreen/qt/voreenapplicationqt.h"

#include "../utils/owlhelperitem.h"

#include "../utils/ruleutility.h"
#include "../models/rule.h"
#include "../models/ruleitem.h"

#define XMLNS_DEFAULTVALUE             "http://www.gnorasi.gr/ontology/generated_%1#"
#define XMLNS_BASEDEFAULTVALUE         "http://www.gnorasi.gr/ontology/generated_%1"

namespace voreen {

const std::string OntologyCreatorWidget::loggerCat_("voreen.OntologyCreatorWidget");

OntologyCreatorWidget::OntologyCreatorWidget(QWidget* parent, OntologyCreatorProcessor* ontologyCreatorProcessor)
    : QProcessorWidget(ontologyCreatorProcessor, parent)
{
    tgtAssert(ontologyCreatorProcessor, "No OntologyCreatorWidget processor");

    setWindowTitle(QString::fromStdString(ontologyCreatorProcessor->getName()));
    resize(400, 400);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

OntologyCreatorWidget::~OntologyCreatorWidget() {

}

void OntologyCreatorWidget::initialize() {

    m_pOntologyClassModel = new OntologyClassModel( this);

    QGroupBox *pGroupBox = new QGroupBox(this);
    pGroupBox->setTitle(tr("Namespace properties"));

    QLabel *pLabel0 = new QLabel(tr("xmlns"),this);
    m_pLineEditXmlns = new QLineEdit(this);

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    m_pLabelXmlBase = new QLabel(QString::fromAscii(XMLNS_BASEDEFAULTVALUE).arg(timestamp),this);
    m_pLabelXmlBase->setVisible(false);

    m_pLineEditXmlns->setText(QString::fromAscii(XMLNS_DEFAULTVALUE).arg(timestamp));

    QVBoxLayout *vboxLayout0 = new QVBoxLayout();
    vboxLayout0->addWidget(pLabel0);
    vboxLayout0->addWidget(m_pLineEditXmlns);

    pGroupBox->setLayout(vboxLayout0);

    QLabel *pLabelActions = new QLabel(tr("Actions"),this);

    QPushButton *addSubClassButton      = new QPushButton(QIcon(":/voreenve/icons/add1.png"),tr("Add subclass"),this);
    QPushButton *addSiblingClassButton  = new QPushButton(QIcon(":/voreenve/icons/add2.png"),tr("Add sibling class"),this);
    QPushButton *deleteClassButton      = new QPushButton(QIcon(":/voreenve/icons/delete.png"),tr("Delete class"),this);

    QLabel *pLabelHeader = new QLabel(tr("Ontology Classes"),this);

    m_pOntologyClassView = new OntologyClassView(this);
    m_pOntologyClassView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pOntologyClassView->setModel(m_pOntologyClassModel);

    addSubClassButton->setToolTip(tr("Add a new <strong>sub class</strong> to the class selected."));
    addSiblingClassButton->setToolTip(tr("Add a new sibling class to the class selected."));
    deleteClassButton->setToolTip(tr("Delete the class selected."));

    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(addSubClassButton);
    hBoxLayout->addWidget(addSiblingClassButton);
    hBoxLayout->addWidget(deleteClassButton);
    hBoxLayout->addSpacerItem(new QSpacerItem(100,20,QSizePolicy::Expanding,QSizePolicy::Fixed));

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(pGroupBox);
    vboxLayout->addWidget(pLabelActions);
    vboxLayout->addLayout(hBoxLayout);
    vboxLayout->addWidget(pLabelHeader);
    vboxLayout->addWidget(m_pOntologyClassView);

    setLayout(vboxLayout);

    connect(m_pOntologyClassModel,SIGNAL(nameValidationError()),m_pOntologyClassView,SLOT(onNameValidationError()));
    connect(m_pOntologyClassModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(onModelDataChanged(QModelIndex,QModelIndex)));
    connect(addSubClassButton,SIGNAL(clicked()),m_pOntologyClassView,SLOT(onAddChildClass()));
    connect(addSiblingClassButton,SIGNAL(clicked()),m_pOntologyClassView,SLOT(onAddSiblingClass()));
    connect(deleteClassButton,SIGNAL(clicked()),m_pOntologyClassView,SLOT(onRemoveCurrentClass()));

    QProcessorWidget::initialize();

    hide();

}


//!
//! This function is called whenever a new event is triggered by the OntologyCreatorProcessor
//! IO functionality and new Ontology functionality is handled..
//!
void OntologyCreatorWidget::updateFromProcessor() {
    OntologyCreatorProcessor* ontologyCreatorProcessor = dynamic_cast<OntologyCreatorProcessor*>(processor_);

    //! take the commandstring
    QString commanstring = QString::fromStdString(ontologyCreatorProcessor->getFilePath());

    //! check whether a new ontology is called
    if(!commanstring.compare(QString::fromUtf8("new"))){
        if(!m_pOntologyClassModel->rowCount())
            return;

        QModelIndex index = m_pOntologyClassModel->index(0,0);
        if(index.isValid())
            m_pOntologyClassModel->removeRows(0,m_pOntologyClassModel->rowCount(index.parent()),index.parent());

        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

        m_pLabelXmlBase->setText(QString::fromAscii(XMLNS_BASEDEFAULTVALUE).arg(timestamp));
        m_pLineEditXmlns->setText(QString::fromAscii(XMLNS_DEFAULTVALUE).arg(timestamp));

        //!
        //! now update the outport text data
        //!
        setupOutportText();

    } //! check whethe a save button is clicked and a file name selected to save the data
    else if(!commanstring.compare("save")){

        //! create a new domdocument
        m_owlWriter.createDocument();

        //! append the namespace data
        m_owlWriter.setupNamespaces(m_pLineEditXmlns->text(),m_pLabelXmlBase->text());

        //! iterate through the OntologyClassItem item hierarchy and append data
        QList<OntologyClassItem*> list = m_pOntologyClassModel->getRootItem()->getChildItems();
        QList<OntologyClassItem*>::const_iterator i;
        for(i = list.constBegin(); i != list.constEnd(); i++){
            OntologyClassItem *citem = *i;
            m_owlWriter.appendData(citem);
        }

        //! save the QDomDocument content to a file location
        QString path = QFileDialog::getSaveFileName(this,tr("Save file"),QDir::homePath(),tr("Ontology Files (*.owl)"));
        if(!path.isEmpty()){
            if(!path.contains(".owl"))
                path.append(".owl");

            QFile file(path);
            if(!file.open(QIODevice::WriteOnly))
                return;

            QTextStream out(&file);
            out << m_owlWriter.docToText();

            file.close();
        }
    } //! Else means that an open button is called and the ontology must be parsed from
      //! a owl file
    else{
        //! output the file path
        LINFO(ontologyCreatorProcessor->getFilePath());

        //! check if the file path is empty
        QString filePath = QString::fromStdString(ontologyCreatorProcessor->getFilePath());
        if(filePath.isEmpty())
            return;

        //! parse the file
        m_owlParser.parse(filePath);

        //! create helper items and setup the OntologyClassModel by these items
        OWLHelperItem *rootItem = m_owlParser.rootOWLHelperItem();
        if(rootItem){
            QModelIndex index = m_pOntologyClassModel->index(0,0);
            if(index.isValid()){

                //! clead the model
                m_pOntologyClassModel->removeRows(0,m_pOntologyClassModel->rowCount(index.parent()),index.parent());
            }

            //! append the namespaces
            m_pLineEditXmlns->setText(m_owlParser.namespaceXmlns());
            m_pLabelXmlBase->setText(m_owlParser.namespaceXmlBase());

            m_pOntologyClassModel->traverseHierarchy(rootItem,m_pOntologyClassModel->getRootItem());
        }

        m_pOntologyClassView->expandAll();

        //!
        //! in order to set the data to the outport
        //! now setup the QDomDocument
        //!
        setupOutportText();
    }
}

void OntologyCreatorWidget::onModelDataChanged(QModelIndex index, QModelIndex index_){
    Q_UNUSED(index);
    Q_UNUSED(index_);

    setupOutportText();
}

void OntologyCreatorWidget::setupOutportText(){
    OntologyCreatorProcessor* ontologyCreatorProcessor = dynamic_cast<OntologyCreatorProcessor*>(processor_);
    if(!ontologyCreatorProcessor)
        return;

    //! create a new QDomDocument
    m_owlWriter.createDocument();

    //! append the namespaces
    m_owlWriter.setupNamespaces(m_pLineEditXmlns->text(),m_pLabelXmlBase->text());

    //! iterate through the item hierrarch and append the data
    QList<OntologyClassItem*> list = m_pOntologyClassModel->getRootItem()->getChildItems();
    QList<OntologyClassItem*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        OntologyClassItem *citem = *i;
        m_owlWriter.appendData(citem);
    }

    //! get the text
    QString text = m_owlWriter.docToText();

    ontologyCreatorProcessor->setOutportText(text.toStdString());
}

} //namespace voreen

