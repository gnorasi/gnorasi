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

#ifndef ITBVIEWEROBSERVABLEREGION_H
#define ITBVIEWEROBSERVABLEREGION_H

#include <QObject>

#include "itiviewerobservable.h"

#include "../widgets/itiotbrgbaqglwidget.h"

namespace itiviewer{

/*!
 * \brief The ItiViewerObservableRegion class
 *  The region in a conceptual view is an area where the viewer is focused on
 *  In the viewer's UI this focus region is represented by a red lined rectangle in
 *  the area of the scrollable widget.
 *  In a c++ view it encaptulates an otb image region.
 */
class ItiViewerObservableRegion : public ItiViewerObservable
{
    Q_OBJECT
public:
    /*!
     * \brief ItiOtbRgbaFocusRegion
     * \param parent
     */
    explicit ItiViewerObservableRegion(QObject *parent = 0) : ItiViewerObservable(parent) { }

    /*!
     * \brief region , getter
     * \return
     */
    RasterRegionType region() const { return m_region; }

    /*!
     * \brief setRegion , setter
     * \param r , the RasterRegionType
     */
    void setRegion(RasterRegionType r) { m_region = r; }

signals:
    
public slots:

private:
    RasterRegionType m_region;

    
};

} // namespace itiviewer

#endif // ITBVIEWEROBSERVABLEREGION_H
