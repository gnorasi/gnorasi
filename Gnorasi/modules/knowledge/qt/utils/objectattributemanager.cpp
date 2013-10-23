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

#include <QStringList>

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

ObjectAttribute* ObjectAttributeManager::objectAttributeOfLevelById(int lid, const QString &idString){
    QList<ObjectAttribute*> list = m_objectAttributeHash.values(lid);
    QList<ObjectAttribute*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        ObjectAttribute *o = *i;

        if(!o->id().compare(idString))
            return o;
    }

    return 0;
}

ObjectAttribute* ObjectAttributeManager::objectAttributeOfLevelById(int lid, int id){
    QString idString = QString::number(id);
    QList<ObjectAttribute*> list = m_objectAttributeHash.values(lid);
    QList<ObjectAttribute*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        ObjectAttribute *o = *i;

        if(!o->id().compare(idString))
            return o;
    }

    return 0;
}

QList<ObjectAttribute*> ObjectAttributeManager::objectAttributeOfLevelList(int lid){
    QList<ObjectAttribute*> list = m_objectAttributeHash.values(lid);
    QList<ObjectAttribute*>::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        ObjectAttribute *oa = *i;
        list << oa;
    }

    return list;
}

QStringList ObjectAttributeManager::objectAttributeNameOfLevelStringList(int lid){
    QStringList list;
    QList<ObjectAttribute*> helperList = m_objectAttributeHash.values(lid);
    QList<ObjectAttribute*>::const_iterator i;
    for(i = helperList.constBegin(); i != helperList.constEnd(); i++){
        ObjectAttribute *oa = *i;
        list << oa->name();
    }

    return list;
}


void ObjectAttributeManager::createPreDefinedAttribute(int levelId, const QString &name, int otype){

    ObjectAttribute *pOA = new ObjectAttribute(OBJECTATTRIBUTEMANAGER);
    pOA->setId(name);
    pOA->setlevelId(levelId);
    pOA->setName(name);
    pOA->setotype(otype);
    OBJECTATTRIBUTEMANAGER->addObjectAttribute(levelId,pOA);

}

void ObjectAttributeManager::createPreDefinedAttributesForLevel(int l){

//
// SHAPE
//
    createPreDefinedAttribute(l,QLatin1String("Area"),2);
    createPreDefinedAttribute(l,QLatin1String("Length"),2);
    createPreDefinedAttribute(l,QLatin1String("Width"),2);
    createPreDefinedAttribute(l,QLatin1String("Border length"),2);
    createPreDefinedAttribute(l,QLatin1String("Length/width"),2);
    createPreDefinedAttribute(l,QLatin1String("Shape index"),2);
    createPreDefinedAttribute(l,QLatin1String("Density"),2);
    createPreDefinedAttribute(l,QLatin1String("Main direction"),2);
    createPreDefinedAttribute(l,QLatin1String("Assymetry"),2);

//
// Spatial relations
//
    createPreDefinedAttribute(l,QLatin1String("Contains"),3);
    createPreDefinedAttribute(l,QLatin1String("ContainsProperly"),3);
    createPreDefinedAttribute(l,QLatin1String("Covers"),3);
    createPreDefinedAttribute(l,QLatin1String("CoveredBy"),3);
    createPreDefinedAttribute(l,QLatin1String("Crosses"),3);
    createPreDefinedAttribute(l,QLatin1String("Disjoint"),3);
    createPreDefinedAttribute(l,QLatin1String("Equals"),3);
    createPreDefinedAttribute(l,QLatin1String("Intersects"),3);
    createPreDefinedAttribute(l,QLatin1String("Overlaps"),3);
    createPreDefinedAttribute(l,QLatin1String("Relate"),3);
    createPreDefinedAttribute(l,QLatin1String("Touches"),3);
    createPreDefinedAttribute(l,QLatin1String("Within"),3);
}
