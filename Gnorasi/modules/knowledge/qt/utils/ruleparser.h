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

#ifndef RULEPARSER_H
#define RULEPARSER_H

#include <QObject>

#include <QDomDocument>


//!
//! Functionality, parses xml data related to rules
//! and adds them in memory.
//!
class RuleParser : public QObject
{
    Q_OBJECT
public:
    explicit RuleParser(QObject *parent = 0);

    //! parse the contents of the Xml file at the location given by the filePath
    void parseFile(const QString &filePath, QString *error = 0);
    
signals:
    
public slots:


private:
    //! namespace checking
    //! checks the rdf element namespaces attribute key values contained in the document passed as a parameter
    //! and the namespaces set in the RuleUtility instance while loading a valid OntologyClassModel given by
    //! the OntologyCreator processor
    bool validateNamespaces(const QDomDocument& );
    
};


#endif // RULEPARSER_H
