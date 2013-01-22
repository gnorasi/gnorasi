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

#ifndef RULEWRITER_H
#define RULEWRITER_H

#include <QObject>

#include <QDomDocument>

namespace voreen {

class RuleItem;

class RuleWriter : public QObject
{
    Q_OBJECT
public:
    explicit RuleWriter(QObject *parent = 0);

    //! creates a new QDomDocument
    void createDocument();

    //! append the data from the OntologyClassItem
    void appendData();

    //! return a QString of the QDomDocument
    QString docToText();

    //! self explanatory
    QDomDocument domDocument() { return doc; }

    //! set the namespace values to the dom document
    void setNamespaces(const QString &nsXmlns, const QString &nsXmlBase);


private:

    int helperCounter;

    QDomDocument doc;

    QDomElement rootElement;
    
};

} // end of namespace voreen


#endif // RULEWRITER_H
