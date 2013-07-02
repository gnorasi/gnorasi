#include "classexpressionsview.h"

#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QStandardItemModel>

ClassExpressionsView::ClassExpressionsView(QWidget *parent) :
    QTreeView(parent)
{
}

//
void ClassExpressionsView::onAddExpressionItem(){

}

//
void ClassExpressionsView::onDeleteExpressionItem(){

}


void ClassExpressionsView::onEditExpressionItem(){

}

//
void ClassExpressionsView::contextMenuEvent(QContextMenuEvent *event){
//    QStandardItemModel *rmodel = qobject_cast<QStandardItemModel*>(model());
//    if(!rmodel || !getRule()){
//        QAbstractItemView::contextMenuEvent(event);
//        return;
//    }

//    QMenu *menu = new QMenu(this);
//    QModelIndex index = indexAt(event->pos());
//    if (index.isValid()){
//        menu->addAction(tr("Delete region"),this,SLOT(onDeleteRuleItem()));
//    }
//    else
//        menu->addAction("Add region",this,SLOT(onAddRuleItem()));

//    menu->exec(QCursor::pos());

    QAbstractItemView::contextMenuEvent(event);
}

//
void ClassExpressionsView::mousePressEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        selectionModel()->clearSelection();

    QAbstractItemView::mousePressEvent(event);

}
