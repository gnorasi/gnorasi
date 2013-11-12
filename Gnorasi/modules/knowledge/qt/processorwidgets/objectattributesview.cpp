#include "objectattributesview.h"

#include <QStandardItemModel>

#include "../processorwidgets/membershipfunctiondialog.h"

ObjectAttributesView::ObjectAttributesView(QWidget *parent) :
    QTreeView(parent)
{
}

//
void ObjectAttributesView::contextMenuEvent(QContextMenuEvent *event){
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
void ObjectAttributesView::mousePressEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        selectionModel()->clearSelection();

    QTreeView::mousePressEvent(event);

}


void ObjectAttributesView::mouseDoubleClickEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if(index.isValid())
    {
        QStandardItemModel *mdl = qobject_cast<QStandardItemModel*>(model());
        if(mdl){
            QStandardItem *item = mdl->itemFromIndex(index);

            if(item->data().toInt() == -1 || item->data().toInt() == -2){
                QAbstractItemView::mouseDoubleClickEvent(event);
                return;
            }
            else{
                QString atid = item->data(Qt::DisplayRole).toString();
                MembershipFunctionDialog *dialog = new MembershipFunctionDialog(m_levelId,atid,this);
                connect(dialog,SIGNAL(fuzzyRuleAdded(int)),this,SIGNAL(fuzzyRuleAdded(int)));
                dialog->exec();
            }
        }
    }

    QTreeView::mouseDoubleClickEvent(event);
}
