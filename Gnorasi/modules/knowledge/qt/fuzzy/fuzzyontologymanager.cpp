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

#include "fuzzyontologymanager.h"
#include "fuzzyontologyclass.h"
#include "fuzzyattribute.h"

FuzzyOntologyManager* FuzzyOntologyManager::m_pInstance = NULL;

FuzzyOntologyManager::FuzzyOntologyManager(QObject *parent) :
    QObject(parent)
{
}


FuzzyOntologyManager* FuzzyOntologyManager::instance(){
    if(m_pInstance == NULL)
        m_pInstance = new FuzzyOntologyManager();

    return m_pInstance;
}


void FuzzyOntologyManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}


FuzzyOntologyClass* FuzzyOntologyManager::fuzzyByOntologyClassByName(const QString &name){
    QList<FuzzyOntologyClass*>::const_iterator i;
    for(i = m_fuzzyOntolgyClassList.constBegin(); i != m_fuzzyOntolgyClassList.constEnd(); i++)
    {
        FuzzyOntologyClass *pClass = *i;
        QString className = pClass->className();
        if(!className.compare(name))
            return pClass;
    }

    return 0;
}


FuzzyOntologyManager::~FuzzyOntologyManager(){
    qDeleteAll(m_fuzzyOntolgyClassList);
    m_fuzzyOntolgyClassList.clear();
}


QList<FuzzyAttribute*> FuzzyOntologyManager::getAllAttributes() const{
    QList<FuzzyAttribute*> list;

    QList<FuzzyOntologyClass*>::const_iterator i;
    for(i = m_fuzzyOntolgyClassList.constBegin(); i != m_fuzzyOntolgyClassList.constEnd(); i++){
        FuzzyOntologyClass *pClass = *i;

        QList<FuzzyAttribute*> aList = pClass->fuzzyAttributes();
        QList<FuzzyAttribute*>::const_iterator a;
        for(a = aList.constBegin(); a != aList.constEnd(); a++){
            FuzzyAttribute *pAtr = *a;
            list << pAtr;
        }
    }

    return list;
}
