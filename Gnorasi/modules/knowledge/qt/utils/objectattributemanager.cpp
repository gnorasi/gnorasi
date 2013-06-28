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

#include "objectattributemanager.h"

#include "objectattribute.h"

ObjectAttributeManager* ObjectAttributeManager::m_pInstance = NULL;

ObjectAttributeManager::ObjectAttributeManager(QObject *parent) :
    QObject(parent)
{
}

ObjectAttributeManager* ObjectAttributeManager::instance(){
    if(m_pInstance == NULL){
        m_pInstance = new ObjectAttributeManager();
    }

    return m_pInstance;
}

void ObjectAttributeManager::deleteInstance(){
    if(m_pInstance != NULL){
        delete m_pInstance;
        m_pInstance = NULL;
    }
}


ObjectAttributeManager::~ObjectAttributeManager(){
    clear();
}

ObjectAttribute* ObjectAttributeManager::objectAttributeById(const QString &idString){
    QList<ObjectAttribute*>::const_iterator i;
    for(i = m_objectAttributeList.constBegin(); i != m_objectAttributeList.constEnd(); i++){
        ObjectAttribute *o = *i;

        if(!o->id().compare(idString))
            return o;
    }

    return 0;
}

ObjectAttribute* ObjectAttributeManager::objectAttributeById(int id){
    QString idString = QString::number(id);
    QList<ObjectAttribute*>::const_iterator i;
    for(i = m_objectAttributeList.constBegin(); i != m_objectAttributeList.constEnd(); i++){
        ObjectAttribute *o = *i;

        if(!o->id().compare(idString))
            return o;
    }

    return 0;
}
