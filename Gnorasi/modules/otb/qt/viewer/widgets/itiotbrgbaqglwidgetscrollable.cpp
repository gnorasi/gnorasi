#include "itiotbrgbaqglwidgetscrollable.h"
#include "../utils/itiotbimagemanager.h"
#include "../observables/itiviewerobservableregion.h"

#include "itkImageRegionConstIteratorWithIndex.h"

#include <QDebug>

using namespace otb;
using namespace itiviewer;

ItiOtbRgbaQGLWidgetScrollable::ItiOtbRgbaQGLWidgetScrollable(QWidget *parent) :
    m_IsotropicZoom(1.0), m_OpenGlBuffer(NULL), m_OpenGlBufferedRegion(), m_Extent(), m_SubsamplingRate(1), QGLWidget(parent)
{
    setAutoFillBackground(false);

    m_pen = QPen(QBrush(Qt::red),2.0);
}

//!
void ItiOtbRgbaQGLWidgetScrollable::ReadBuffer(const RasterImageType *image, const RasterRegionType &region){
    // Before doing anything, check if region is inside the buffered
    // region of image
    if (!image->GetBufferedRegion().IsInside(region))
      {
//      itkExceptionMacro(<< "Region to read is oustside of the buffered region.");
      }
    // Delete previous buffer if needed
    this->ClearBuffer();

    // Allocate new memory
    m_OpenGlBuffer = new unsigned char[3 * region.GetNumberOfPixels()];

    // Declare the iterator
    itk::ImageRegionConstIteratorWithIndex<RasterImageType> it(image, region);

    // Go to begin
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        // Fill the buffer
        unsigned int index = 0;

        // compute the linear index (buffer is flipped around X axis
        // when gl acceleration is disabled
        index = ComputeXAxisFlippedBufferIndex(it.GetIndex(), region);

        // Fill the buffer
        m_OpenGlBuffer[index]  = it.Get()[0];
        m_OpenGlBuffer[index + 1] = it.Get()[1];
        m_OpenGlBuffer[index + 2] = it.Get()[2];
        ++it;
    }

    //! Last, updating buffer size
    m_OpenGlBufferedRegion = region;

    update();
}

//!
void ItiOtbRgbaQGLWidgetScrollable::ClearBuffer(){
    // Delete previous buffer if needed
    if (m_OpenGlBuffer != NULL)
    {
        delete[] m_OpenGlBuffer;
        m_OpenGlBuffer = NULL;
    }

    RasterRegionType region;
    RasterIndexType index;
    RasterSizeType  size;

    size.Fill(0);
    index.Fill(0);
    region.SetIndex(index);
    region.SetSize(size);

    // Last, updating buffer size
    m_OpenGlBufferedRegion = region;

    update();
}


void ItiOtbRgbaQGLWidgetScrollable::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}


void ItiOtbRgbaQGLWidgetScrollable::resizeGL(int w, int h)
{
    setupViewport(w,h);

    QRect rect;
    rect.setX(m_Extent.GetIndex()[0]);
    rect.setY(m_Extent.GetIndex()[1]);

    int wt = qMin(static_cast<int>(w),static_cast<int>(m_OpenGlBufferedRegion.GetSize()[0]));
    int ht = qMin(static_cast<int>(h),static_cast<int>(m_OpenGlBufferedRegion.GetSize()[1]));
    rect.setWidth(wt);
    rect.setHeight(ht);


    emit visibleAreaChanged(rect);
}

void ItiOtbRgbaQGLWidgetScrollable::setupViewport(int w, int h){
    RasterSizeType size;
    size [0] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[0]));
    size [1] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[1]));

    RasterRegionType::IndexType index;
    index[0] = (w - static_cast<int>(size[0])) / 2;
    index[1] = (h - static_cast<int>(size[1])) / 2;

    m_Extent.SetIndex(index);
    m_Extent.SetSize(size);

    m_W = (GLint)w;
    m_H = (GLint)h;

    glViewport(0, 0, m_W, m_H);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_W, 0, m_H, -1, 1);
}

