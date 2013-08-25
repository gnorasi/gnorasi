#include "fuzzyrulemodel.h"

#include "../fuzzy/fuzzyrule.h"
#include "../utils/ontologyclassificationmanager.h"
#include "../models/ontologyclass.h"
#include "../utils/ontologyclassificationmanager.h"
#include "../fuzzy/fuzzyrule.h"
#include "../utils/objectattribute.h"
#include "../utils/objectattributemanager.h"
#include "../utils/objectlevelmanager.h"
#include "../utils/objectlevel.h"

#include <QVector>

#define HEADER_VALUE   "HEADER"

using namespace voreen;

FuzzyRuleModel::FuzzyRuleModel(QObject *parent, bool editable) :
    m_editable(editable), QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    rootData << tr("Name");

    rootItem = new FuzzyRule(0,this);
    rootItem->setId(0);
}


FuzzyRuleModel::~FuzzyRuleModel()
{
    delete rootItem;
}

int FuzzyRuleModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}


QVariant FuzzyRuleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    FuzzyRule *item = getItem(index);

    return item->name();
}


Qt::ItemFlags FuzzyRuleModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if(m_editable)
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}



FuzzyRule *FuzzyRuleModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        OntologyClass *item = static_cast<OntologyClass*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}


QVariant FuzzyRuleModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->name();

    return QVariant();
}


QModelIndex FuzzyRuleModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    FuzzyRule *parentItem = getItem(parent);

    FuzzyRule *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


bool FuzzyRuleModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}


bool FuzzyRuleModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    FuzzyRule *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}


QModelIndex FuzzyRuleModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FuzzyRule *childItem = getItem(index);
    FuzzyRule *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


bool FuzzyRuleModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}


bool FuzzyRuleModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    FuzzyRule *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


int FuzzyRuleModel::rowCount(const QModelIndex &parent) const
{
    FuzzyRule *parentItem = getItem(parent);

    return parentItem->childCount();
}


bool FuzzyRuleModel::setData(const QModelIndex &index, const QVariant &value,
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

    FuzzyRule *item = getItem(index);
    bool result = item->setN(index.column(), value);

    if(index.column()==0){
        QString id = value.toString();
        id = id.remove(' ');
        item->setId(id);
    }

    if (result)
        emit dataChanged(index, index);

    return result;
}


bool FuzzyRuleModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}


void FuzzyRuleModel::setupModel()
{
    OntologyClass *pClass = ONTOLOGYCLASSIFICATIONMANAGER->ontologyClassById(m_ontologyClassId);

    if(!pClass)
        return;


    int levelId = pClass->level();
    ObjectLevel *pOL = OBJECTLEVELMANAGER->objectLevelById(levelId);
    if(!pOL)
        return;

    QHash<int,FuzzyRule*> rulesList = pClass->fuzzyRuleHash();
    QHash<int,FuzzyRule*>::const_iterator i;
    for(i = rulesList.constBegin(); i != rulesList.constEnd(); i++){
        int attributeId = i.key();


        ObjectAttribute *pAttribute = OBJECTATTRIBUTEMANAGER->objectAttributeOfLevelById(levelId,attributeId);
        if(!pAttribute)
            continue;

        QString name = pAttribute->name();
    }
}
