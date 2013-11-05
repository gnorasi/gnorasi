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
 *Copyright (c) Argyros Argyridis <arargyridis@gmail.com>. All rights reserved.	*
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

#include "fuzzyfunctiontrapezoidal.h"
#include <iostream>
FuzzyFunctionTrapezoidal::FuzzyFunctionTrapezoidal(QObject *parent) :
    FuzzyFunction(parent)
{
    for (register int i = 0; i < parametersCount() ; i++)
        m_parameterList[i] = -1;

    QStringList iconlist = FuzzyFunction::availableIcons;
    seticon(iconlist.at(3));
}



double FuzzyFunctionTrapezoidal::parameterValueForIndex(int index){

    if (index < 4)
        return m_parameterList[index];
    else
        return -10000;
}


void FuzzyFunctionTrapezoidal::setParameterValueForIndex(int index, double val)
{

    if(index < MAXPARAMETERSCOUNT)
        m_parameterList[index] = val;
    else {
        std::cerr << "error, Index out of bounds \n";
        exit(1);
    }

}


double FuzzyFunctionTrapezoidal::calculate(double val){

    if ( isReady() ) {
        double trpz = std::min( (val-m_parameterList[0])/(m_parameterList[1]-m_parameterList[0]), 1.0 );
        trpz = std::min ( trpz, (m_parameterList[3] - val)/(m_parameterList[3] - m_parameterList[2]) );
        trpz = std::max (trpz, 0.0);
        return trpz;
    }
    else
          return -1;

}

bool FuzzyFunctionTrapezoidal::isReady(){
    return m_parameterList[0] != -1 && m_parameterList[1] != -1 && m_parameterList[2] != -1 && m_parameterList[3] != -1  ;
}


