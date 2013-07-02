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

#ifndef FUZZYRULE_H
#define FUZZYRULE_H

#include <QObject>

class                   FuzzyFunction;
class                   FuzzyRuleModel;

namespace voreen {

class                   RuleItem;
class                   OntologyClass;

//!
//! \brief The FuzzyRule class contains information about Rules the user can set through
//! the related processors example GeoClassification processor.
//!
//! A Rule is always attributed to a reference Ontology Class
//! A Rule has a confidence number ( ask Babis for more )
//! A Rule has a unique if possible id
//! A Rule has a list of Rule items
//!
class                   FuzzyRule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int      id          READ id         WRITE   setid           NOTIFY idChanged)
    Q_PROPERTY(QString  name        READ name )
    Q_PROPERTY(int      attribute   READ attribute  WRITE   setAttribute    NOTIFY attributeChanged)
    Q_PROPERTY(int levelId READ levelId WRITE setlevelId NOTIFY levelIdChanged)
//    Q_PROPERTY(int      headClass   READ headClass  WRITE   setHeadClass    NOTIFY headClassChanged)
//    Q_PROPERTY(double   confidence  READ confidence WRITE   setConfidence   NOTIFY confidenceChanged)
public:
    virtual             ~FuzzyRule();
    
    //
    int                 id() const      { return m_id; }
    void                setid(int &cl)  { m_id = cl; }

    QString             name() const ;

    int                 attribute() const { return m_attributeId;}
    void                setAttribute(int i ) { m_attributeId = i; }

    int                 levelId() const { return m_levelId; }
    void                setlevelId(int i) { m_levelId = i; }

    bool                header() const { return m_header; }
    void                setHeader(bool h) { m_header = h; }

    bool                operatorType() const { return m_operatorType; }
    void                setoperatorType(bool v) { m_operatorType = v; }

    //
//    QString             headClass() const { return m_headClass; }
//    void                setHeadClass(const QString &cl) { m_headClass = cl; }

    //
//    double              confidence() const { return m_confidence; }
//    void                setConfidence(double l) { m_confidence = l; }

    //
//    void                setRuleItem(QList<RuleItem*> l) { m_ruleItemList = l ; }
//    QList<RuleItem*>    ruleItemList() { return m_ruleItemList; }

    //
//    void                appendRuleItem(RuleItem *r) { m_ruleItemList.append(r) ; }
//    bool                removeRuleItem(RuleItem *r) { return m_ruleItemList.removeOne(r); }
//    void                removeRuleItems(int position,int rows);

    //
//    int                 childCount() { return m_ruleItemList.count(); }

    //
    RuleItem*           childAt(int);

    //
//    QList<RuleItem*>    ruleItemList() const {return m_ruleItemList; }

    //
//    RuleItem*           createRuleItem() ;


    FuzzyFunction*      funzzyFunction() { return m_pFuzzyFunction; }
    void                setFuzzyFunction(FuzzyFunction *p) { m_pFuzzyFunction = p; }

signals:
    void                idChanged();
    void                headClassChanged();
    void                confidenceChanged();
    void                attributeChanged();
    void                levelIdChanged();
    void                headerChanged();
    void                operatorTypeChanged();
    
public slots:

private:
    friend class        FuzzyRuleModel;
    friend class        OntologyClass;

    explicit            FuzzyRule(FuzzyFunction *pFunction = 0, QObject *parent = 0);

//    QList<RuleItem*>    m_ruleItemList;

    FuzzyFunction       *m_pFuzzyFunction;

    int                 m_id;

    int                 m_attributeId;
    int                 m_levelId;

    bool                m_header;
    bool                m_operatorType;
};

} // end of namespace voreen

#endif // FUZZYRULE_H
