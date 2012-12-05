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

#ifndef ITIOTBRGBAIMAGEVIEWER_H
#define ITIOTBRGBAIMAGEVIEWER_H

#include <QtCore>
#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>

#include "itiotbimageviewer.h"
#include "itiotbimagemanager.h"


namespace itiviewer{

class ItiOtbRgbaScrollableWidget;
class ItiOtbRgbaFullWidget;
class ItiOtbRgbaZoomableWidget;
class ItiOtbRgbaFocusRegion;

/** \class ITIOTBImageViewer
 *   \brief This class implements a standard visualization tool to be
 *   plugged at the end of a pipeline.
 *
 *  \ingroup Visualization
 *
 */
class ItiOtbRgbaImageViewer : public ItiOtbImageViewer
{
    Q_OBJECT
public:
    explicit ItiOtbRgbaImageViewer(QWidget *parent = 0);

    //! implementation
    void disassembleWidgets();

    //! implementation
    void assembleWidgets();

    //! implementation
    void draw();

    //! implementation
    void applyGreyScaleColorMode(int band);

    //! implementation
    void applyRGBColorMode(int red, int green, int blue);

    //! implementation
    void applyContrastEnhancementMethod(CC ce, double aval, double bval);

signals:
    
public slots:

private:
    void setupLayout();

    //! The Scrollable QGLWidget
    ItiOtbRgbaScrollableWidget *m_pItiOtbRgbaImageWidgetScroll;

    //! The Full QGLWidget
    ItiOtbRgbaFullWidget *m_pItiOtbRgbaImageWidgetFull;

    //! The Zoomable QGLWidget
    ItiOtbRgbaZoomableWidget *m_pItiOtbRgbaImageWidgetZoom;

    //! The left vertical layout, needed for assembiling and disassembliing purposes
    QVBoxLayout *m_pvBoxLayoutLeft;

    //! The right vertical layout, needed for assembiling and disassembliing purposes
    QVBoxLayout *m_pVboxLayoutRight;

    //! The main layout, needed for assembiling and disassembliing purposes
    QVBoxLayout *m_pMainLayout;

    //! The horizontal splitter;
    QSplitter *m_pSplitter;

    //! the following labels were taken from the monteverdi viewer
    //! a label showing a title for the full resolution view
    QLabel *m_pLabelFullResolution;

    //! a label showing a title for the navigation view
    QLabel *m_pLabelNavigationView;

    //! a label showing a title for the zoom view
    QLabel *m_pLabelZoomView;

    //! a label showing a title for the metadata view
    QLabel *m_pLabelMetadataView;

    //! Container widget
    QWidget *m_pMetadataWidget;

    
};

} // end of namespace itiviewer

#endif // ITIOTBRGBAIMAGEVIEWER_H
