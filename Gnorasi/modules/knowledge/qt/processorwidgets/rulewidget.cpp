#include "rulewidget.h"

#include "ruleitemview.h"
#include "comboboxdelegate.h"

#include "../utils/ruleutility.h"
#include "../models/ontologyclassitem.h"
#include "../models/rule.h"
#include "../models/ruleitem.h"


#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QListView>


RuleWidget::RuleWidget(const QString &ocid, QWidget *parent) :
    m_ontologyClassId(ocid), QWidget(parent)
{
    initialize();
    setWindowFlags(Qt::Window);
    setWindowTitle(tr("Rules Widget"));
}

//
void RuleWidget::initialize(){

    QLabel *labelRulesLabel = new QLabel(tr("Available Rules "),this);

    m_pRulesModel = new QStandardItemModel(this);
    QStringList headers0;
    headers0 << tr("Rule name");
    m_pRulesModel->setHorizontalHeaderLabels(headers0);
    m_pListViewRules = new QListView(this);
    m_pListViewRules->setAlternatingRowColors(true);
    m_pListViewRules->setModel(m_pRulesModel);
    m_pListViewRules->setMinimumHeight(75);
    m_pListViewRules->setMaximumHeight(150);
    m_pListViewRules->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QLabel *labelActions = new QLabel(tr("Actions"),this);
    m_pPushButtonNewRule = new QPushButton(QIcon(":/voreenve/icons/add1.png"),tr("New"),this);
    m_pPushButtonNewRule->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_pPushButtonDeleteRule = new QPushButton(QIcon(":/voreenve/icons/delete.png"),tr("Delete"),this);
    m_pPushButtonDeleteRule->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout *vBoxLayout_0 = new QVBoxLayout();
    vBoxLayout_0->addWidget(labelActions);
    vBoxLayout_0->addWidget(m_pPushButtonNewRule);
    vBoxLayout_0->addWidget(m_pPushButtonDeleteRule);
    vBoxLayout_0->addSpacerItem(new QSpacerItem(100,10,QSizePolicy::Preferred,QSizePolicy::Expanding));

    QHBoxLayout *hBoxLayout0 = new QHBoxLayout();
    hBoxLayout0->addWidget(m_pListViewRules);
    hBoxLayout0->addLayout(vBoxLayout_0);
    hBoxLayout0->addSpacerItem(new QSpacerItem(50,20,QSizePolicy::Expanding,QSizePolicy::Fixed));

    QFrame *hline = new QFrame(this);
    hline->setFrameShape(QFrame::HLine);

    QLabel *pLabelProperties = new QLabel(tr("<strong>Rule properties</strong>"),this);

    QLabel *lbl1 = new QLabel(tr("Class : "),this);
    m_pLabelClassName = new QLabel(tr("Not set"),this);

    QHBoxLayout *hBoxLayout1 = new QHBoxLayout();
    hBoxLayout1->addWidget(lbl1);
    hBoxLayout1->addWidget(m_pLabelClassName);
    hBoxLayout1->addSpacerItem(new QSpacerItem(50,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    QLabel *lbl2 = new QLabel(tr("Confidence : "),this);
    m_pSpinBoxConfidence = new QDoubleSpinBox(this);
    m_pSpinBoxConfidence->setMinimum(0);
    m_pSpinBoxConfidence->setMaximum(1);
    m_pSpinBoxConfidence->setSingleStep(0.01);

    QHBoxLayout *hBoxLayout2 = new QHBoxLayout();
    hBoxLayout2->addWidget(lbl2);
    hBoxLayout2->addWidget(m_pSpinBoxConfidence);
    hBoxLayout2->addSpacerItem(new QSpacerItem(50,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    m_pRuleItemModel = new QStandardItemModel(this);
    QStringList m_headers;
    m_headers << tr("id") << tr("class") << tr("property") << tr("Segmentation level");
    m_pRuleItemModel->setHorizontalHeaderLabels(m_headers);

    m_pRuleItemView = new RuleItemView(this);
    m_pRuleItemView->setAlternatingRowColors(true);
    m_pRuleItemView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pRuleItemView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pRuleItemView->setModel(m_pRuleItemModel);
    m_pRuleItemView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->addWidget(labelRulesLabel);
    vBoxLayout->addLayout(hBoxLayout0);
    vBoxLayout->addWidget(hline);
    vBoxLayout->addWidget(pLabelProperties);
    vBoxLayout->addLayout(hBoxLayout1);
    vBoxLayout->addLayout(hBoxLayout2);
    vBoxLayout->addWidget(m_pRuleItemView,1);

    connect(m_pPushButtonDeleteRule,SIGNAL(clicked()),this,SLOT(onDeleteRuleClicked()));
    connect(m_pPushButtonNewRule,SIGNAL(clicked()),this,SLOT(onAddNewRuleClicked()));
    connect(m_pRuleItemModel,SIGNAL(itemChanged(QStandardItem*)),SLOT(onRuleItemJChanged(QStandardItem*)));
    connect(m_pListViewRules->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(onRulesListViewSelectionChanged(QItemSelection,QItemSelection)));
    connect(m_pSpinBoxConfidence,SIGNAL(valueChanged(double)),this,SLOT(onSpinBoxJChanged(double)));
    connect(m_pRulesModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onRulesListViewChanged(QStandardItem*)));

    setLayout(vBoxLayout);
}

//!
void RuleWidget::setupByOntologyClassItem(OntologyClassItem *oitem, const QStringList &list){
    Q_ASSERT(oitem);

    m_ontologyClassId = oitem->id();

    ComboBoxDelegate *pComboBoxDelegate = new ComboBoxDelegate(list,this);
    m_pRuleItemView->setItemDelegateForColumn(1,pComboBoxDelegate);

    m_pLabelClassName->setText(m_ontologyClassId);

    m_pRulesModel->removeRows(0,m_pRulesModel->rowCount());
    m_pListViewRules->clearSelection();
    m_pSpinBoxConfidence->clear();

    // get the instance
    RuleUtility *m_pRuleUtility = RuleUtility::instance();

    // get the lis
    QList<Rule*> rlist = m_pRuleUtility->ruleListByHeadClass(oitem->id());
    // iterate through the list
    QList<Rule*>::const_iterator i;
    for(i = rlist.constBegin(); i != rlist.constEnd(); i++){
        Rule *pRule = *i;

        // add a new entry
        QStandardItem *ritem = new QStandardItem();
        ritem->setData(pRule->id(),Qt::DisplayRole);

        // store the pointer as a user data
        QVariant v = qVariantFromValue((void *) pRule);
        ritem->setData(v);

        // append the row
        m_pRulesModel->appendRow(ritem);
    }

    m_pRuleItemModel->removeRows(0,m_pRuleItemModel->rowCount());
}

//!
//! This slot is called when the rules listview selection model is changed..
//!
void RuleWidget::onRulesListViewSelectionChanged(const QItemSelection &selected,const QItemSelection &deselected){

    //! check if it is empty
    if(selected.isEmpty())
        return;

    //! get the fist index
    QModelIndex index = selected.indexes().first();

    //! checks if the current index is valid
    if(!index.isValid())
        return;

    RuleUtility *m_pRuleUtility = RuleUtility::instance();

    QString rid = m_pRulesModel->data(index,Qt::DisplayRole).toString();

    Rule *pRule = m_pRuleUtility->ruleById(rid);
    if(!pRule){
        QMessageBox::warning(this,tr("Warning"),tr("NO valid rule found"));
        return ;
    }

    m_pSpinBoxConfidence->setValue(pRule->confidence());

    m_pRuleItemModel->removeRows(0,m_pRuleItemModel->rowCount());

    setupRuleItemViewByRule(pRule);
}

//!
void RuleWidget::onSpinBoxJChanged(double val){
    RuleUtility *m_pRuleUtility = RuleUtility::instance();

    QString rid = m_pRulesModel->data(m_pListViewRules->selectionModel()->currentIndex(),Qt::DisplayRole).toString();

    Rule *pRule = m_pRuleUtility->ruleById(rid);
    if(!pRule){
        return ;
    }

    pRule->setConfidence(val);
}

//!
void RuleWidget::onAddNewRuleClicked(){
    Q_ASSERT(!m_ontologyClassId.isEmpty());

    m_pRuleItemModel->removeRows(0,m_pRuleItemModel->rowCount());

    RuleUtility *m_pRuleUtility = RuleUtility::instance();

    Rule *pRule = m_pRuleUtility->createRule(m_ontologyClassId);
    m_pRuleItemView->setRuleId(pRule->id());

    QStandardItem *item = new QStandardItem();
    item->setData(pRule->id(),Qt::DisplayRole);

    // store the pointer as a user data
    QVariant v = qVariantFromValue((void *) pRule);
    item->setData(v);

    m_pRulesModel->appendRow(item);
    m_pListViewRules->setCurrentIndex(m_pRulesModel->index(m_pRulesModel->rowCount()-1,0));

    emit dataChanged();
}

//!
void RuleWidget::onDeleteRuleClicked(){
    Q_ASSERT(!m_ontologyClassId.isEmpty());

    int ret  = QMessageBox::question(this,tr("Rule delete"),tr("You are going to delete this Rule, all regions will be deleted. If you want to delete this rule click Yes else click No."),QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::No)
        return;

    QString rid = m_pRulesModel->data(m_pListViewRules->currentIndex(),Qt::DisplayRole).toString();

    RuleUtility *m_pRuleUtility = RuleUtility::instance();
    Rule *pRule = m_pRuleUtility->ruleById(rid);
    if(!pRule){
        QMessageBox::warning(this,tr("Warning"),tr("No valid rule found"));
        return ;
    }

    m_pRuleUtility->removeRule(pRule);
    m_pRuleItemModel->removeRows(0,m_pRuleItemModel->rowCount());
    m_pRulesModel->removeRow(m_pListViewRules->selectionModel()->currentIndex().row());

    // set default value
    m_pSpinBoxConfidence->setValue(0);
    m_pLabelClassName->setText(tr("NOT SET"));

    // now get the next selected item and setup by this
    rid = m_pRulesModel->data(m_pListViewRules->selectionModel()->currentIndex(),Qt::DisplayRole).toInt();
    pRule = m_pRuleUtility->ruleById(rid);
    if(!pRule){
        return ;
    }

    // setup by the rule item
    setupRuleItemViewByRule(pRule);


    // emit the singal
    emit dataChanged();
}

//!
void RuleWidget::setupRuleItemViewByRule(Rule *pRule){
    Q_ASSERT(pRule);

    m_pRuleItemView->setRuleId(pRule->id());

    QList<RuleItem*> list = pRule->ruleItemList();
    QList<RuleItem*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd() ; i++){
        RuleItem *rItem = *i;

        QStandardItem *item0 = new QStandardItem();
        QStandardItem *item1 = new QStandardItem();
        QStandardItem *item2 = new QStandardItem();
        QStandardItem *item3 = new QStandardItem();

        item0->setData(rItem->data(0),Qt::DisplayRole);
        item1->setData(rItem->data(1),Qt::DisplayRole);
        item2->setData(rItem->data(2),Qt::DisplayRole);
        item3->setData(rItem->data(3),Qt::DisplayRole);

        QList<QStandardItem*> l;
        l << item0 << item1 << item2 << item3 ;

        m_pRuleItemModel->appendRow(l);
    }
}

//!
void RuleWidget::onRulesListViewChanged(QStandardItem *item){
    QVariant val = item->data();

    Rule *pRule = (Rule *) val.value<void *>();
    if(!pRule)
        return;

    pRule->setid(item->data(Qt::DisplayRole).toString());

    m_pRuleItemView->setRuleId(pRule->id());
}

//!
void RuleWidget::onRuleItemJChanged(QStandardItem *item){
    RuleUtility *m_pRuleUtility = RuleUtility::instance();

    QString rid = m_pRulesModel->data(m_pListViewRules->currentIndex(),Qt::DisplayRole).toString();

    Rule *pRule = m_pRuleUtility->ruleById(rid);
    if(!pRule){
        QMessageBox::warning(this,tr("Warning"),tr("NO valid rule found"));
        return ;
    }

    RuleItem *rItem = pRule->childAt(item->row());
    if(!rItem)
        return;

    rItem->setData(item->column(),item->data(Qt::DisplayRole));

    emit dataChanged();
}

//! Update the relative QItemDelegate
void RuleWidget::setFeaturePropertyList(const QStringList &list){
    ComboBoxDelegate* pFeaturePropertyDelegate = new ComboBoxDelegate(list,this);
    m_pRuleItemView->setItemDelegateForColumn(2,pFeaturePropertyDelegate);
}

//! Update the relative QItemDelegate
void RuleWidget::setLevelList(const QStringList &list){
    ComboBoxDelegate *pLevelComboBoxDelegate = new ComboBoxDelegate(list,this);
    m_pRuleItemView->setItemDelegateForColumn(3,pLevelComboBoxDelegate);
}

//! Update the relative QItemDelegate
void RuleWidget::setOntologyClassList(const QStringList &list){
    ComboBoxDelegate *pComboBoxDelegate = new ComboBoxDelegate(list,this);
    m_pRuleItemView->setItemDelegateForColumn(1,pComboBoxDelegate);
}

RuleWidget::~RuleWidget(){
    RuleUtility::deleteInstance();
}
