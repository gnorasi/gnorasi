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

#include <QDomDocument>

class FuzzyOperator;

class FuzzyRule;

//namespace voreen {
//!
//! \brief This class stores data related to OntologyClass items handled by the OntologyClassModel
//!
class OntologyClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  id      READ id         WRITE setId         NOTIFY idChanged)
    Q_PROPERTY(QString  name    READ name       WRITE setName       NOTIFY nameChanged)
    Q_PROPERTY(QString  color   READ color      WRITE setColor      NOTIFY colorChanged)
    Q_PROPERTY(int      level   READ level      WRITE setLevel      NOTIFY levelChanged)
    Q_PROPERTY(QString  parentId READ parentId WRITE setparentId NOTIFY parentIdChanged)
    Q_PROPERTY(QString opername READ opername WRITE setopername NOTIFY opernameChanged)
public:
    OntologyClass(OntologyClass *parent = 0);
    ~OntologyClass();

    QString id() const { return m_id; }
    void setId(QString i) { m_id = i; }

    OntologyClass *child(int number);
    int childCount() const;

    OntologyClass *parent();

    int childNumber() const;

    QList<OntologyClass*> getChildItems() const { return childItems; }

    void addChild(OntologyClass *o) { childItems.append(o); }
    void removeChild(OntologyClass *o) { childItems.removeOne(o); }

    QHash<int,FuzzyRule*> fuzzyRuleHash() const { return m_fuzzyRuleHash; }
    void setFuzzyRuleHash(const QHash<int,FuzzyRule*> &f) { m_fuzzyRuleHash = f; }
    void clearFuzzyRuleHash() { m_fuzzyRuleHash.clear(); }
    void addFuzzyRule(int i, FuzzyRule *r) { m_fuzzyRuleHash.insertMulti(i,r); }
    QList<FuzzyRule*> fuzzyRuleList(int i) const { return m_fuzzyRuleHash.values(i); }
    void removeFuzzyRule(int i, FuzzyRule *r);


    QString name() const { return m_name; }
    void setName(const QString &n) { m_name = n; }


    QString color() const { return m_color; }
    void setColor(const QString &n) { m_color = n; }

    int level() const { return m_level; }
    void setLevel(const int l) { m_level = l; }

    QString parentId() const  { return m_parentId; }
    void setparentId(const QString &i) { m_parentId = i; }


    QString opername() const {return m_opername; }
    void setopername(const QString &n) { m_opername = n; }


    void parseChildren(const QDomDocument& );


signals:
    void idChanged();
    void nameChanged();
    void colorChanged();
    void levelChanged();
    void parentIdChanged();
    void opernameChanged();

private:

    OntologyClass *parentItem;

    QList<OntologyClass*> childItems;

    QHash<int,FuzzyRule*> m_fuzzyRuleHash;

    bool checkSiblings(const QString &);

    void parseSiblings(const QDomDocument &doc);

    QString m_name;
    QString m_color;
    QString m_id;
    QString m_parentId;
    QString m_opername;
    int     m_level;
};

//} // end of namespace voreen

#endif // ONTOLOGYCLASSITEM_H
