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

#ifndef OBJECTLEVEL_H
#define OBJECTLEVEL_H

#include <QObject>


class ObjectLevel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int      id      READ id     WRITE setId     NOTIFY idChanged)
    Q_PROPERTY(QString  name    READ name   WRITE setName   NOTIFY nameChanged)
public:
    explicit ObjectLevel(QObject *parent = 0);
    
    int id() const {return m_id; }
    void setId(int i){ m_id = i; }

    QString name() const {return m_name; }
    void setName(const QString &name) { m_name = name; }

signals:
    void idChanged();
    void nameChanged();
public slots:

private:
    int m_id;
    QString m_name;
    
};


#endif // OBJECTLEVEL_H
