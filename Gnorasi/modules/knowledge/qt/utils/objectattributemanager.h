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
#include <QMultiHash>

class ObjectAttribute;

// a helper macro
#define OBJECTATTRIBUTEMANAGER ObjectAttributeManager::instance()

class ObjectAttributeManager : public QObject
{
    Q_OBJECT
public:

    static ObjectAttributeManager* instance();
    static void deleteInstance();
    
    QMultiHash<int,ObjectAttribute*> objectAttributeHash() const {return m_objectAttributeHash; }
    void setObjectAttributeHash(const QHash<int,ObjectAttribute*> &l) { m_objectAttributeHash = l; }

    void addObjectAttribute(int lid, ObjectAttribute *l) { m_objectAttributeHash.insertMulti(lid,l); }
    void removeObjectAttribute(int lid, ObjectAttribute *l) { m_objectAttributeHash.remove(lid, l); }

    ObjectAttribute* objectAttributeOfLevelById(int lid, const QString &idString) ;
    ObjectAttribute* objectAttributeOfLevelById(int lid, int id) ;

    void clear() { qDeleteAll(m_objectAttributeHash); m_objectAttributeHash.clear(); }

    QList<ObjectAttribute*> objectAttributeOfLevelList(int );
    QStringList objectAttributeNameOfLevelStringList(int );

    void createPreDefinedAttributesForLevel(int l);

signals:
    
public slots:



private:
    explicit ObjectAttributeManager(QObject *parent = 0);
    ~ObjectAttributeManager();

    void createPreDefinedAttribute(int levelId, const QString &name, int otype);

    static ObjectAttributeManager *m_pInstance;

    QMultiHash<int,ObjectAttribute*> m_objectAttributeHash;
    
};

#endif // OBJECTATTRIBUTEMANAGER_H
