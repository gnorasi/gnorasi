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

#ifndef OWLHELPERITEM_H
#define OWLHELPERITEM_H

#include <QObject>

#include <QDomDocument>

#include "voreen/qt/voreenmoduleqt.h"

namespace voreen {


class OWLHelperItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id       READ id         WRITE setId         NOTIFY idChanged)
    Q_PROPERTY(QString label    READ label      WRITE setLabel      NOTIFY labelChanged)
    Q_PROPERTY(QString comment  READ comment    WRITE setComment    NOTIFY commentChanged)
public:
    explicit OWLHelperItem(const QString &id = QString(), const QString &lbl = QString(), const QString &cmt = QString(), QObject *parent = 0);

    QString id() const { return m_id; }
    void setId(const QString &s) { m_id = s; }

    QString label() const { return m_label; }
    void setLabel(const QString &s) { m_label = s; }

    QString comment() const { return m_comment; }
    void setComment(const QString &s) { m_comment = s; }

    QList<OWLHelperItem*> owlChildren() const { return m_owlchildren; }

    void parseChildren(QDomDocument );

signals:
    void labelChanged();
    void commentChanged();
    void idChanged();

private:
    QList<OWLHelperItem*> m_owlchildren;
    
    bool checkSiblings(const QString &);

    void parseSiblings(QDomDocument doc);

    QString m_label;
    QString m_comment;
    QString m_id;
};

} // end of namepsace voreen

#endif // OWLHELPERITEM_H
