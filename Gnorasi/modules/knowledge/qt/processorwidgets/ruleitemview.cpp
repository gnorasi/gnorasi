#include "ruleitemview.h"

#include "../utils/ruleutility.h"

#include "../models/rule.h"
#include "../models/ruleitem.h"

#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QStandardItemModel>

using namespace voreen;

RuleItemView::RuleItemView(QWidget *parent) :
    QTableView(parent)
{

}

//
Rule* RuleItemView::getRule(){
    RuleUtility *ru = RuleUtility::instance();
    return ru->ruleById(m_rule_id);
}

//
void RuleItemView::onAddRuleItem(){

    QStandardItemModel *rmodel = qobject_cast<QStandardItemModel*>(model());
    if(!rmodel){
        return;
    }

    Rule *pRule = getRule();
    if(!pRule)
        return;

    RuleItem *pItem = new RuleItem(QVector<QVariant>(4));
    pRule->appendRuleItem(pItem);

    int row = rmodel->rowCount();

    rmodel->insertRows(model()->rowCount(),1);

    QStandardItem *item0 = new QStandardItem(QString::number(rmodel->rowCount()));
    QStandardItem *item1 = new QStandardItem();
    QStandardItem *item2 = new QStandardItem();
    QStandardItem *item3 = new QStandardItem();

    rmodel->setItem(row,0,item0);
    rmodel->setItem(row,1,item1);
    rmodel->setItem(row,2,item2);
    rmodel->setItem(row,3,item3);

    selectRow(rmodel->rowCount()-1);
}

//
void RuleItemView::onDeleteRuleItem(){
    Rule *pRule = getRule();
    if(!pRule)
        return;

    // checking for validation of the index is not needed
    QModelIndex index = selectionModel()->currentIndex();

    int row = index.row();

    RuleItem *rItem = pRule->childAt(row);
    if(!rItem)
        return;

    pRule->removeRuleItem(rItem);
    model()->removeRows(row,1);
}

//
void RuleItemView::contextMenuEvent(QContextMenuEvent *event){
    QStandardItemModel *rmodel = qobject_cast<QStandardItemModel*>(model());
    if(!rmodel || !getRule()){
        QAbstractItemView::contextMenuEvent(event);
        return;
    }

    QMenu *menu = new QMenu(this);
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()){
        menu->addAction(tr("Delete region"),this,SLOT(onDeleteRuleItem()));
    }
    else
        menu->addAction("Add region",this,SLOT(onAddRuleItem()));

    menu->exec(QCursor::pos());

    QAbstractItemView::contextMenuEvent(event);
}

//
void RuleItemView::mousePressEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        selectionModel()->clearSelection();

    QAbstractItemView::mousePressEvent(event);

}
