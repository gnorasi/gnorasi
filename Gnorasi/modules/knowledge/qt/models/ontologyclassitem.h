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

#ifndef ONTOLOGYCLASSITEM_H
#define ONTOLOGYCLASSITEM_H

#include <QList>
#include <QVariant>
#include <QVector>


//!
//! This class stores data related to OntologyClass items handled by the OntologyClassModel
//!
class OntologyClassItem
{
public:
    OntologyClassItem(const QVector<QVariant> &data, OntologyClassItem *parent = 0);
    ~OntologyClassItem();

    QString id() const { return m_id; }
    void setId(QString i) { m_id = i; }

    OntologyClassItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    OntologyClassItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    QList<OntologyClassItem*> getChildItems() const { return childItems; }

private:
    QList<OntologyClassItem*> childItems;
    QVector<QVariant> itemData;
    OntologyClassItem *parentItem;

    QString m_id;
};

#endif // ONTOLOGYCLASSITEM_H
