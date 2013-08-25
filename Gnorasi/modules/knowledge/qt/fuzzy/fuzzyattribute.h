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


#ifndef FUZZYATTRIBUTE_H
#define FUZZYATTRIBUTE_H

#include <QObject>
#include <QStringList>

class FuzzyFunction;
class FuzzyFunctionFactory;

/*!
 * \brief The FuzzyAttribute class
 *  The FuzzyAttribute class is related to one FuzzyFunction , the values for the parameters and the membership value.
 */
class FuzzyAttribute : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief The FUZZYOPERATORTYPE enum
     */
    enum FUZZYOPERATORTYPE{
        FOT_NOTSET          = -1,
        FOT_GREATEREEQUAL   = 0,
        FOT_GREATER         = 1,
        FOT_LESSEQUAL       = 2,
        FOT_LESS            = 3
    };

    /*!
     * \brief FuzzyAttribute
     * \param n
     * \param parent
     */
    explicit FuzzyAttribute(const QString &n , const QString &vN, QObject *parent = 0);

    /*!
     * \brief id
     * \return
     */
    QString displayName() const { return m_displayName; }

    /*!
     * \brief valueName
     * \return
     */
    QString valueName() const { return m_valueName; }
    
    /*!
     * \brief fuzzuFunction
     * \return
     */
    FuzzyFunction* fuzzyFunction( ) { return m_pFuzzyFunction; }

    /*!
     * \brief setFuzzuFunction
     * \param f
     */
    void setFuzzyFunction( FuzzyFunction *f) { m_pFuzzyFunction = f;}

    /*!
     * \brief parameterValueForIndex
     * \param index
     * \return
     */
    double parameterValueForIndex(int index) const ;

    /*!
     * \brief setParameterValueForIndex
     */
    void setParameterValueForIndex(int , double );

    /*!
     * \brief fuzzyOperator
     * \return
     */
    FUZZYOPERATORTYPE fuzzyOperator( ) { return m_fuzzyOperator;}

    /*!
     * \brief setFuzzyOperator
     * \param f
     */
    void setFuzzyOperator(FUZZYOPERATORTYPE f) { m_fuzzyOperator = f; }

    /*!
     * \brief fuzzyOperatorByName
     *  get the operator index from the name
     * \param name
     * \return
     */
    FUZZYOPERATORTYPE fuzzyOperatorByName(const QString &name);

    /*!
     * \brief fuzzyOperatorNames
     *  This is a list if all the available operators
     */
    static QStringList fuzzyOperatorNames;

    /*!
     * \brief fuzzyOperatorXmlNames
     *  This is a helpler list related to the creation xml content.
     */
    static QStringList fuzzyOperatorXmlNames;

    /*!
     * \brief fuzzyOperatorName
     *  A helper function get the name of the operator set
     * \return
     */
    QString fuzzyOperatorName() ;

    /*!
     * \brief fuzzyOperatorXmlName
     *  A helper function get the xml name of the operator set
     *
     * \return
     */
    QString fuzzyOperatorXmlName();


    /*!
     * \brief updateAttribute
     *
     * \return
     */
    bool updateAttribute(int , const QVariant&, FuzzyFunctionFactory* );

    /*!
     * \brief calculateMembershipValue
     */
    double calculateMembershipValue(double );

    /*!
     * \brief threshold
     * \return
     */
    double threshold() const { return m_threshold; }

    /*!
     * \brief setThreshold
     * \param t
     */
    void setThreshold(double t) { m_threshold = t; }

    /*!
     * \brief validate
     * \return
     */
    bool isReady();

signals:
    
public slots:
    
private:

    /*!
     * \brief m_threshold
     *  The threshold is used during the classification
     */
    double m_threshold;

    /*!
     * \brief m_displayName
     */
    QString m_displayName;

    /*!
     * \brief m_valueName
     */
    QString m_valueName;


    /*!
     * \brief m_pFuzzyFunction
     */
    FuzzyFunction *m_pFuzzyFunction;

    /*!
     * \brief m_fuzzyOperator
     */
    FUZZYOPERATORTYPE m_fuzzyOperator;

};

#endif // FuzzyAttribute_H
