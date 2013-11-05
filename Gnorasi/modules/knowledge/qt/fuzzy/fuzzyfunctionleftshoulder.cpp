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
 * Copyright (c) Argyros Argyridis <arargyridis@gmail.com>. All rights reserved. *
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
#include <iostream>

FuzzyFunctionLeftShoulder::FuzzyFunctionLeftShoulder(QObject *parent) :

    FuzzyFunction(parent)
{
    m_parameterList[0] = -1.0;
    m_parameterList[1] = -1.0;

    QStringList iconlist = FuzzyFunction::availableIcons;
    seticon(iconlist.at(0));
}


double FuzzyFunctionLeftShoulder::parameterValueForIndex(int index){
    if(!index) // 0
        return m_parameterList[0];
    else if(index == 1) // 1
        return m_parameterList[1];
    else
        return 0.0;
}


void FuzzyFunctionLeftShoulder::setParameterValueForIndex(int index, double val){
    if(index < 2)
        m_parameterList[index] = val;
    else {
        std::cerr << "error, Index out of bounds \n";
        exit(1);
    }


}


double FuzzyFunctionLeftShoulder::calculate(double val){

    if ( isReady() ) {
            if(val <= m_parameterList[0])
                return 1.0;
            else if(val >= m_parameterList[1])
                return 0.0;
            else
                return (m_parameterList[1] - val)/(m_parameterList[1]-m_parameterList[0]);
    }
    else
        return -1;
}


bool FuzzyFunctionLeftShoulder::isReady(){
    return m_parameterList[0] != -1 && m_parameterList[1] != -1;
}

