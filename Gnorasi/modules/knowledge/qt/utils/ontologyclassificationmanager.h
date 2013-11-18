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

#ifndef ONLOLOGYCLASSIFICATIONMANAGER_H
#define ONLOLOGYCLASSIFICATIONMANAGER_H

#include <QObject>

// a helper macro
#define ONTOLOGYCLASSIFICATIONMANAGER OntologyClassificationManager::instance()

class OntologyClass;

class OntologyClassificationManager : public QObject
{
    Q_OBJECT
public:

    static OntologyClassificationManager* instance();
    static void deleteInstance();
    
    QList<OntologyClass*> ontologyClassList() const { return m_OntoloyClassList; }
    void setOntologyClassList(QList<OntologyClass*> list) { m_OntoloyClassList = list; }

    //
    void addOntologyClass(OntologyClass *p) { m_OntoloyClassList.append(p); }
    void removeOntologyClass(OntologyClass *p) { m_OntoloyClassList.removeOne(p); }

    void clear(){ /*qDeleteAll(m_OntoloyClassList);*/ m_OntoloyClassList.clear(); }

    OntologyClass* ontologyClassById(int );

    OntologyClass* ontologyClassById(const QString& );

    OntologyClass* ontologyByIdx(int idx);

    int uniqueIdx();

signals:
    
public slots:


private:
    explicit OntologyClassificationManager(QObject *parent = 0);
    ~OntologyClassificationManager();

    static OntologyClassificationManager *m_pInstance;

    QList<OntologyClass*> m_OntoloyClassList;
    
};




#endif // FUZZYCLASSIFICATIONMANAGER_H
