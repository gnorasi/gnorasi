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

#ifndef OWLPARSER_H
#define OWLPARSER_H

#include <QtCore/QObject>

#include <QtXml/QDomDocument>


class OWLHelperItem;

//!
//! This class is a parse of the OWL document
//!
class OWLParser : public QObject{
    Q_OBJECT

public:

    OWLParser(QObject *parent = 0);

    //! parse the file given by the QString parameter ..
    void parse(const QString & );

    //! parse the content given by the QString parameter
    void parseContent(const QString &);

    OWLHelperItem* rootOWLHelperItem() const { return m_pOWLHelperItem; }

    //!
    QString namespaceXmlns()const { return m_namespace_xmlns; }
    QString namespaceXmlBase() const { return m_namespace_xml_base; }

private:

    //! setup the namespaces
    //! this function should be called after the document is parsed..
    void setupNamespaces(const QDomDocument& );

    OWLHelperItem *m_pOWLHelperItem;

    //! These namespaces are set when a new Ontology hierrarchy is being created
    //! Should be set when a file is parsed
    QString m_namespace_xmlns;
    QString m_namespace_xml_base;

}; // OWLPARSER_H


#endif
