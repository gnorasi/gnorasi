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

#ifndef RULEUTILITY_H
#define RULEUTILITY_H

#include <QObject>


class Rule;
class OntologyClassItem;

//!
//! This is a singleton class for handling the Rule instances
//! It is created as a singleton in order to be accessed from different
//! locations inside the knowledge module.
//!
class RuleUtility : public QObject
{
    Q_OBJECT
public:
    //! getting the instance
    static RuleUtility* instance();

    //! deletes the instance
    static void deleteInstance();

    //! get all the rules
    QList<Rule*> ruleList() const {return m_ruleList; }

    //! create a new rule and add it to the list
    Rule* createRule(const QString &ocid);

    //! remove the rule from the list
    bool removeRule(Rule *r) { return m_ruleList.removeOne(r); }

    //! get a list of rules that have head class equal to the function parameter
    QList<Rule*> ruleListByHeadClass(const QString &);

    //! get the Rule given an id ( must be unique..)
    Rule* ruleById(const QString &id);

    //! unique id generator
    QString uniqueId() const;

    //! clear all items in the list
    void clear();

    //!
    QString namespaceXmlns()    const    { return m_namespaceXmlns;     }
    QString namespaceXmlBase()  const    { return m_namespaceXmlBase;   }

    // set the namespace values to the dom document
    void setNamespaces(const QString &nsXmlns, const QString &nsXmlBase) {
        m_namespaceXmlBase = nsXmlBase; m_namespaceXmlns = nsXmlns;
    }

    //! get the class names list currenlty used
    QStringList classNamesList() const;

    
signals:
    
public slots:
    
private:
    //
    explicit RuleUtility();

    //
    QList<Rule*> m_ruleList;

    //
    static RuleUtility *m_pInstance;

    //! the following variables are used for content validation
    //! a Rule must be attributed to an existent and valid OntologyClassItem
    //! The owl file containg the data of the OntologyClassItems must
    //! have the same rdf element attribute values for the xmlns and xml:base attribute keys
    QString m_namespaceXmlns;
    QString m_namespaceXmlBase;

};


#endif // RULEUTILITY_H
