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

#ifndef ONTOLOGYMODEL_H
#define ONTOLOGYMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace voreen {

class OntologyClassItem;
class OWLHelperItem;

//!
//! \brief Models the Ontology Class hierrarchy
//!
class  OntologyClassModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    OntologyClassModel(QObject *parent = 0, bool editable = true);
    ~OntologyClassModel();


    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;



    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole);

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());

    OntologyClassItem* getRootItem() { return rootItem; }

    void traverseHierarchy(OWLHelperItem* , OntologyClassItem*);

    void setRootItem(OntologyClassItem *ri) { rootItem = ri; }

    bool validateLabel(const QVariant& );

    bool isEditable(){ return m_editable; }

    QStringList classIdList(OntologyClassItem*) const;

    bool hasRules(const QString&) const;

    void updateRules(const QString &oldname, const QString &newname);

signals:
    void nameValidationError();

private:

    OntologyClassItem *getItem(const QModelIndex &index) const;

    OntologyClassItem *rootItem;

    bool m_editable;
};

} // end of namespace voreen


#endif // ONTOLOGYMODEL_H
