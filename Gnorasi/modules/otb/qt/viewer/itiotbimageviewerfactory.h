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

#ifndef ITIIMAGEVIEWERFACTORY_H
#define ITIIMAGEVIEWERFACTORY_H

#include <QObject>

namespace itiviewer{

class ItiOtbImageViewer;

/*!
 * \brief The ITIImageViewerFactory class
 *
 * \class The ITIImageViewerFactory class is an abstract factory which is responsible for creating ITIImageViewer instances
 * Each subclass of this class must reimplement the create method.
 *
 */
class ItiOtbImageViewerFactory : public QObject
{
    Q_OBJECT
public:
    //! ctor
    explicit ItiOtbImageViewerFactory(QObject *parent = 0) : QObject(parent) { }

    //! abstract method, every subclass must implement this class
    virtual ItiOtbImageViewer* createViewer() = 0;

protected:

    
};

} // end of namespace iti

#endif // ITIIMAGEVIEWERFACTORY_H
