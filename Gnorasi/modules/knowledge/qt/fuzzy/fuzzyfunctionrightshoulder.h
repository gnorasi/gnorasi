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

#ifndef FUZZYFUNCTIONRIGHTSHOULDER_H
#define FUZZYFUNCTIONRIGHTSHOULDER_H

#include <QObject>

#include "fuzzyfunction.h"

/*!
 * \brief The FuzzyFunctionRightShoulder class
 */
class FuzzyFunctionRightShoulder : public FuzzyFunction
{
    Q_OBJECT
public:
    explicit FuzzyFunctionRightShoulder(QObject *parent = 0);

    /*!
     * \brief paramtersCount
     * \return
     */
    int parametersCount() const { return 2; }

    /*!
     * \brief parameterFunctionForIndex
     * \param index
     * \return
     */
    double parameterFunctionForIndex(int index );

    /*!
     * \brief setParameterValudeForIndex
     * \param index
     * \param val
     */
    void setParameterValueForIndex(int index, double val);
    
    /*!
     * \brief calculate
     *  Implementation
     *  ATM this function has not been implemented yet.
     * \param val
     * \return
     */
    double calculate(double val);

    /*!
     * \brief name
     * \return
     */
    QString name() const { return tr("Right Shoulder"); }

    /*!
     * \brief setParameterA
     * \param a
     */
    void setParameterA( double a) { m_parameterA = a; }
    void setParameterB( double b) { m_parameterB = b; }

    /*!
     * \brief parameterA
     * \return
     */
    double parameterA() const{ return  m_parameterA; }
    double parameterB() const { return m_parameterB ;}

    /*!
     * \brief isReady
     *  Implementation.
     * \return
     */
    bool isReady();

signals:
    
public slots:

private:
    double m_parameterA;
    double m_parameterB;
    
};

#endif // FUZZYFUNCTIONRIGHTSHOULDER_H
