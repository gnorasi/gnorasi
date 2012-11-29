#ifndef QOTBIMAGEWIDGET_H
#define QOTBIMAGEWIDGET_H

#include <QGLWidget>

#include "otbImage.h"
#include "itkRGBAPixel.h"
#include "itkFixedArray.h"
#include "otbGlComponent.h"
#include "otbObjectList.h"
#include "otbImageWidgetController.h"

using namespace otb;

const unsigned int Dimension = 2;

typedef itk::RGBAPixel<unsigned char>       PixelType;
typedef otb::Image<PixelType, Dimension>    ImageType;
typedef ImageType::RegionType               RegionType;
typedef ImageType::IndexType                IndexType;
typedef ImageType::SizeType                 SizeType;

/** GlComponent typedef */
typedef GlComponent                    GlComponentType;
typedef ObjectList<GlComponentType>    GlComponentListType;

/** Controller typedef */
typedef ImageWidgetController          ControllerType;


/** \class QOtbImageWidget
*   \brief This class renders an RGB bytes image buffer to the screen.
*   Rendered data can be loaded using the ReadBuffer() method.
*   The SetIsotropicZoom() method allows to tune the zooming (zooming
*   is centered).
*
*   It is also able to display a rectangle on the displayed image.
*  \ingroup Visualization
 */

class QOTBImageWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit QOTBImageWidget(QWidget *parent = 0);

    virtual ~QOTBImageWidget();

    /** Reads the OpenGl buffer from an image pointer
     *  \param image The image pointer,
     *  \param region The region to read.
     *  Potential exception thrown if region is outside of the buffered
     *  region.
     * This method fills the m_OpenGl buffer according to the region
     *  size. Buffer in flipped over X axis if OTB_USE_GL_ACCEL is OFF.
     */
    virtual void ReadBuffer(const ImageType * image, const RegionType& region);

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
    RegionType openGLBufferedRegion() { return m_OpenGlBufferedRegion; }
    void setOpenGLBufferedRegion(RegionType r) { m_OpenGlBufferedRegion = r; }

    //! setter getter, self explanatory
    RegionType extent() { return m_Extent; }

    /** Compute the linear buffer index according to the 2D region and
     * its 2D index.This method is used when OTB_GL_USE_ACCEL is OFF.
     * The resulting buffer will be flipped over the X axis.
     * \param index 2D index
     * \param region 2D region
     */
    static inline unsigned int ComputeXAxisFlippedBufferIndex(const IndexType& index, const RegionType& region)
    {
      return (region.GetSize()[1] - 1 + region.GetIndex()[1] -
              index[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
    }


    /** Add a GlComponent */
    unsigned int AddGlComponent(GlComponent * glComponent)
    {
        m_GlComponents->PushBack(glComponent);
        return m_GlComponents->Size() - 1;
    }

    /** Get the nth GlComponent */
    GlComponent * GetNthGlComponent(unsigned int index)
    {
        return m_GlComponents->GetNthElement(index);
    }

    /** Remove a GlComponent */
    void RemoveGlComponent(unsigned int index)
    {
        m_GlComponents->Erase(index);
    }

    /** Clear the GlComponent list */
    void ClearGlComponents()
    {
        m_GlComponents->Clear();
    }

    /** Get the number of GlComponent */
    unsigned int GetNumberOfGlComponents()
    {
        return m_GlComponents->Size();
    }

    //! set / get the MVC Controller
    ControllerType* controller() const { return m_pController; }
    void setController(ControllerType *control) { m_pController = control; }

protected:

    //! reimplemented method declared in the QGLWidget class
    void initializeGL();

    //! reimplemented method declared in the QGLWidget class
    void resizeGL(int w, int h);

    //! reimplemented method declared in the QGLWidget class
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
    RegionType m_OpenGlBufferedRegion;

    /** The display extent */
    RegionType m_Extent;

    /** If the image is subsampled with respect to the original image,
     * this indicates the subsampling rate */
    unsigned int m_SubsamplingRate;

    /** Controller */
    ControllerType::Pointer m_pController;

    /** Addtionnal Gl components */
    GlComponentListType::Pointer m_GlComponents;

};


#endif // QOTBIMAGEWIDGET_H
