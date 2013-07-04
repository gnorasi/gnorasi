#include "insertexpressiondialog.h"

#include "../utils/objectattribute.h"
#include "../utils/objectattributemanager.h"

#include "objectattributesview.h"
#include "membershipfunctiondialog.h"

#include <QVBoxLayout>
#include <QLabel>

InsertExpressionDialog::InsertExpressionDialog(QWidget *parent) :
    QDialog(parent)
{
    m_pSIM = new QStandardItemModel(this);
    QStringList headers;
    headers << tr("Name");

    m_pSIM->setHorizontalHeaderLabels(headers);

    m_pObjectAttributesView = new ObjectAttributesView(this);
    m_pObjectAttributesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pObjectAttributesView->setModel(m_pSIM);

    m_pCancelButton = new QPushButton(tr("Cancel"),this);
    m_pOkButton = new QPushButton(tr("Insert"),this);
    QLabel *pLabel = new QLabel(tr("Available expressions"),this);

    QHBoxLayout *hboxlayout = new QHBoxLayout;

    hboxlayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hboxlayout->addWidget(m_pOkButton);
    hboxlayout->addWidget(m_pCancelButton);

    QVBoxLayout *vboxlayout = new QVBoxLayout;
    vboxlayout->addWidget(pLabel);
    vboxlayout->addWidget(m_pObjectAttributesView);
    vboxlayout->addLayout(hboxlayout);

    setLayout(vboxlayout);

    setWindowTitle(tr("Insert Expression"));

    connect(m_pCancelButton,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(m_pOkButton,SIGNAL(clicked()),this,SLOT(onOkClicked()));

    initialize();
}

void InsertExpressionDialog::onCancelClicked(){

    reject();
}

void InsertExpressionDialog::onOkClicked(){

    accept();
}

void InsertExpressionDialog::setupByLevel(int level){
    Q_ASSERT(m_pObjectMapAttributeRootItem && m_pSpatialAttributeRootItem);

    QMultiHash<int,ObjectAttribute*> hash = OBJECTATTRIBUTEMANAGER->objectAttributeHash();

    QList<ObjectAttribute*> list = hash.values(level);
    QList<ObjectAttribute*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        ObjectAttribute *pAttr = *i;

        QString name = pAttr->name();
        QString id = pAttr->id();

        QStandardItem *pItem = new QStandardItem();
        pItem->setData(name,Qt::DisplayRole);
        pItem->setData(id);

        int ot = pAttr->otype();
        if(ot==1)
            m_pObjectMapAttributeRootItem->setChild(m_pObjectMapAttributeRootItem->rowCount(),pItem);
        else
            m_pSpatialAttributeRootItem->setChild(m_pSpatialAttributeRootItem->rowCount(),pItem);
    }

    m_pObjectAttributesView->setLeveLId(level);
    m_pObjectAttributesView->expand(m_pSIM->indexFromItem(m_pObjectMapAttributeRootItem));
    m_pObjectAttributesView->expand(m_pSIM->indexFromItem(m_pSpatialAttributeRootItem));
}

void InsertExpressionDialog::initialize(){
    m_pObjectMapAttributeRootItem = new QStandardItem();
    m_pObjectMapAttributeRootItem->setData(tr("Object Map Attributes"),Qt::DisplayRole);
    m_pObjectMapAttributeRootItem->setData(-1);

    m_pSpatialAttributeRootItem = new QStandardItem();
    m_pSpatialAttributeRootItem->setData(tr("Spatial Attributes"),Qt::DisplayRole);
    m_pSpatialAttributeRootItem->setData(-2);

    m_pSIM->setItem(0,m_pObjectMapAttributeRootItem);
    m_pSIM->setItem(1,m_pSpatialAttributeRootItem);
}
