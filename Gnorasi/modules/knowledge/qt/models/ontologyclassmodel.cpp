#include <QtGui>

#include "ontologyclassitem.h"
#include "ontologyclassmodel.h"

#include "../utils/owlhelperitem.h"

#include "../utils/ruleutility.h"
#include "../models/rule.h"
#include "../models/ruleitem.h"

#include <QMessageBox>

#define OBJECTDEPICTION_VALUE   "http://www.gnorasi.gr/ontology#ObjectDepiction"


OntologyClassModel::OntologyClassModel(QObject *parent, bool editable)
    : m_editable(editable), QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    rootData << tr("Name") << tr("Comment");

    rootItem = new OntologyClassItem(rootData);
    rootItem->setId(QString::fromAscii(OBJECTDEPICTION_VALUE));
}

OntologyClassModel::~OntologyClassModel()
{
    delete rootItem;
}

int OntologyClassModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}


QVariant OntologyClassModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    OntologyClassItem *item = getItem(index);

    return item->data(index.column());
}


Qt::ItemFlags OntologyClassModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if(m_editable)
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}



OntologyClassItem *OntologyClassModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        OntologyClassItem *item = static_cast<OntologyClassItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}


QVariant OntologyClassModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}


QModelIndex OntologyClassModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    OntologyClassItem *parentItem = getItem(parent);

    OntologyClassItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


bool OntologyClassModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}


bool OntologyClassModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    OntologyClassItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}


QModelIndex OntologyClassModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    OntologyClassItem *childItem = getItem(index);
    OntologyClassItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


bool OntologyClassModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}


bool OntologyClassModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    OntologyClassItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


int OntologyClassModel::rowCount(const QModelIndex &parent) const
{
    OntologyClassItem *parentItem = getItem(parent);

    return parentItem->childCount();
}


bool OntologyClassModel::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
    if (role != Qt::EditRole)
        return false;

    if(!index.column() && data(index,Qt::DisplayRole).toString().compare(value.toString()) && !validateLabel(value) ){
        emit nameValidationError();
        return false;
    }


    //!
    //! Checking funcitonality whether Rules are connected to the Ontontolgy Class Item is being edited
    //
    if(m_editable && !index.row()){
        //!
        //! get the old name
        //!
        QString oldname = data(index.sibling(index.row(),0),Qt::DisplayRole).toString();
        //!
        //! compare it with the new name and check if the old name class has any rules attached
        //! if yes inform the user
        //!
        //! Using a QMessageBox in a core class is not a good practise , it is used though as a fast checking..
        //!
        if(value.toString().compare(oldname) && hasRules(oldname)){
            int ret = QMessageBox::question(0,tr("Edit"),tr("Rules have been found related to this class , click Yes if you want to continue to update the rules attached to this class, or click Cancel to go back."),QMessageBox::Yes,QMessageBox::No);
            if(ret == QMessageBox::No)
                return false;
            else //! the rules attached will be updated with the new name of this class
                updateRules(oldname,value.toString());
        }
    }

    OntologyClassItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if(index.column()==0){
        QString id = value.toString();
        id = id.remove(' ');
        item->setId(id);
    }

    if (result)
        emit dataChanged(index, index);

    return result;
}


bool OntologyClassModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

//! This functions traverses the OntologyClassItem hierarchy and using the OWLHelperItem item
//! creates the respective OntologyClassItem hierrarchey
//! This funciton is called recursive as there is parent child relation of the OWLHelperItem class
void OntologyClassModel::traverseHierarchy(OWLHelperItem *owlItem, OntologyClassItem *ontologyItem){
    QList<OWLHelperItem*> list = owlItem->owlChildren();
    QList<OWLHelperItem*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        OWLHelperItem *child = *i;

        ontologyItem->insertChildren(ontologyItem->childCount(),1,rootItem->columnCount());

        ontologyItem->child(ontologyItem->childCount() - 1)->setId(child->id());
        ontologyItem->child(ontologyItem->childCount() - 1)->setData(0, child->label());
        ontologyItem->child(ontologyItem->childCount() - 1)->setData(1, child->comment());

        traverseHierarchy(child,ontologyItem->child(ontologyItem->childCount()-1));
    }
}

//! getter
QStringList OntologyClassModel::classIdList(OntologyClassItem *item) const {
    QStringList list;
    QList<OntologyClassItem*> ilist = item->getChildItems();
    QList<OntologyClassItem*>::const_iterator i;
    for(i = ilist.constBegin(); i != ilist.constEnd(); i++){
        OntologyClassItem *oitem = *i;
        list.append(oitem->id());

        list.append(classIdList(oitem));
    }

    return list;
}

//! checking functionality
bool OntologyClassModel::hasRules(const QString &name) const{
    RuleUtility *ru = RuleUtility::instance();

    QList<Rule*> list = ru->ruleList();
    QList<Rule*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        Rule *pRule = *i;
        if(!pRule->headClass().compare(name)){
            return true;
        }

        QList<RuleItem*> ilist = pRule->ruleItemList();
        QList<RuleItem*>::const_iterator j;
        for(j = ilist.constBegin(); j != ilist.constEnd(); j++){
            RuleItem *pRuleItem = *j;

            if(!pRuleItem->headClass().compare(name))
                return true;
        }
    }

    return false;
}

//! updating the Rules is achieved using the RuleUtility signleton class
void OntologyClassModel::updateRules(const QString &oldname, const QString &newname){

    RuleUtility *ru = RuleUtility::instance();

    QList<Rule*> list = ru->ruleList();
    QList<Rule*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        Rule *pRule = *i;
        if(!pRule->headClass().compare(oldname)){
            pRule->setHeadClass(newname);
            //! continue cause the head class cannot be contained as a head class of a child RuleItem
            continue;
        }

        QList<RuleItem*> ilist = pRule->ruleItemList();
        QList<RuleItem*>::const_iterator j;
        for(j = ilist.constBegin(); j != ilist.constEnd(); j++){
            RuleItem *pRuleItem = *j;

            if(!pRuleItem->headClass().compare(oldname))
                pRuleItem->setData(1,newname);
        }
    }
}

//! this is a validation label functionality . Al it does is to check for already added OntologyClassItem and
//! return false if there are already functions and true if no
bool OntologyClassModel::validateLabel(const QVariant &val){
    QModelIndex index_ = index(0,0);
    // check for matching values
    QModelIndexList list = match(index_,Qt::DisplayRole,val,1,Qt::MatchRecursive | Qt::MatchExactly | Qt::MatchWrap);

    return list.count() == 1? false : true;
}
