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

#ifndef ITIOTBRGBAIMAGEWIDGET_H
#define ITIOTBRGBAIMAGEWIDGET_H

#include <QGLWidget>

#include "otbImage.h"
#include "itkRGBAPixel.h"
#include "itkFixedArray.h"
#include "otbGlComponent.h"
#include "otbObjectList.h"
#include "otbImageWidgetController.h"

using namespace otb;


const unsigned int Dimension = 2;

typedef itk::RGBAPixel<unsigned char>           PixelType;
typedef otb::Image<PixelType, Dimension>        RGBAImageType;
typedef RGBAImageType::RegionType               RGBARegionType;
typedef RGBAImageType::IndexType                RGBAIndexType;
typedef RGBAImageType::SizeType                 RGBASizeType;

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

class ItiOtbRgbaImageWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit ItiOtbRgbaImageWidget(QWidget *parent = 0);

    virtual ~ItiOtbRgbaImageWidget();

    /** Reads the OpenGl buffer from an image pointer
     *  \param image The image pointer,
     *  \param region The region to read.
     *  Potential exception thrown if region is outside of the buffered
     *  region.
     * This method fills the m_OpenGl buffer according to the region
     *  size. Buffer in flipped over X axis if OTB_USE_GL_ACCEL is OFF.
     */
    virtual void ReadBuffer(const RGBAImageType * image, const RGBARegionType& region);

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
    RGBARegionType openGLBufferedRegion() { return m_OpenGlBufferedRegion; }
    void setOpenGLBufferedRegion(RGBARegionType r) { m_OpenGlBufferedRegion = r; }

    //! setter getter, self explanatory
    RGBARegionType extent() { return m_Extent; }

    /** Compute the linear buffer index according to the 2D region and
     * its 2D index.This method is used when OTB_GL_USE_ACCEL is OFF.
     * The resulting buffer will be flipped over the X axis.
     * \param index 2D index
     * \param region 2D region
     */
    static inline unsigned int ComputeXAxisFlippedBufferIndex(const RGBAIndexType& index, const RGBARegionType& region)
    {
      return (region.GetSize()[1] - 1 + region.GetIndex()[1] -
              index[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
    }

protected:

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


private:
    /** OpenGl zoom factor */
    double m_IsotropicZoom;

    /** The interpolation method */
    GLint m_InterpolationMethod;

    //! helper values
    GLint m_W, m_H;

    /** OpenGl buffer      */
    unsigned char * m_OpenGlBuffer;

    /** OpenGl buffered region */
    RGBARegionType m_OpenGlBufferedRegion;

    /** The display extent */
    RGBARegionType m_Extent;

    /** If the image is subsampled with respect to the original image,
     * this indicates the subsampling rate */
    unsigned int m_SubsamplingRate;

};

} // end of itiviewer

#endif // ITIOTBRGBAIMAGEWIDGET_H
