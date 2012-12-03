#ifndef ONTOLOGYCLASSITEM_H
#define ONTOLOGYCLASSITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

#include "voreen/qt/voreenmoduleqt.h"

namespace voreen {

//!
//! This class stores data related to OntologyClass items handled by the OntologyClassModel
//!
class OntologyClassItem
{
public:
    OntologyClassItem(const QVector<QVariant> &data, OntologyClassItem *parent = 0);
    ~OntologyClassItem();

    QString id() const { return m_id; }
    void setId(QString i) { m_id = i; }

    OntologyClassItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    OntologyClassItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    QList<OntologyClassItem*> getChildItems() const { return childItems; }

private:
    QList<OntologyClassItem*> childItems;
    QVector<QVariant> itemData;
    OntologyClassItem *parentItem;

    QString m_id;
};

}
#endif // ONTOLOGYCLASSITEM_H
