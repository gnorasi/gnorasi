#include "fuzzyruleview.h"

#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QStandardItemModel>

#include "insertexpressiondialog.h"
#include "membershipfunctiondialog.h"
#include "../fuzzy/fuzzyrule.h"
#include "../fuzzy/fuzzyrulemanager.h"

#include "../utils/objectattribute.h"
#include "../utils/objectattributemanager.h"

FuzzyRuleView::FuzzyRuleView(QWidget *parent) :
    QTreeView(parent)
{
    m_pInsertExpressionDialog = new InsertExpressionDialog(this);
}

//
void FuzzyRuleView::onAddExpressionItem(){

}

//
void FuzzyRuleView::onDeleteExpressionItem(){

}


void FuzzyRuleView::onEditExpressionItem(){

}

//
void FuzzyRuleView::contextMenuEvent(QContextMenuEvent *event){
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
void FuzzyRuleView::mousePressEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        selectionModel()->clearSelection();

    QAbstractItemView::mousePressEvent(event);

}


void FuzzyRuleView::mouseDoubleClickEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if(!index.isValid())
    {
        QAbstractItemView::mouseDoubleClickEvent(event);
        return;
    }

    QStandardItemModel *mdl = qobject_cast<QStandardItemModel*>(model());
    if(mdl){
        QStandardItem *item = mdl->itemFromIndex(index);
        if(!item){
            QAbstractItemView::mouseDoubleClickEvent(event);
            return;
        }

        int iid = item->data().toInt();

        if( iid == -101 || iid == -102){
            QAbstractItemView::mouseDoubleClickEvent(event);
            return;
        }

        if(iid == -103)
        {
            m_pInsertExpressionDialog->setupByLevel(m_levelId);
            m_pInsertExpressionDialog->exec();

        }else if(iid > 0){

//            MembershipFunctionDialog *pMembershipFunctionDialog = new MembershipFunctionDialog();
//            pMembershipFunctionDialog->setLevelId(m_levelId);
//            pMembershipFunctionDialog->setupByRuleId(iid);
//            pMembershipFunctionDialog->exec();
        }
    }

    QAbstractItemView::mouseDoubleClickEvent(event);
}
