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

#ifndef OBJECTLEVELMANAGER_H
#define OBJECTLEVELMANAGER_H

#include <QObject>

class ObjectLevel;

// a helper macro
#define OBJECTLEVELMANAGER ObjectLevelManager::instance()

class ObjectLevelManager : public QObject
{
    Q_OBJECT
public:
    static ObjectLevelManager* instance();

    static void deleteInstance();

    QList<ObjectLevel*> objectLevelList() const { return m_objectLevelList; }
    void setObjectLevelList(const QList<ObjectLevel*> &l){ m_objectLevelList = l;}


    void addObjectLevel(ObjectLevel* l) { m_objectLevelList.append(l); }
    void removeObjectLevel(ObjectLevel *l) { m_objectLevelList.removeOne(l); }


    void clear() { qDeleteAll(m_objectLevelList); m_objectLevelList.clear(); }


    ObjectLevel* objectLevelById(int ) ;


    int generateNextLevelId() const;
    
signals:
    
public slots:
    
private:
    explicit ObjectLevelManager(QObject *parent = 0);
    virtual ~ObjectLevelManager();

    QList<ObjectLevel*> m_objectLevelList;

    static ObjectLevelManager *m_pInstance;
};

#endif // OBJECTLEVELMANAGER_H
