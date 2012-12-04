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

#ifndef ITIOTBRGBAIMAGEVIEWERFACTORY_H
#define ITIOTBRGBAIMAGEVIEWERFACTORY_H

#include "itiotbimageviewerfactory.h"

#include <QObject>

namespace itiviewer{

class ItiOtbImageViewer;

/*!
 * \brief The ItiOtbRgbaImageViewerFactory class
 *
 *  \class A factory class for creating viewers visualizing RGBA type of images
 *
 */
class ItiOtbRgbaImageViewerFactory : public ItiOtbImageViewerFactory
{
public:
    //! ctor
    ItiOtbRgbaImageViewerFactory(QObject *parent = 0);

    //! dtor
    ~ItiOtbRgbaImageViewerFactory();

    //!
    ItiOtbImageViewer* createViewer();
};

} // end of namespace itiviewer

#endif // ITIOTBRGBAIMAGEVIEWERFACTORY_H
