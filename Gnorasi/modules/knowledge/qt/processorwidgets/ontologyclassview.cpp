#include "ontologyclassview.h"

#include "../models/ontologyclass.h"
//#include "../models/ontologyclassmodel.h"
#include "../utils/ontologyclassificationmanager.h"
#include "../utils/objectlevelmanager.h"

#include "classdescriptiondialog.h"

#include <QtGui/QMenu>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMessageBox>

#include <QStandardItemModel>

#include <QtCore/QDebug>



using namespace voreen;

OntologyClassView::OntologyClassView(QWidget *parent)
    : QTreeView(parent)
{
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
//    setSortingEnabled(true);
}

void OntologyClassView::contextMenuEvent ( QContextMenuEvent * e )
{
    QStandardItemModel *omodel = qobject_cast<QStandardItemModel*>(model());
    if(!omodel){
        QAbstractItemView::contextMenuEvent(e);
        return;
    }

//    if(!omodel->isEditable()){
//        QAbstractItemView::contextMenuEvent(e);
//        return;
//    }

    if(!OBJECTLEVELMANAGER->count())
    {
        QAbstractItemView::contextMenuEvent(e);
        return;
    }

    QMenu *menu = new QMenu(this);
    QModelIndex index = indexAt(e->pos());
    if (index.isValid()){
        menu->addAction(tr("New subclass"),this,SLOT(onAddChildClass()));
        menu->addAction(tr("New sibling class"),this,SLOT(onAddSiblingClass()));
        menu->addAction(tr("Delete class"),this,SLOT(onRemoveCurrentClass()));
    }
    else
        menu->addAction("New subclass",this,SLOT(onAddChildClass()));

    menu->exec(QCursor::pos());

    QAbstractItemView::contextMenuEvent(e);
}

void OntologyClassView::mousePressEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if(!index.isValid()){
        selectionModel()->clear();
    }

    QAbstractItemView::mousePressEvent(event);
}


void OntologyClassView::mouseDoubleClickEvent(QMouseEvent *event){
    QModelIndex index = indexAt(event->pos());
    if(index.isValid()){
        QStandardItemModel *omodel = qobject_cast<QStandardItemModel*>(model());
        if(!omodel)
            return;

        QStandardItem *pItem = omodel->item(index.row());
        if(pItem){
            ClassDescriptionDialog *pClassDescriptionDialog = new ClassDescriptionDialog(this);
            QString classId = pItem->data(Qt::DisplayRole).toString();
            pClassDescriptionDialog->setClassId(classId);
            pClassDescriptionDialog->setupData();

            QModelIndex parentindex = index.parent();
            if(parentindex.isValid()){
                pClassDescriptionDialog->setParentClassId(omodel->data(parentindex).toString());
            }

            if(pClassDescriptionDialog->exec() == QDialog::Rejected){
                QAbstractItemView::mouseDoubleClickEvent(event);
                return;
            }

            QString className = pClassDescriptionDialog->classId();

            pItem->setData(className,Qt::DisplayRole);
            pItem->setData(className);

        }
    }

    QAbstractItemView::mouseDoubleClickEvent(event);
}

void OntologyClassView::onAddChildClass(){
    if(!OBJECTLEVELMANAGER->count())
    {
        return;
    }

    QStandardItemModel *omodel = qobject_cast<QStandardItemModel*>(model());
    if(!omodel)
        return;


    ClassDescriptionDialog *pClassDescriptionDialog = new ClassDescriptionDialog(this);

    QModelIndex index = selectionModel()->currentIndex();
    if(index.isValid()){
        pClassDescriptionDialog->setParentClassId(omodel->data(index).toString());
    }

    if(pClassDescriptionDialog->exec() == QDialog::Rejected)
        return;

    QString classId = pClassDescriptionDialog->classId();

    if(index.isValid()){

        omodel->insertRows(omodel->rowCount(index),1,index);
        QStandardItem *pItem = new QStandardItem();
        pItem->setData(classId,Qt::DisplayRole);
        pItem->setData(classId);

        QList<QStandardItem*> parentItemList = omodel->findItems(pClassDescriptionDialog->parentClassId());
        if(parentItemList.isEmpty())
        {
            QStandardItem *pParentItem = parentItemList.first();
            pParentItem->setChild(pParentItem->rowCount(),pItem);
        }

    }else{
        int childMax = omodel->rowCount();

        omodel->insertRows(childMax,1);
        QStandardItem *pItem = new QStandardItem();
        pItem->setData(classId,Qt::DisplayRole);
        pItem->setData(classId);
        omodel->setItem(omodel->rowCount()-1,pItem);

        setCurrentIndex(omodel->index(childMax,0));
    }
}

void OntologyClassView::onRemoveCurrentClass(){

    QModelIndex index = selectionModel()->currentIndex();
    if(!index.isValid())
        return;

    int ret  = QMessageBox::question(this,tr("Class delete"),tr("You are going to delete this Class, all sub classes and rules related to this class will be also deleted. If you want to delete this Class click Yes else click No."),QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::No)
        return;

//    int ret = QMessageBox::warning(this, tr("Delete Class"),
//                                    tr("Do you want to delete the Class?"),
//                                    QMessageBox::Yes | QMessageBox::No);

    if(ret == QMessageBox::Yes){
        QStandardItemModel *omodel = qobject_cast<QStandardItemModel*>(model());
        if(!omodel)
            return;

        omodel->removeRows(index.row(),1,index.parent());
    }
}

void OntologyClassView::onAddSiblingClass(){
    if(!OBJECTLEVELMANAGER->count())
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if(!index.isValid())
        return;

    QStandardItemModel *omodel = qobject_cast<QStandardItemModel*>(model());
    if(!omodel)
        return;

    ClassDescriptionDialog *pClassDescriptionDialog = new ClassDescriptionDialog(this);

    pClassDescriptionDialog->setParentClassId(omodel->data(index).toString());

    if(pClassDescriptionDialog->exec() == QDialog::Rejected)
        return;

    QString classId = pClassDescriptionDialog->classId();

    if(omodel->insertRows(omodel->rowCount(index.parent()),1,index.parent())){

        QStandardItem *pItem = new QStandardItem();
        pItem->setData(classId,Qt::DisplayRole);
        pItem->setData(classId);

        omodel->setItem(omodel->rowCount()-1,pItem);

        setCurrentIndex(omodel->index(omodel->rowCount(index.parent())-1,0,index.parent()));
    }
}

QString OntologyClassView::getUniqueNameFromIndex(const QModelIndex &index){
    QStandardItemModel *omodel = qobject_cast<QStandardItemModel*>(model());
    if(!omodel)
        return QString();

    QString name = QString::fromUtf8("New Item_%1").arg(QString::number(index.row()));

    QModelIndex parentIndex = omodel->parent(index);

    while(parentIndex.isValid()){

        int indexNo = name.indexOf('_') + 1;

        name.insert(indexNo,QString::number(parentIndex.row()));

        parentIndex = omodel->parent(parentIndex);
    }

    return name;
}

void OntologyClassView::onNameValidationError(){
    QStandardItemModel *omodel = qobject_cast<QStandardItemModel*>(model());
    if(!omodel)
        return;

    QMessageBox::warning(this,tr("Class name error"),
                         tr("The class name must be unique, please type a unique name and try again."));
}
