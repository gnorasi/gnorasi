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

#include "ontologyclassificationmanager.h"

#include "../models/ontologyclass.h"


OntologyClassificationManager* OntologyClassificationManager::m_pInstance = NULL;

OntologyClassificationManager::OntologyClassificationManager(QObject *parent) :
    QObject(parent)
{
}

OntologyClassificationManager* OntologyClassificationManager::instance(){
    if(m_pInstance == NULL){
        m_pInstance = new OntologyClassificationManager();
    }

    return m_pInstance;
}

void OntologyClassificationManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

OntologyClassificationManager::~OntologyClassificationManager(){
    clear();
}


OntologyClass* OntologyClassificationManager::ontologyClassById(int id){
    QList<OntologyClass*>::const_iterator i;
    QString idString = QString::number(id);
    for(i = m_OntoloyClassList.constBegin(); i != m_OntoloyClassList.constEnd(); i++){
        OntologyClass *p = *i;
        if(!p->id().compare(idString))
            return p;
    }

    return 0;
}

OntologyClass* OntologyClassificationManager::ontologyClassById(const QString &id){
    QList<OntologyClass*>::const_iterator i;
    for(i = m_OntoloyClassList.constBegin(); i != m_OntoloyClassList.constEnd(); i++){
        OntologyClass *p = *i;
        if(!p->id().compare(id))
            return p;
    }

    return 0;
}


OntologyClass* OntologyClassificationManager::ontologyByIdx(int idx){

    QList<OntologyClass*>::const_iterator i;
    for(i = m_OntoloyClassList.constBegin(); i != m_OntoloyClassList.constEnd(); i++){
        OntologyClass *p = *i;
        if(p->idx() == idx)
            return p;
    }

    return 0;
}

int OntologyClassificationManager::uniqueIdx(){

    int helper = 1;
    OntologyClass *pClass = ontologyByIdx(helper);
    while(pClass){

        helper++;
        pClass = ontologyByIdx(helper);
    }

    return helper;
}
