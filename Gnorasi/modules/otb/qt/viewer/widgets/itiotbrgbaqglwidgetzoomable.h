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

#ifndef ITIOTBRGBAQGLWIDGETZOOMABLE_H
#define ITIOTBRGBAQGLWIDGETZOOMABLE_H

#include <QGLWidget>
#include <QPen>
#include <QWheelEvent>

#include "../rgba_globaldefs.h"


#include "itiviewerobserver.h"


#include "otbImageWidgetController.h"


using namespace voreen;

namespace itiviewer{

/** \class QOtbImageWidget
*   \brief This class renders an RGB bytes image buffer to the screen.
*   Rendered data can be loaded using the ReadBuffer() method.
*   The SetIsotropicZoom() method allows to tune the zooming (zooming
*   is centered).
*
*   It is also able to display a rectangle on the displayed image.
*  \ingroup Visualization
 */

class ItiOtbRgbaQGLWidgetZoomable : public QGLWidget, public ItiViewerObserver
{
    Q_OBJECT
public:
    explicit ItiOtbRgbaQGLWidgetZoomable(QWidget *parent = 0);

    virtual ~ItiOtbRgbaQGLWidgetZoomable();

    /** Reads the OpenGl buffer from an image pointer
     *  \param image The image pointer,
     *  \param region The region to read.
     *  Potential exception thrown if region is outside of the buffered
     *  region.
     * This method fills the m_OpenGl buffer according to the region
     *  size. Buffer in flipped over X axis if OTB_USE_GL_ACCEL is OFF.
     */
    virtual void ReadBuffer(const RasterImageType * image, const RasterRegionType& region);

    /** Clear the OpenGl buffer */
    void ClearBuffer();

    //! setter getter, self explanatory
    double isotropicZoom() const { return m_IsotropicZoom; }
    void setIsotropicZoom(double z) { m_IsotropicZoom = z; }

    //! setter getter, self explanatory
    unsigned int subSamplingRate() const { return m_SubsamplingRate; }
    void setSubSamplingRate(unsigned int ss) { m_SubsamplingRate = ss; }

    //! setter getter, self explanatory
    unsigned char * openGLBuffer() { return m_OpenGlBuffer;}

    //! setter getter, self explanatory
    RasterRegionType openGLBufferedRegion() { return m_OpenGlBufferedRegion; }
    void setOpenGLBufferedRegion(RasterRegionType r) { m_OpenGlBufferedRegion = r; }

    //! setter getter, self explanatory
    RasterRegionType extent() { return m_Extent; }


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

    /** The interpolation method */
    GLint m_InterpolationMethod;

    //! helper values
    GLint m_W, m_H;

    /** OpenGl buffer      */
    unsigned char * m_OpenGlBuffer;

    /** OpenGl buffered region */
    RasterRegionType m_OpenGlBufferedRegion;

    /*!
     * \brief m_Extent , The display extent handles the visible area's size and index values
     *  The Extend's values are related to the windows's size values
     */
    RasterRegionType m_Extent;

    /** If the image is subsampled with respect to the original image,
     * this indicates the subsampling rate */
    unsigned int m_SubsamplingRate;

    /*!
     * \brief m_nb_displayed_rows , a variable holding the number of rows that are visualized on an image of a fixed size [columns,rows]
     */
    unsigned int m_nb_displayed_rows;

    /*!
     * \brief m_nb_displayed_cols , a variable holding the number of columns that are visualized on an image of a fixed size [columns,rows]
     */
    unsigned int m_nb_displayed_cols;

    /*!
     * \brief m_nb_displayed_rows , a variable holding the first visualized row on an image of a fixed size [columns,rows]
     */
    unsigned int m_first_displayed_row;

    /*!
     * \brief m_nb_displayed_cols , a variable holding the first visualized column on an image of a fixed size [columns,rows]
     */
    unsigned int m_first_displayed_col;

};

} // end of itiviewer

#endif // ITIOTBRGBAQGLWIDGETZOOMABLE_H
