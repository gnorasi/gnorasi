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

#ifndef ITIOTBVECTORQGLWIDGETZOOMABLE_H
#define ITIOTBVECTORQGLWIDGETZOOMABLE_H

#include <QGLWidget>
#include <QPen>
#include <QWheelEvent>

#include "../../vector_globaldefs.h"

#include "../itiviewerobserver.h"


using namespace voreen;

namespace itiviewer{

//
// Internal classes pre-declaration.
class ImageModelRendererZoomable;
class ImageViewManipulatorZoomable;
class ItiOtbVectorImageViewer;

/** \class QOtbImageWidget
*   \brief This class renders an RGB bytes image buffer to the screen.
*   Rendered data can be loaded using the ReadBuffer() method.
*   The SetIsotropicZoom() method allows to tune the zooming (zooming
*   is centered).
*
*   It is also able to display a rectangle on the displayed image.
*  \ingroup Visualization
 */

class ItiOtbVectorQGLWidgetZoomable : public QGLWidget, public ItiViewerObserver
{
    Q_OBJECT
public:
    explicit ItiOtbVectorQGLWidgetZoomable(ItiOtbVectorImageViewer *parent);

    virtual ~ItiOtbVectorQGLWidgetZoomable();

    //! setter getter, self explanatory
    double isotropicZoom() const { return m_IsotropicZoom; }
    void setIsotropicZoom(double z) { m_IsotropicZoom = z; }

    //! setter getter, self explanatory
    unsigned int subSamplingRate() const { return m_SubsamplingRate; }
    void setSubSamplingRate(unsigned int ss) { m_SubsamplingRate = ss; }


    /*!
     * \brief update , implementation from parent class
     * \param region, the region
     */
    void updateObserver(ItiViewerObservable *observable);

    /*!
     * \brief draw
     */
    void draw();

public slots:

    /*!
     * \brief onLargestPossibleRegionChanged
     * \param largestRegion
     */
    void onLargestPossibleRegionChanged(const ImageRegionType& largestRegion);

    /*!
     * \brief zoomIn
     */
    void zoomIn();

    /*!
     * \brief zoomOut
     */
    void zoomOut();

    /*!
     * \brief translate
     * \param dx
     * \param dy
     */
    void translate(int dx, int dy);

signals:
    /*!
     * \brief visibleAreaChanged , this signal is emitted uppon the view resizing
     * \param rect
     */
    void visibleAreaChanged(const QRect &rect);

    /*!
     * \brief currentIndexChanged
     * \param text
     */
    void currentIndexChanged(const QString &text);


protected:

    /*!
     * \brief setupExtents
     */
    void setupRowColumnDisplay();

    /*!
     * \brief setupAndSendSignal
     */
    void setupAndSendSignal();

    /*!
     * \brief mousePressEvent
     */
    void wheelEvent(QWheelEvent *);


    /*!
     * \brief initializeGL
     *  reimplemented method declared in the QGLWidget class
     */
    void initializeGL();

    /*!
     * \brief resizeGL , reimplemented method declared in the QGLWidget class
     * \param w , the width parameter
     * \param h , the height parameter
     */
    void resizeGL(int w, int h);

    /*!
     * \brief paintGL , reimplemented method declared in the QGLWidget class
     */
    void paintGL();

    /*!
     * \brief mouseMoveEvent
     * \param event
     */
    void mouseMoveEvent(QMouseEvent *event);

    /*!
     * \brief resizeEvent
     * \param event
     */
//    void resizeEvent(QResizeEvent* event);


private:
    /*!
     * \brief setupcolumnRowParameters
     */
    void initializeColumnRowParameters();

    /*!
     * \brief setupViewport, basically setup the extends
     * \param width
     * \param height
     */
    void setupViewport(int w, int h);

    /** OpenGl zoom factor */
    double m_IsotropicZoom;

    /** Event handler pointer */
    ImageViewManipulatorZoomable* m_pImageViewManipulator;

    /** Model Renderer pointer */
    ImageModelRendererZoomable* m_pImageModelRenderer;

    /** If the image is subsampled with respect to the original image,
     * this indicates the subsampling rate */
    unsigned int m_SubsamplingRate;

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;


};

} // end of itiviewer

#endif // ITIOTBVECTORQGLWIDGETZOOMABLE_H
