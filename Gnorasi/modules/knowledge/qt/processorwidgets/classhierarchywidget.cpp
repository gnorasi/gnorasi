#include "classhierarchywidget.h"

#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

#include "../models/ontologyclassitem.h"
#include "../models/ontologyclass.h"
#include "../utils/ontologyclassificationmanager.h"
//#include "../models/ontologyclassmodel.h"

#include "../processorwidgets/ontologyclassview.h"

#include <QDebug>


using namespace voreen;

ClassHierarchyWidget::ClassHierarchyWidget(QWidget *parent) :
    QWidget(parent),
    m_pOntologyClassModel(0),
    m_pOntologyClassView(0)
{
    initialize();
}

void ClassHierarchyWidget::initialize(){
//    m_pOntologyClassModel = new OntologyClassModel(this);
    m_pOntologyClassModel = new QStandardItemModel(this);
    QStringList headers;
    headers << tr("Name");
    m_pOntologyClassModel->setHorizontalHeaderLabels(headers);

    m_pOntologyClassView = new OntologyClassView(this);
    m_pOntologyClassView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pOntologyClassView->setModel(m_pOntologyClassModel);

    QLabel *pLabelActions = new QLabel(tr("Actions"),this);

    QPushButton *addSubClassButton      = new QPushButton(QIcon(":/voreenve/icons/add1.png"),tr("New subclass"),this);
    QPushButton *addSiblingClassButton  = new QPushButton(QIcon(":/voreenve/icons/add2.png"),tr("New sibling class"),this);
    QPushButton *deleteClassButton      = new QPushButton(QIcon(":/voreenve/icons/delete.png"),tr("Delete class"),this);

    QLabel *pLabelHeader = new QLabel(tr("Ontology Classes"),this);

    addSubClassButton->setToolTip(tr("Add a new <strong>sub class</strong> to the class selected."));
    addSiblingClassButton->setToolTip(tr("Add a new sibling class to the class selected."));
    deleteClassButton->setToolTip(tr("Delete the class selected."));

    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(addSubClassButton);
    hBoxLayout->addWidget(addSiblingClassButton);
    hBoxLayout->addWidget(deleteClassButton);
    hBoxLayout->addSpacerItem(new QSpacerItem(100,20,QSizePolicy::Expanding,QSizePolicy::Fixed));

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
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
}


void ClassHierarchyWidget::setupModel(){

    m_pOntologyClassModel->removeRows(0,m_pOntologyClassModel->rowCount());

    QList<OntologyClass*> list = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassList();
    QList<OntologyClass*>::const_iterator i;
    for(i = list.constBegin(); i  != list.constEnd(); i++){

        OntologyClass *pClass  = *i;

        if(!pClass->parent()){

            qDebug() << "processing class : " << pClass->id() << " with no parent class : " ;

            processOntology(pClass);
        }
    }
}

void ClassHierarchyWidget::processOntology(OntologyClass *pClass, QStandardItem *pParentItem ){

    QStandardItem *pItem = new QStandardItem();
    pItem->setData(pClass->name(),Qt::DisplayRole);
    pItem->setData(pClass->id());

    if(pParentItem){

        pParentItem->setChild(pParentItem->rowCount(),pItem);
    }
    else{
        m_pOntologyClassModel->setItem(m_pOntologyClassModel->rowCount(),pItem);
    }

    QList<OntologyClass*> childlist = pClass->getChildItems();
    QList<OntologyClass*>::const_iterator i;
    for(i = childlist.constBegin(); i != childlist.constEnd(); i++){

        OntologyClass *pChildClass = *i;
        processOntology(pChildClass,pItem);
    }
}

void ClassHierarchyWidget::onModelDataChanged(QModelIndex index, QModelIndex index_){
    Q_UNUSED(index);
    Q_UNUSED(index_);

//    setupOutportText();
}
