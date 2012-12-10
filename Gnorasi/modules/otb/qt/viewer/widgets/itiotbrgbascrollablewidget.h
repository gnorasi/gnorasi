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

#ifndef ITIOTBRGBASCROLLABLEWIDGET_H
#define ITIOTBRGBASCROLLABLEWIDGET_H

#include "itiviewerobserver.h"

namespace itiviewer{

class ItiOtbRgbaQGLWidget;
class ItiViewerObservable;

/*!
 * \brief The ItiOtbRgbaScrollableWidget class
 *  This class is a container class.
 */
class ItiOtbRgbaScrollableWidget : public ItiViewerObserver
{
    Q_OBJECT
public:
    /*!
     * \brief ItiOtbRgbaScrollableWidget
     * \param parent
     */
    explicit ItiOtbRgbaScrollableWidget(QWidget *parent = 0);

    /*!
     * \brief update , implementation from parent class
     * \param region, the region
     */
    void update(ItiViewerObservable *observable);

    /*!
     * \brief itiOtbRgbaImageWidget
     * \return the open gl widget
     */
    ItiOtbRgbaQGLWidget* itiOtbRgbaImageWidget() const { return m_pOpenGlWidget; }

    /*!
     * \brief draw
     */
    void draw();
    
signals:
    /*!
     * \brief sizeChanged
     * \param size , the size of the widget
     */
    void visibleAreaChanged(const QSize &size);

    
public slots:


private:

    /*!
     * \brief initialize
     */
    void initialize();

    //! The opengl QGLWidget
    ItiOtbRgbaQGLWidget *m_pOpenGlWidget;

};

}

#endif // ITIOTBRGBASCROLLABLEWIDGET_H
