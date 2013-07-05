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

#ifndef OWLWRITER_H
#define OWLWRITER_H

#include <QtCore/QObject>

#include <QtXml/QDomDocument>

#include "voreen/qt/voreenmoduleqt.h"

class OntologyClass;

namespace voreen {

class OntologyClassItem;

class OwlWriter : public QObject{
    Q_OBJECT

public:

    OwlWriter(QObject *parent = 0);

    void createDocument();

    void createDocumentVersion2();

    //! append the data from the OntologyClassItem
    void appendData(OntologyClassItem* );


    void appendData(OntologyClass *);

    //!
    //! set the namespaces edited by the user
    //!
    //! vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    //!
    //! Take notice that this function must be called prior to the appendData() function..
    //!
    //! ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    void setupNamespaces(const QString &nsXmlns, const QString &nsXmlBase);

    //! return the dom document data as text
    QString docToText();

    //! returns the QDomDocument
    QDomDocument domDocument() { return doc; }

    /*!
     * \brief appendRulesData
     */
    void appendRulesData();

private:

    //!
    QString m_namespaceXmlns;

    //!
    QString m_namespaceXmlBase;

    //!
    int helperCounter;

    QDomDocument doc;

    QDomElement owlrootElement;
}; // OWLWRITER_H

} // end of namespace voreen

#endif
