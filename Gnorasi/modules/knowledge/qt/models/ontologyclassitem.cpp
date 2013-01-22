#include <QStringList>

#include "ontologyclassitem.h"

using namespace voreen;

OntologyClassItem::OntologyClassItem(const QVector<QVariant> &data, OntologyClassItem *parent)
{
    parentItem = parent;
    itemData = data;
}


OntologyClassItem::~OntologyClassItem()
{
    qDeleteAll(childItems);
}


OntologyClassItem *OntologyClassItem::child(int number)
{
    return childItems.value(number);
}


int OntologyClassItem::childCount() const
{
    return childItems.count();
}


int OntologyClassItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<OntologyClassItem*>(this));

    return 0;
}


int OntologyClassItem::columnCount() const
{
    return itemData.count();
}


QVariant OntologyClassItem::data(int column) const
{
    return itemData.value(column);
}


bool OntologyClassItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        OntologyClassItem *item = new OntologyClassItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}


bool OntologyClassItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (OntologyClassItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}


OntologyClassItem *OntologyClassItem::parent()
{
    return parentItem;
}


bool OntologyClassItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}


bool OntologyClassItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (OntologyClassItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}


bool OntologyClassItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

