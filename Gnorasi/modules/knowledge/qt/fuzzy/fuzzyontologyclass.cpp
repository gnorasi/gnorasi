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

#include "fuzzyontologyclass.h"

#include "fuzzyattribute.h"

FuzzyOntologyClass::FuzzyOntologyClass(QObject *parent) :
    QObject(parent)
{
}


bool FuzzyOntologyClass::removeFuzzyAttribute(const QString &name){
    FuzzyAttribute *pAttribute = getAttributeByName(name);

    if(pAttribute != NULL){
        m_fuzzyAttributeList.removeOne(pAttribute);
        return true;
    }

    return false;
}

FuzzyAttribute* FuzzyOntologyClass::getAttributeByName(const QString &name){
    QList<FuzzyAttribute*>::const_iterator i;
    for(i = m_fuzzyAttributeList.constBegin(); i != m_fuzzyAttributeList.constEnd(); i++){
        FuzzyAttribute *pAttribute = *i;

        if(!pAttribute->valueName().compare(name))
            return pAttribute;
    }

    return 0;
}

FuzzyAttribute* FuzzyOntologyClass::getAttributeByIndex(int index){
    if(index >= 0 && index < m_fuzzyAttributeList.count())
        return m_fuzzyAttributeList.at(index);
    else
        return 0;
}

bool FuzzyOntologyClass::updateAttribute(int row, int column, const QVariant &val, FuzzyFunctionFactory *pFactory){
    FuzzyAttribute *pFuzzyAttribute = getAttributeByIndex(row);
    if(pFuzzyAttribute){
        return pFuzzyAttribute->updateAttribute(column,val,pFactory);
    }


    return false;
}

QString FuzzyOntologyClass::mixMaxName(){
    if(m_minMaxType == MMT_MIN)
        return QLatin1String("min");
    else
        return QLatin1String("max");
}

void FuzzyOntologyClass::calculateMemberShipValues(){
    QList<FuzzyAttribute*>::const_iterator i;
    for(i = m_fuzzyAttributeList.constBegin(); i != m_fuzzyAttributeList.constEnd(); i++){
        FuzzyAttribute *pAttr = *i;
//        pAttr->calculateMembershipValue();
    }
}
