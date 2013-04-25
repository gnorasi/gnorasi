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


#include "fuzzyfunctionleftshoulder.h"

FuzzyFunctionLeftShoulder::FuzzyFunctionLeftShoulder(QObject *parent) :
    m_parameterA(-1.0),
    m_parameterB(-1.0),
    FuzzyFunction(parent)
{

}


double FuzzyFunctionLeftShoulder::parameterValueForIndex(int index){
    if(!index) // 0
        return m_parameterA;
    else if(index == 1) // 1
        return m_parameterB;
    else
        return 0.0;
}


void FuzzyFunctionLeftShoulder::setParameterValueForIndex(int index, double val){
    if(index == 0)
        m_parameterA = val;
    else if(index = 1)
        m_parameterB = val;


}


double FuzzyFunctionLeftShoulder::calculate(double val){

    if(val <= m_parameterA)
        return 0.0;
    else if(val >= m_parameterB)
        return 1.0;
    else
        return (val -m_parameterA)/(m_parameterB-m_parameterA);
}


bool FuzzyFunctionLeftShoulder::isReady(){
    return m_parameterA >= 0 && m_parameterB >= 0;
}
