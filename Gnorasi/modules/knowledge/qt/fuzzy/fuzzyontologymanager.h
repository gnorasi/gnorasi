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

#ifndef FUZZYONTOLOGYMANAGER_H
#define FUZZYONTOLOGYMANAGER_H

#include <QObject>


// a helper macro
#define FUZZYONTOLOGYCLASSMANAGER FuzzyOntologyManager::instance()

class FuzzyOntologyClass;
class FuzzyAttribute;

/*!
 * \brief The FuzzyOntologyManager class
 *
 *  This is the manager for the fuzzy ontology sub module.
 *  Take notice that this is a singleton class.
 *  This class manages the FuzzyOntologyClass items.
 */
class FuzzyOntologyManager : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief instance
     * \return
     */
    static FuzzyOntologyManager* instance() ;

    /*!
     * \brief deleteInstance
     */
    static void deleteInstance();

    /*!
     * \brief addFuzzyOntologyClass
     * \param c
     */
    void addFuzzyOntologyClass(FuzzyOntologyClass* c) { m_fuzzyOntolgyClassList.append(c); }

    /*!
     * \brief deleteFuzzuyOntologyClass
     * \param c
     */
    void deleteFuzzuyOntologyClass(FuzzyOntologyClass * c) { m_fuzzyOntolgyClassList.removeOne(c);}

    /*!
     * \brief fuzzyOntologyClasses
     * \return
     */
    QList<FuzzyOntologyClass*> fuzzyOntologyClasses() const  { return m_fuzzyOntolgyClassList; }

    /*!
     * \brief clear
     */
    void clear() { m_fuzzyOntolgyClassList.clear(); }

    /*!
     * \brief fuzzyByOntologyClassByName
     * \param name
     * \return
     */
    FuzzyOntologyClass* fuzzyByOntologyClassByName(const QString &name);

    /*!
     * \brief getAllAttributes
     * \return
     */
    QList<FuzzyAttribute*> getAllAttributes() const;

signals:
    
public slots:
    
private:
    /*!
     * \brief FuzzyOntologyManager
     * \param parent
     */
    explicit FuzzyOntologyManager(QObject *parent = 0);

    /*!
      * This is the destructor
      */
    ~FuzzyOntologyManager();

    /*!
     * \brief m_fuzzyOntolgyClassList
     */
    QList<FuzzyOntologyClass*> m_fuzzyOntolgyClassList;

    /*!
     * \brief m_pInstance
     */
    static FuzzyOntologyManager *m_pInstance;


};

#endif // FUZZYONTOLOGYMANAGER_H
