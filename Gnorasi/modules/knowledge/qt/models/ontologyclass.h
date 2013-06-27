/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#ifndef ONTOLOGYCLASS_H
#define ONTOLOGYCLASS_H

#include <QList>
#include <QVariant>
#include <QVector>


namespace voreen {

class FuzzyRule;
//!
//! \brief This class stores data related to OntologyClass items handled by the OntologyClassModel
//!
class OntologyClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  id      READ id         WRITE setId         NOTIFY idChanged)
    Q_PROPERTY(QString  name    READ name       WRITE setName       NOTIFY nameChanged)
    Q_PROPERTY(QString  color   READ color      WRITE setColor      NOTIFY colorChanged)
    Q_PROPERTY(QString  level   READ level      WRITE setLevel      NOTIFY levelChanged)
public:
    OntologyClass(const QVector<QVariant> &data, OntologyClass *parent = 0);
    ~OntologyClass();

    QString id() const { return m_id; }
    void setId(QString i) { m_id = i; }

    OntologyClass *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    OntologyClass *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    QList<OntologyClass*> getChildItems() const { return childItems; }


    QHash<int,FuzzyRule*> fuzzyRuleHash() const { return m_fuzzyRuleHash; }
    void setFuzzyRuleHash(const QHash<int,FuzzyRule*> &f) { m_fuzzyRuleHash = f; }


    QString name() const { if(itemData.count() > 1) return itemData[2].toString(); return QString(); }
    void setName(const QString &n) { itemData[1] = n; }


    QString color() const { if(itemData.count() > 2) return itemData[2].toString();  return QString(); }
    void setColor(const QString &n) { itemData[2] = n; }

    QString level() const { return m_level; }
    void setLevel(const QString &l) { m_level = l; }


signals:
    void idChanged();
    void nameChanged();
    void colorChanged();
    void levelChanged();

private:
    QList<OntologyClass*> childItems;
    QVector<QVariant> itemData;
    OntologyClass *parentItem;

    QString m_level;

    QHash<int,FuzzyRule*> m_fuzzyRuleHash;

    QString m_id;
};

} // end of namespace voreen

#endif // ONTOLOGYCLASSITEM_H
