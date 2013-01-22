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

#ifndef RULEITEM_H
#define RULEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

namespace voreen {

/*!
 * \brief The RuleItem class
 *
//! The rule item represents a record on the rules table view
//! A rule is a set of conditions applied to an ontology class look in Rule class for more..
//! An ontology class is defined in the ontology process context
//! The rule item has the following properties :
//!  - id ( an index no. , ex. 1 )
//!  - class ( the class it refers to )
//!  - property ( a string value holding the name of the condition its applied..)
//!  - segmentation level ( a number from 0 - 10 holding the level, ask Babis for more )
//!
//!  ** In case of the GeoClassification processor the ruleitem is a region **
//!     This class could be used as the abstract class from where all subclasses could inherit.
//!
 */
class RuleItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  id                  READ id                 )
    Q_PROPERTY(QString  headClass           READ headClass          )
    Q_PROPERTY(QString  property            READ property           )
    Q_PROPERTY(int      segmentationLevel   READ segmentationLevel  )
public:
    explicit RuleItem(const QVector<QVariant>&);
    ~RuleItem();

    //! helper getters
    QString     id()                const { return itemData.value(0).toString();    }
    QString     headClass()         const { return itemData.value(1).toString();    }
    QString     property()          const { return itemData.value(2).toString();    }
    int         segmentationLevel() const { return itemData.value(3).toInt();       }

    int         columnCount() const;
    QVariant    data(int column) const;
    bool        setData(int column, const QVariant &value);

private:

    QVector<QVariant> itemData;
};

} // end of namespace voreen

#endif // RULEITEM_H
