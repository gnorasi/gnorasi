#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itiotbrgbaimageviewer.h"

#include "itiotbrgbaqglwidgetzoomable.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "../utils/itiotbimagemanager.h"
#include "../observables/itiviewerobservableregion.h"
#include "../../../ports/otbimageport.h"
#include <QMouseEvent>

using namespace otb;
using namespace itiviewer;

const double ZOOM_VALUE = 0.125;

ItiOtbRgbaQGLWidgetZoomable::ItiOtbRgbaQGLWidgetZoomable(QWidget *parent) :
    m_IsotropicZoom(1.0), m_OpenGlBuffer(NULL), m_OpenGlBufferedRegion(), m_Extent(), m_SubsamplingRate(1), m_first_displayed_col(0), m_first_displayed_row(0), m_nb_displayed_cols(0), m_nb_displayed_rows(0), QGLWidget(parent)
{

}

//!
void ItiOtbRgbaQGLWidgetZoomable::ReadBuffer(const RasterImageType *image, const RasterRegionType &region){
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

    //!
    setupViewport(width(),height());

    //!
    initializeColumnRowParameters();

    //!
    setupAndSendSignal();

    update();
}

//!
void ItiOtbRgbaQGLWidgetZoomable::ClearBuffer(){
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

    resizeGL(width(),height());

    update();
}


void ItiOtbRgbaQGLWidgetZoomable::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

//!
void ItiOtbRgbaQGLWidgetZoomable::resizeGL(int w, int h)
{
    setupViewport(w,h);

    setupAndSendSignal();

    if( m_Extent.GetIndex()[0] > 0 )
        m_nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
    else
        m_nb_displayed_cols = m_W / m_IsotropicZoom;

    if( m_Extent.GetIndex()[1] > 0 )
        m_nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
    else
        m_nb_displayed_rows = m_H / m_IsotropicZoom;
}

///!
void ItiOtbRgbaQGLWidgetZoomable::setupViewport(int w, int h){
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
void ItiOtbRgbaQGLWidgetZoomable::initializeColumnRowParameters(){
    m_first_displayed_col = 0;

    if( m_Extent.GetIndex()[0] > 0 )
    {
        m_nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
    }
    else
    {
        m_nb_displayed_cols = m_W / m_IsotropicZoom;
    }

    if( m_Extent.GetIndex()[1] > 0 )
    {
        m_nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
    }
    else
    {
        m_nb_displayed_rows = m_H / m_IsotropicZoom;
    }

    m_first_displayed_row = m_Extent.GetSize()[1] - m_nb_displayed_rows;
}

//!
void ItiOtbRgbaQGLWidgetZoomable::paintGL()
{

    RasterIndexType startPosition = m_Extent.GetIndex();
    startPosition[0] = startPosition[0] < 0 ? 0 : startPosition[0];
    startPosition[1] = startPosition[1] < 0 ? 0 : startPosition[1];

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_OpenGlBufferedRegion.GetSize()[0]);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, m_first_displayed_col);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,m_first_displayed_row);


    glClear(GL_COLOR_BUFFER_BIT);
    glPixelZoom(m_IsotropicZoom,m_IsotropicZoom);

    glRasterPos2f(startPosition[0], startPosition[1]);
    glDrawPixels(m_nb_displayed_cols,
                m_nb_displayed_rows,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                m_OpenGlBuffer);


    glFlush();
}

//!
void ItiOtbRgbaQGLWidgetZoomable::mouseMoveEvent(QMouseEvent *event){
    OTBImagePort *imgPort = (OTBImagePort*)ITIOTBIMAGEMANAGER->port();
    if(imgPort && imgPort->isConnected()){
        RasterImageType* imgType =  (RasterImageType*)imgPort->getData();
        if(!imgType){
            QGLWidget::mouseMoveEvent(event);
            return;
        }

        //! get the position
        QPoint point = event->pos();

        RasterIndexType index;
        if(m_Extent.GetIndex()[0] > 0)
            index[0] = qRound((double)point.x()/m_IsotropicZoom) + m_Extent.GetIndex()[0];
        else
            index[0] = m_first_displayed_col + qRound((double)point.x()/m_IsotropicZoom);

        if(m_Extent.GetIndex()[1] > 0)
            index[1] =  m_Extent.GetIndex()[1] + m_Extent.GetSize()[1] - qRound((double)point.y()/m_IsotropicZoom);
        else
            index[1] = m_Extent.GetSize()[1] - m_nb_displayed_rows - m_first_displayed_row + qRound((double)point.y()/m_IsotropicZoom);

        QString text = ItiOtbRgbaImageViewer::constructTextFromImageIndex(index,imgType);

        emit currentIndexChanged(text);
    }


    QGLWidget::mouseMoveEvent(event);
}

//!
void ItiOtbRgbaQGLWidgetZoomable::draw(){
    OTBImagePort *port = (OTBImagePort*)ITIOTBIMAGEMANAGER->port();

    if(!port)
        return;

    //!
    RasterImageType* imgType =  (RasterImageType*)port->getData();
    if(!imgType)
        return;

    //!
    RasterRegionType region = imgType->GetLargestPossibleRegion();

    //!
    ReadBuffer(imgType,region);

    //!
    setMouseTracking(true);
}

//!
void ItiOtbRgbaQGLWidgetZoomable::updateObserver(ItiViewerObservable *observable){

    ItiViewerObservableRegion *region = qobject_cast<ItiViewerObservableRegion*>(observable);
    if(!region)
        return;

}

