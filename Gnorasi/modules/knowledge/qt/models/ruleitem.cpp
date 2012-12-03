#include <QStringList>

#include <QDebug>

#include "ruleitem.h"


RuleItem::RuleItem(const QVector<QVariant> &data)
{
    itemData = data;
}


RuleItem::~RuleItem()
{

}

int RuleItem::columnCount() const
{
    return itemData.count();
}


QVariant RuleItem::data(int column) const
{
    return itemData.value(column);
}


bool RuleItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

