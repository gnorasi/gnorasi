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

#ifndef OBJECTATTRIBUTEMANAGER_H
#define OBJECTATTRIBUTEMANAGER_H

#include <QObject>

class ObjectAttribute;

// a helper macro
#define OBJECTATTRIBUTEMANAGER ObjectAttributeManager::instance()

class ObjectAttributeManager : public QObject
{
    Q_OBJECT
public:

    static ObjectAttributeManager* instance();
    static void deleteInstance();
    
    QList<ObjectAttribute*> objectAttributeList() const {return m_objectAttributeList; }
    void setObjectAttributeList(const QList<ObjectAttribute*> &l) { m_objectAttributeList = l; }

    void addObjectAttribute(ObjectAttribute *l) { m_objectAttributeList.append(l); }
    void removeObjectAttribute(ObjectAttribute *l) { m_objectAttributeList.removeOne(l); }

    ObjectAttribute* objectAttributeById(const QString &idString) ;
    ObjectAttribute* objectAttributeById(int id) ;

    void clear() { qDeleteAll(m_objectAttributeList); m_objectAttributeList.clear(); }

signals:
    
public slots:



private:
    explicit ObjectAttributeManager(QObject *parent = 0);
    ~ObjectAttributeManager();

    static ObjectAttributeManager *m_pInstance;

    QList<ObjectAttribute*> m_objectAttributeList;
    
};

#endif // OBJECTATTRIBUTEMANAGER_H