//!
void ItiOtbRgbaQGLWidgetZoomable::setupAndSendSignal(){

    //!
    QRect rect;

    //!
    rect.setX(m_first_displayed_col);
    rect.setWidth(m_nb_displayed_cols);
    rect.setY(m_first_displayed_row);
    rect.setHeight(m_nb_displayed_rows);

    emit visibleAreaChanged(rect);
}

//!
void ItiOtbRgbaQGLWidgetZoomable::wheelEvent(QWheelEvent *event){
    if(event->delta() > 0)
        zoomIn();
    else
        zoomOut();

    //! accept the event
    event->accept();
}

//!
void ItiOtbRgbaQGLWidgetZoomable::zoomIn(){

    //! decrease the zoom level
    m_IsotropicZoom = m_IsotropicZoom + ZOOM_VALUE;

    //!
    setupViewport(width(),height());

    //! setup visualizing properties
    if( m_Extent.GetIndex()[0] > 0 )
    {
        m_nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
        m_first_displayed_col = 0;
    }
    else
    {
        m_nb_displayed_cols = qRound(m_W / m_IsotropicZoom);

        int helperX = m_first_displayed_col + qRound((m_nb_displayed_cols*ZOOM_VALUE)/2);

        if(helperX < 0)
            m_first_displayed_col = 0;
        else if(helperX > m_Extent.GetSize()[0] - m_nb_displayed_cols)
            m_first_displayed_col = m_Extent.GetSize()[0] - m_nb_displayed_cols;
        else
            m_first_displayed_col = helperX;
    }

    if( m_Extent.GetIndex()[1] > 0 )
    {
        m_nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
        m_first_displayed_row = 0;
    }
    else
    {
        m_nb_displayed_rows = qRound(m_H / m_IsotropicZoom);

        int helperY = m_first_displayed_row - qRound((m_nb_displayed_rows*ZOOM_VALUE)/2);

        if(helperY < 0)
            m_first_displayed_row = 0;
        else if(helperY > m_Extent.GetSize()[1] - m_nb_displayed_rows)
            m_first_displayed_row = m_Extent.GetSize()[1] - m_nb_displayed_rows;
        else
            m_first_displayed_row = helperY;
    }

    //!
    updateGL();

    //! setup and send signal
    setupAndSendSignal();
}

//!
void ItiOtbRgbaQGLWidgetZoomable::zoomOut(){

    //! decrease the zoom level
    m_IsotropicZoom = m_IsotropicZoom - ZOOM_VALUE;

    //! the minimum zoom level equals to 1
    if(m_IsotropicZoom < 1.0){
        m_IsotropicZoom = 1.0;
        return;
    }

    //!
    setupViewport(width(),height());

    //! setup visualizing properties
    if( m_Extent.GetIndex()[0] > 0 )
    {
        m_nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
        m_first_displayed_col = 0;
    }
    else
    {
        m_nb_displayed_cols = qRound(m_W / m_IsotropicZoom);

        int helperX = m_first_displayed_col - qRound((m_nb_displayed_cols*ZOOM_VALUE)/2);

        if(helperX < 0)
            m_first_displayed_col = 0;
        else if(helperX > m_Extent.GetSize()[0] - m_nb_displayed_cols)
            m_first_displayed_col = m_Extent.GetSize()[0] - m_nb_displayed_cols;
        else
            m_first_displayed_col = helperX;
    }

    if( m_Extent.GetIndex()[1] > 0 )
    {
        m_nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
        m_first_displayed_row = 0;
    }
    else
    {
        m_nb_displayed_rows = qRound(m_H / m_IsotropicZoom);

        int helperY = m_first_displayed_row + qRound((m_nb_displayed_rows*ZOOM_VALUE)/2);

        if(helperY < 0)
            m_first_displayed_row = 0;
        else if(helperY > m_Extent.GetSize()[1] - m_nb_displayed_rows)
            m_first_displayed_row = m_Extent.GetSize()[1] - m_nb_displayed_rows;
        else
            m_first_displayed_row = helperY;
    }

    //!
    updateGL();

    //! setup and send signal
    setupAndSendSignal();
}

/*!
 * \brief ItiOtbRgbaQGLWidgetZoomable::translate
 * \param rect
 */
void ItiOtbRgbaQGLWidgetZoomable::translate(int dx, int dy){

    //!
    int helperX = m_first_displayed_col + dx;
    if(helperX < 0)
        m_first_displayed_col = 0;
    else if(helperX > m_Extent.GetSize()[0] - m_nb_displayed_cols)
        m_first_displayed_col = m_Extent.GetSize()[0] - m_nb_displayed_cols;
    else
        m_first_displayed_col = helperX;

    //!
    int helperY = m_first_displayed_row - dy;
    if(helperY < 0)
        m_first_displayed_row = 0;
    else if(helperY > m_Extent.GetSize()[1] - m_nb_displayed_rows)
        m_first_displayed_row = m_Extent.GetSize()[1] - m_nb_displayed_rows;
    else
        m_first_displayed_row = helperY;

    //!
    qDebug() << " m_first_displayed_col : " << m_first_displayed_col << ", m_first_displayed_row : " << m_first_displayed_row;

    //!
    updateGL();
}

//!
ItiOtbRgbaQGLWidgetZoomable::~ItiOtbRgbaQGLWidgetZoomable(){
    ClearBuffer();
}
