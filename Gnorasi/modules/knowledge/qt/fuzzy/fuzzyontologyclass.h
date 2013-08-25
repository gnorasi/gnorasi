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

#ifndef FUZZYONTOLOGYCLASS_H
#define FUZZYONTOLOGYCLASS_H

#include <QObject>

class FuzzyAttribute;
class FuzzyFunctionFactory;

/*!
 * \brief The FuzzyOntologyClass class
 *  The FuzzyOntologyClass class is related to a set of FuzzyAttribute items
 *  The FuzzyOntologyClass item is also related to a min / max operator value.
 *  The FuzzyOntologyClass has an id and a class name string variables.
 *
 *
 */
class FuzzyOntologyClass : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief The MINMAXTYPE enum
     */
    enum MINMAXTYPE{
        MMT_NOTSET  = 0,
        MMT_MIN     = 1,
        MMT_MAX     = 2
    };

    explicit FuzzyOntologyClass(QObject *parent = 0);

    /*!
     * \brief addFuzzyAttribute
     * \param a
     */
    void addFuzzyAttribute(FuzzyAttribute *a) { m_fuzzyAttributeList.append(a); }

    /*!
     * \brief removeFuzzyAttribute
     * \param a
     */
    bool removeFuzzyAttribute(FuzzyAttribute *a) { return m_fuzzyAttributeList.removeOne(a); }

    /*!
     * \brief removeFuzzyAttribute
     * \param name
     */
    bool removeFuzzyAttribute(const QString &name);

    /*!
     * \brief fuzzyAttributes
     * \return
     */
    QList<FuzzyAttribute*> fuzzyAttributes() { return m_fuzzyAttributeList;}

    /*!
     * \brief clear
     */
    void clear() { m_fuzzyAttributeList.clear(); }


    /*!
     * \brief minMaxType
     * \return
     */
    MINMAXTYPE minMaxType() { return m_minMaxType; }

    /*!
     * \brief setMinMaxType
     * \param m
     */
    void setMinMaxType(MINMAXTYPE m) { m_minMaxType = m; }

    /*!
     * \brief mixMaxName
     * \return
     */
    QString mixMaxName() ;

    /*!
     * \brief setMinMaxType
     * \param val
     */
    void setMinMaxType(int val) { val == 1? m_minMaxType = MMT_MIN : m_minMaxType = MMT_MAX; }

    /*!
     * \brief isMin
     * \return
     */
    bool isMin() { return m_minMaxType == MMT_MIN? true: false; }

    /*!
     * \brief className
     *  getter
     * \return
     */
    QString className() const { return m_className; }
    void setClassName(const QString &name) { m_className = name ; }


    int id() const { return m_id; }
    void setId(int i ) { m_id = i; }

    /*!
     * \brief isEmpty
     * \return
     */
    bool isEmpty() { return m_fuzzyAttributeList.isEmpty(); }

    /*!
     * \brief updateAttribute
     * \return
     */
    bool updateAttribute(int , int , const QVariant&, FuzzyFunctionFactory* );

    /*!
     * \brief getAttributeByName
     * \return
     */
    FuzzyAttribute* getAttributeByName(const QString &);

    /*!
     * \brief getAttributeByIndex
     * \param index
     * \return
     */
    FuzzyAttribute* getAttributeByIndex(int index);

    /*!
     * \brief calculateMemberShipValues
     */
    void calculateMemberShipValues();


signals:
    
public slots:


private:

    /*!
     * \brief m_className
     */
    QString m_className;

    /*!
     * \brief m_id
     */
    int m_id;

    /*!
     * \brief m_fuzzyAttributeList
     */
    QList<FuzzyAttribute*> m_fuzzyAttributeList;

    /*!
     * \brief m_minMaxType
     */
    MINMAXTYPE m_minMaxType;

    
};

#endif // FUZZYONTOLOGYCLASS_H