//!
void ItiOtbRgbaQGLWidgetScrollable::paintEvent(QPaintEvent *event){
    //!
    makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    unsigned int nb_displayed_rows;
    unsigned int nb_displayed_cols;
    unsigned int first_displayed_row;
    unsigned int first_displayed_col;

    if( m_Extent.GetIndex()[0] >= 0 )
    {
        nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
        first_displayed_col = 0;
    }
    else
    {
        nb_displayed_cols = m_W / m_IsotropicZoom;
        first_displayed_col = (m_OpenGlBufferedRegion.GetSize()[0] - nb_displayed_cols) / 2;
    }

    if( m_Extent.GetIndex()[1] >= 0 )
    {
        nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
        first_displayed_row = 0;
    }
    else
    {
        nb_displayed_rows = m_H / m_IsotropicZoom;
        first_displayed_row = (m_OpenGlBufferedRegion.GetSize()[1] - nb_displayed_rows) / 2;
    }


    RasterIndexType startPosition = m_Extent.GetIndex();
    startPosition[0] = startPosition[0] < 0 ? 0 : startPosition[0];
    startPosition[1] = startPosition[1] < 0 ? 0 : startPosition[1];

    qglClearColor(Qt::black);
//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_MULTISAMPLE);

    setupViewport(width(), height());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_OpenGlBufferedRegion.GetSize()[0]);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, first_displayed_col);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,first_displayed_row);

    glClear(GL_COLOR_BUFFER_BIT);
    glPixelZoom(m_IsotropicZoom,m_IsotropicZoom);

    glRasterPos2f(startPosition[0], startPosition[1]);
    glDrawPixels(nb_displayed_cols,
             nb_displayed_rows,
             GL_RGB,
             GL_UNSIGNED_BYTE,
             m_OpenGlBuffer);

    glFlush();

    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    //! overpainting
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(m_pen);
    painter.drawRect(m_focusRegion);

    painter.end();

}

//!
void ItiOtbRgbaQGLWidgetScrollable::draw(){
    OTBImagePort *port = (OTBImagePort*)ITIOTBIMAGEMANAGER->port();

    if(!port)
        return;

    //!
    RasterImageType* imgType =  (RasterImageType*)port->getData();
    if(!imgType)
        return;

    RasterRegionType region = imgType->GetLargestPossibleRegion();

    //!
    ReadBuffer(imgType,region);
}

//!
void ItiOtbRgbaQGLWidgetScrollable::updateObserver(ItiViewerObservable *observable){
    ItiViewerObservableRegion *region = qobject_cast<ItiViewerObservableRegion*>(observable);
    if(!region)
        return;

    QRect rregion = region->region();

    QRect rect;
    rect.setX(rregion.x()+qAbs(m_Extent.GetIndex()[0]));
    rect.setY(rregion.y()+qAbs(m_Extent.GetIndex()[1]));
    rect.setWidth(rregion.width());
    rect.setHeight(rregion.height());

    m_focusRegion = rect;

    update();
}

//!
void ItiOtbRgbaQGLWidgetScrollable::wheelEvent(QWheelEvent *event){
    double deltaval = (double)event->delta();

    if(deltaval>0)
        emit zoomIn();
    else
        emit zoomOut();

    event->accept();
}

//!
void ItiOtbRgbaQGLWidgetScrollable::mousePressEvent(QMouseEvent *event){

    if(event->button() == Qt::LeftButton){
        QPoint previousCenter = m_focusRegion.center();
        QPoint point = event->pos();
        QLine line(previousCenter,point);
        int dx = 0, dy = 0;

        if(point.x()+ m_focusRegion.width()/2 > m_Extent.GetSize()[0] + m_Extent.GetIndex()[0]){
            dx = m_Extent.GetSize()[0] + m_Extent.GetIndex()[0] - m_focusRegion.width()/2 - previousCenter.x();
        }else if(point.x()-m_focusRegion.width()/2 < m_Extent.GetIndex()[0]){
            dx = m_Extent.GetIndex()[0] + m_focusRegion.width()/2 - previousCenter.x();
        }else
            dx = line.dx();

        if(point.y()+ m_focusRegion.height()/2 > m_Extent.GetSize()[1] + m_Extent.GetIndex()[1]){
            dy = m_Extent.GetSize()[1] + m_Extent.GetIndex()[1] - m_focusRegion.height()/2 - previousCenter.y();
        }else if(point.y()-m_focusRegion.height()/2 < m_Extent.GetIndex()[1]){
            dy = m_Extent.GetIndex()[1] + m_focusRegion.height()/2 - previousCenter.y();
        }else
            dy = line.dy();

        m_focusRegion.translate(dx,dy);

        emit focusRegionTranslated(dx,dy);

        update();
    }

    QGLWidget::mousePressEvent(event);
}

//!
ItiOtbRgbaQGLWidgetScrollable::~ItiOtbRgbaQGLWidgetScrollable(){
    ClearBuffer();
}
