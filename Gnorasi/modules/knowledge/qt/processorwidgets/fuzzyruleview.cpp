#include "fuzzyruleview.h"

#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QStandardItemModel>

#include "insertexpressiondialog.h"
#include "membershipfunctiondialog.h"
#include "selectoperatordialog.h"

#include "../fuzzy/fuzzyrule.h"
#include "../fuzzy/fuzzyrulemanager.h"
#include "../utils/ontologyclassificationmanager.h"
#include "../models/ontologyclass.h"

#include "../fuzzy/fuzzyoperator.h"
#include "../fuzzy/fuzzyoperatormanager.h"

#include "../utils/objectattribute.h"
#include "../utils/objectattributemanager.h"

FuzzyRuleView::FuzzyRuleView(QWidget *parent) :
    QTreeView(parent)
{
    m_pInsertExpressionDialog = new InsertExpressionDialog(this);
    connect(m_pInsertExpressionDialog,SIGNAL(fuzzyRuleAdded(int)),this,SIGNAL(fuzzyRuleAdded(int)));
}

void FuzzyRuleView::onChangeOperator(){
    SelectOperatorDialog *pDialog = new SelectOperatorDialog(this);
    if(pDialog->exec() == QDialog::Accepted){
        QString operatorName = pDialog->selectedOperator();

        FuzzyOperator *pOpr = FUZZYOPERATORMANAGER->fuzzyOperatorByName(operatorName);
        if(!pOpr)
            return;

        m_pOperatorItem->setData(operatorName,Qt::DisplayRole);
    }
}

//
void FuzzyRuleView::onAddExpressionItem(){

}

//
void FuzzyRuleView::onDeleteFuzzyRule(){
    QModelIndex index = selectionModel()->currentIndex();
    if(!index.isValid())
        return;

    int ret  = QMessageBox::question(this,tr("Delete Fuzzy Rule "),tr("You are going to delete this Fuzzy Rule, If you want to delete this Fuzzy rule click Yes else click No."),QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::No)
        return;

    if(ret == QMessageBox::Yes){
        QStandardItemModel *omodel = qobject_cast<QStandardItemModel*>(model());
        if(!omodel)
            return;

        QStandardItem *pItem = omodel->itemFromIndex(index);

        int id = pItem->data().toInt();
        FuzzyRule *pRule = FUZZYRULEMANAGER->fuzzyRuleById(id);
        if(!pRule)
            return;

        OntologyClass *pClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(m_classId);
        if(!pClass)
            return;

        pClass->removeFuzzyRule(m_levelId,pRule);

        FUZZYRULEMANAGER->removeFuzzyRule(pRule);

        m_pOperatorItem->removeRow(pItem->row());
    }
}


void FuzzyRuleView::onEditExpressionItem(){

}

//
void FuzzyRuleView::contextMenuEvent(QContextMenuEvent *event){
    Q_ASSERT(m_pOperatorItem);

    QStandardItemModel *omodel = qobject_cast<QStandardItemModel*>(model());
    if(!omodel){
        QTreeView::contextMenuEvent(event);
        return;
    }

    QModelIndex index = indexAt(event->pos());
    if(!index.isValid())
    {
        QTreeView::contextMenuEvent(event);
        return;
    }

    QStandardItem *pItem = omodel->itemFromIndex(index);
    int iid = pItem->data().toInt();
    if( iid == -101 || iid == -102){
        QTreeView::contextMenuEvent(event);
        return;
    }

    QMenu *menu = new QMenu(this);

    int helperId = m_pOperatorItem->data().toInt();

    if(helperId == iid){
        menu->addAction(tr("Change operator"),this,SLOT(onChangeOperator()));
    }else{
        menu->addAction(tr("Delete Fuzzy Rule"),this,SLOT(onDeleteFuzzyRule()));
    }

    menu->exec(QCursor::pos());

    QTreeView::contextMenuEvent(event);
}

//
void FuzzyRuleView::mousePressEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        selectionModel()->clearSelection();

    QTreeView::mousePressEvent(event);

}


void FuzzyRuleView::mouseDoubleClickEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if(!index.isValid())
    {
        QTreeView::mouseDoubleClickEvent(event);
        return;
    }

    QStandardItemModel *mdl = qobject_cast<QStandardItemModel*>(model());
    if(mdl){
        QStandardItem *item = mdl->itemFromIndex(index);
        if(!item){
            QTreeView::mouseDoubleClickEvent(event);
            return;
        }

        int iid = item->data().toInt();

        if( iid == -101 || iid == -102){
            QTreeView::mouseDoubleClickEvent(event);
            return;
        }

        if(iid == -103)
        {
            m_pInsertExpressionDialog->setupByLevel(m_levelId);
            m_pInsertExpressionDialog->exec();

        }else if(iid > 0){

            FuzzyRule *pRule = FUZZYRULEMANAGER->fuzzyRuleById(iid);
            if(pRule){
                QString atid = pRule->attribute();
                MembershipFunctionDialog *pMembershipFunctionDialog = new MembershipFunctionDialog(m_levelId,atid,this);
                pMembershipFunctionDialog->setupByRuleId(iid);
                pMembershipFunctionDialog->exec();
            }
        }
    }

    QTreeView::mouseDoubleClickEvent(event);
}
