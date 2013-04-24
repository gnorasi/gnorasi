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


#include "fuzzyattribute.h"
#include "fuzzyfunctionfactory.h"
#include "fuzzyfunction.h"

#include <QVariant>

QStringList FuzzyAttribute::fuzzyOperatorNames = QStringList()  << ">=" << ">" << "<=" << "<";
QStringList FuzzyAttribute::fuzzyOperatorXmlNames = QStringList() << "ge" << "g" << "le" << "l";


FuzzyAttribute::FuzzyAttribute(const QString &n , const QString &vN, QObject *parent) :
    m_displayName(n), m_valueName(vN), m_fuzzyOperator(FOT_NOTSET), QObject(parent)
{
    m_pFuzzyFunction = NULL;
    m_threshold = 0.0;
}

double FuzzyAttribute::parameterValueForIndex(int index) const{
    return m_pFuzzyFunction->parameterFunctionForIndex(index);
}


QString FuzzyAttribute::fuzzyOperatorName(){
    QString name ;
    if((int)m_fuzzyOperator >= 0 && (int)m_fuzzyOperator < fuzzyOperatorNames.count())
        name = fuzzyOperatorNames.at((int)m_fuzzyOperator);

    return name;
}

QString FuzzyAttribute::fuzzyOperatorXmlName(){
    QString name ;
    if((int)m_fuzzyOperator >= 0 && (int)m_fuzzyOperator < fuzzyOperatorXmlNames.count())
        name = fuzzyOperatorXmlNames.at((int)m_fuzzyOperator);

    return name;
}

void FuzzyAttribute::setParameterValueForIndex(int index, double val){
    m_pFuzzyFunction->setParameterValueForIndex(index,val);
}

FuzzyAttribute::FUZZYOPERATORTYPE FuzzyAttribute::fuzzyOperatorByName(const QString &name){
    return (FUZZYOPERATORTYPE)fuzzyOperatorNames.indexOf(name);
}


bool FuzzyAttribute::updateAttribute(int index, const QVariant &val, FuzzyFunctionFactory *pFactory){
    if(index == 1){ // then this is the function
        QString ftype = val.toString();
        FuzzyFunction *pFuzzyFunction = pFactory->createFuzzyFunction(ftype);
        setFuzzyFunction(pFuzzyFunction);

    }else if(index == m_pFuzzyFunction->MAXPARAMETERSCOUNT + 2){ // then it is the operator
        QString oname = val.toString();
        setFuzzyOperator(fuzzyOperatorByName(oname));
    }else if(index == m_pFuzzyFunction->MAXPARAMETERSCOUNT + 3){ // then it is the threshold value
        setThreshold(val.toDouble());
    }else{ // the it is the parameters value
        m_pFuzzyFunction->setParameterValueForIndex(index-2,val.toDouble());
    }

    return true;
}

bool FuzzyAttribute::isReady(){
    return m_pFuzzyFunction->isReady();
}


double FuzzyAttribute::calculateMembershipValue(double val){
    if(m_pFuzzyFunction)
        return m_pFuzzyFunction->calculate(val);
}
