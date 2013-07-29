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

#include "fuzzyoperatormanager.h"
#include "fuzzyoperator.h"

FuzzyOperatorManager* FuzzyOperatorManager::m_pInstance = NULL;

FuzzyOperatorManager::FuzzyOperatorManager(QObject *parent) :
    QObject(parent)
{
}

FuzzyOperatorManager::~FuzzyOperatorManager(){
    clear();
}

FuzzyOperatorManager* FuzzyOperatorManager::instance()
{
    if(m_pInstance == NULL)
        m_pInstance = new FuzzyOperatorManager();

    return m_pInstance;
}

void FuzzyOperatorManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

FuzzyOperator* FuzzyOperatorManager::fuzzyOperatorByName(const QString &name){
    QList<FuzzyOperator*>::const_iterator i;
    for(i = m_fuzzyOperatorList.constBegin(); i != m_fuzzyOperatorList.constEnd(); i++){
        FuzzyOperator *pOperator = *i;

        QString n = pOperator->name();

        if(!name.compare(n))
            return pOperator;
    }

    return 0;
}
