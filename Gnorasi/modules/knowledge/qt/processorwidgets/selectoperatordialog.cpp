#include "selectoperatordialog.h"

#include <QHBoxLayout>
#include <QGroupBox>

#include "../fuzzy/fuzzyoperator.h"
#include "../fuzzy/fuzzyoperatormanager.h"

SelectOperatorDialog::SelectOperatorDialog(QWidget *parent) :
    QDialog(parent)
{
    m_pModel = new QStandardItemModel(this);
    m_pTreeView = new QTreeView(this);
    m_pTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTreeView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pOkbutton = new QPushButton(tr("Ok"),this);
    m_pCancelbutton = new QPushButton(tr("Cancel"),this);
    m_pTreeView->setModel(m_pModel);

    QStringList headers;
    headers << tr("Name");
    m_pModel->setHorizontalHeaderLabels(headers);

    QHBoxLayout *hboxlayout = new QHBoxLayout;
    hboxlayout->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Expanding));
    hboxlayout->addWidget(m_pOkbutton);
    hboxlayout->addWidget(m_pCancelbutton);

    QGroupBox *pGroupBox = new QGroupBox(tr("Operators"),this);

    QHBoxLayout *hboxlayout1 = new QHBoxLayout;
    hboxlayout1->addWidget(m_pTreeView);

    pGroupBox->setLayout(hboxlayout1);

    QVBoxLayout *vboxlayout = new QVBoxLayout;
    vboxlayout->addWidget(pGroupBox);
    vboxlayout->addLayout(hboxlayout);
    setLayout(vboxlayout);

    connect(m_pCancelbutton,SIGNAL(clicked()),this,SLOT(onCancelCLicked()));
    connect(m_pOkbutton,SIGNAL(clicked()),this,SLOT(onOkClicked()));

    setupData();
}

void SelectOperatorDialog::onOkClicked(){
    accept();
}


void SelectOperatorDialog::onCancelCLicked(){
    reject();
}

QString SelectOperatorDialog::selectedOperator() const{
    QString name;
    QModelIndex index = m_pTreeView->selectionModel()->currentIndex();
    if(!index.isValid())
        return name;

    QStandardItem *pItem = m_pModel->itemFromIndex(index);
    if(!pItem)
        return name;

    name = pItem->data(Qt::DisplayRole).toString();

    return name;
}

void SelectOperatorDialog::setupData(){
    QList<FuzzyOperator*> list = FUZZYOPERATORMANAGER->fuzzyOperatorList();
    QList<FuzzyOperator*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        FuzzyOperator *pOperator = *i;

        QStandardItem *pItem = new QStandardItem;
        pItem->setData(pOperator->name(),Qt::DisplayRole);
        pItem->setData(pOperator->name());

        m_pModel->setItem(m_pModel->rowCount(),pItem);
    }
}
