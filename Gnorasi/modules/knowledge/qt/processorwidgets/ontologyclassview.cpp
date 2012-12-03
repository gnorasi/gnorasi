#include "ontologyclassview.h"

#include "../models/ontologyclassitem.h"
#include "../models/ontologyclassmodel.h"

#include <QtGui/QMenu>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMessageBox>

#include <QtCore/QDebug>


OntologyClassView::OntologyClassView(QWidget *parent)
    : QTreeView(parent)
{
    setAlternatingRowColors(true);
//    setSortingEnabled(true);
}

void OntologyClassView::contextMenuEvent ( QContextMenuEvent * e )
{
    OntologyClassModel *omodel = qobject_cast<OntologyClassModel*>(model());
    if(!omodel){
        QAbstractItemView::contextMenuEvent(e);
        return;
    }

    if(!omodel->isEditable()){
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

void OntologyClassView::onAddChildClass(){
    OntologyClassModel *omodel = qobject_cast<OntologyClassModel*>(model());
    if(!omodel)
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if(index.isValid()){

        if(omodel->insertRows(omodel->rowCount(index),1,index)){

            QString name = getUniqueNameFromIndex(omodel->index(omodel->rowCount(index)-1,0,index));

            if(!omodel->validateLabel(name)){
                int counter = 1;
                name = tr("Custom Item %1").arg(QString::number(counter));
                while(!omodel->validateLabel(name)){
                    name = tr("Custom Item %1").arg(QString::number(++counter));
                }
            }

            omodel->setData(omodel->index(omodel->rowCount(index)-1,0,index),name);

            expand(index);

            setCurrentIndex(omodel->index(omodel->rowCount(index)-1,0,index));
        }
    }else{
        int childMax = omodel->getRootItem()->childCount();

        if(omodel->insertRows(childMax,1)){

            QString name = tr("New Item_%1").arg(QString::number(childMax));

            if(!omodel->validateLabel(name)){
                int counter = 1;
                name = tr("Custom Item %1").arg(QString::number(counter));
                while(!omodel->validateLabel(name)){
                    name = tr("Custom Item %1").arg(QString::number(++counter));
                }
            }

            omodel->setData(omodel->index(childMax,0),name);

            setCurrentIndex(omodel->index(childMax,0));
        }
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
        OntologyClassModel *omodel = qobject_cast<OntologyClassModel*>(model());
        if(!omodel)
            return;

        omodel->removeRows(index.row(),1,index.parent());
    }
}

void OntologyClassView::onAddSiblingClass(){
    QModelIndex index = selectionModel()->currentIndex();
    if(!index.isValid())
        return;

    OntologyClassModel *omodel = qobject_cast<OntologyClassModel*>(model());
    if(!omodel)
        return;

    if(omodel->insertRows(omodel->rowCount(index.parent()),1,index.parent())){

        QString name = getUniqueNameFromIndex(omodel->index(omodel->rowCount(index.parent())-1,0,index.parent()));

        if(!omodel->validateLabel(name)){
            int counter = 1;
            name = tr("Custom Item %1").arg(QString::number(counter));
            while(!omodel->validateLabel(name))
                counter++;
        }

        omodel->setData(omodel->index(omodel->rowCount(index.parent())-1,0,index.parent()),name);

        setCurrentIndex(omodel->index(omodel->rowCount(index.parent())-1,0,index.parent()));
    }
}

QString OntologyClassView::getUniqueNameFromIndex(const QModelIndex &index){
    OntologyClassModel *omodel = qobject_cast<OntologyClassModel*>(model());
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
    OntologyClassModel *omodel = qobject_cast<OntologyClassModel*>(model());
    if(!omodel)
        return;

    QMessageBox::warning(this,tr("Class name error"),
                         tr("The class name must be unique, please type a unique name and try again."));
}
