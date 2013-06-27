#include <QStringList>

#include "ontologyclass.h"

using namespace voreen;

#include "../fuzzy/fuzzyrule.h"

OntologyClass::OntologyClass(const QVector<QVariant> &data, OntologyClass *parent)
    : QObject(parent)
{
    parentItem = parent;
    itemData = data;
}


OntologyClass::~OntologyClass()
{
    qDeleteAll(childItems);
}


OntologyClass *OntologyClass::child(int number)
{
    return childItems.value(number);
}


int OntologyClass::childCount() const
{
    return childItems.count();
}


int OntologyClass::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<OntologyClass*>(this));

    return 0;
}


int OntologyClass::columnCount() const
{
    return itemData.count();
}


QVariant OntologyClass::data(int column) const
{
    return itemData.value(column);
}


bool OntologyClass::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        OntologyClass *item = new OntologyClass(data, this);
        childItems.insert(position, item);
    }

    return true;
}


bool OntologyClass::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (OntologyClass *child, childItems)
        child->insertColumns(position, columns);

    return true;
}


OntologyClass *OntologyClass::parent()
{
    return parentItem;
}


bool OntologyClass::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}


bool OntologyClass::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (OntologyClass *child, childItems)
        child->removeColumns(position, columns);

    return true;
}


bool OntologyClass::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

