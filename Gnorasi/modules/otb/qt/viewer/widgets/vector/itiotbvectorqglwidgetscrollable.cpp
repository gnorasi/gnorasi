#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itiotbvectorimageviewer.h"

#include "itiotbvectorqglwidgetscrollable.h"
#include "../../utils/itiotbimagemanager.h"
#include "../../observables/itiviewerobservableregion.h"
#include "../../../../ports/otbvectorimageport.h"

#include "itkImageRegionConstIteratorWithIndex.h"

using namespace otb;
using namespace itiviewer;
using namespace voreen;

ItiOtbVectorQGLWidgetScrollable::ItiOtbVectorQGLWidgetScrollable(QWidget *parent) :
    m_IsotropicZoom(1.0), m_OpenGlBuffer(NULL), m_OpenGlBufferedRegion(), m_Extent(), m_SubsamplingRate(1), QGLWidget(parent)
{
    setAutoFillBackground(false);

    m_pen = QPen(QBrush(Qt::red),2.0);
}

//!
void ItiOtbVectorQGLWidgetScrollable::ReadBuffer(const VectorImageType *image, const VectorRegionType &region){
    // Before doing anything, check if region is inside the buffered
    // region of image
    if (!image->GetBufferedRegion().IsInside(region))
      {
        qDebug() << "Region to read is oustside of the buffered region.";
      }
    // Delete previous buffer if needed
    this->ClearBuffer();

    // Allocate new memory
    m_OpenGlBuffer = new unsigned char[3 * region.GetNumberOfPixels()];

    // Declare the iterator
    itk::ImageRegionConstIteratorWithIndex<VectorImageType> it(image, region);

    // Go to begin
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        // Fill the buffer
        unsigned int index = 0;

        // compute the linear index (buffer is flipped around X axis
        // when gl acceleration is disabled
        index = ItiOtbVectorImageViewer::ComputeXAxisFlippedBufferIndex(it.GetIndex(), region);

        // Fill the buffer
//        m_OpenGlBuffer[index]       = it.Get()[0];
//        m_OpenGlBuffer[index + 1]   = it.Get()[1];
//        m_OpenGlBuffer[index + 2]   = it.Get()[2];
        ++it;
    }

    //! Last, updating buffer size
    m_OpenGlBufferedRegion = region;

    update();
}

//!
void ItiOtbVectorQGLWidgetScrollable::ClearBuffer(){
    // Delete previous buffer if needed
    if (m_OpenGlBuffer != NULL)
    {
        delete[] m_OpenGlBuffer;
        m_OpenGlBuffer = NULL;
    }

    VectorRegionType region;
    VectorIndexType index;
    VectorSizeType  size;

    size.Fill(0);
    index.Fill(0);
    region.SetIndex(index);
    region.SetSize(size);

    // Last, updating buffer size
    m_OpenGlBufferedRegion = region;

    update();
}

/*!
 * \brief ItiOtbVectorQGLWidgetScrollable::initializeGL
 */
void ItiOtbVectorQGLWidgetScrollable::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}


void ItiOtbVectorQGLWidgetScrollable::resizeGL(int w, int h)
{
    //! firstly setup the viweport with the new width and height
    setupViewport(w,h);

    //! create a rect
    QRect rect;

    rect.setX(m_Extent.GetIndex()[0]);
    rect.setY(m_Extent.GetIndex()[1]);

    //! create helper integer values
    int wt = qMin(static_cast<int>(w),static_cast<int>(m_OpenGlBufferedRegion.GetSize()[0]));
    int ht = qMin(static_cast<int>(h),static_cast<int>(m_OpenGlBufferedRegion.GetSize()[1]));

    //! set the helper values
    rect.setWidth(wt);
    rect.setHeight(ht);

    //! emit the signal
    emit visibleAreaChanged(rect);
}

//!
void ItiOtbVectorQGLWidgetScrollable::setupViewport(int w, int h){
    VectorSizeType size;
    size [0] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[0]));
    size [1] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[1]));

    VectorRegionType::IndexType index;
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
void ItiOtbVectorQGLWidgetScrollable::paintEvent(QPaintEvent *event){
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


    VectorIndexType startPosition = m_Extent.GetIndex();
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

//    DebugOpenGL();

    //! overpainting
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(m_pen);
    painter.drawRect(m_focusRegion);

    painter.end();
}

//!
void ItiOtbVectorQGLWidgetScrollable::draw(){
    //! get the current port from the manager
    OTBVectorImagePort *port = (OTBVectorImagePort*)ITIOTBIMAGEMANAGER->port();

    if(!port)
        return;

    //!
    VectorImageType* imgType =  (VectorImageType*)port->getData();
    if(!imgType)
        return;

    //! get the biggest available region
    VectorRegionType region = imgType->GetLargestPossibleRegion();

    //! read the buffer
    ReadBuffer(imgType,region);

    //! mouse tracking is disabled on startup, set it on
    setMouseTracking(true);
}

//! The observer gets notified on a zoomable view's change ( either zooming or resizing events)
void ItiOtbVectorQGLWidgetScrollable::updateObserver(ItiViewerObservable *observable){
    //!
    if(!m_OpenGlBuffer)
        return;

    ItiViewerObservableRegion *region = qobject_cast<ItiViewerObservableRegion*>(observable);
    if(!region)
        return;

    //! get the current rect from the region
    QRect rregion = region->region();

    //! create helper values from the rect's parameters
    int x       = rregion.x();
    int y       = rregion.y();
    int width   = rregion.width();
    int height  = rregion.height();

    m_focusRegion.setX(m_Extent.GetIndex()[0] + x);
    //!
    //! Take notice that the y-axes is inverted [1,-1]. That's why the focus resion's y value equals to
    //! the following calculated value
    //!
    m_focusRegion.setY(m_Extent.GetIndex()[1] + m_Extent.GetSize()[1] - height - y);
    m_focusRegion.setWidth(width);
    m_focusRegion.setHeight(height);

    //! finally update the view
    update();
}

//!
void ItiOtbVectorQGLWidgetScrollable::wheelEvent(QWheelEvent *event){
    double deltaval = (double)event->delta();

    //! check the delta value if is more than zero, if yes zoomin, else zoomout
    if(deltaval>0)
        emit zoomIn();
    else
        emit zoomOut();

    event->accept();
}

//!
void ItiOtbVectorQGLWidgetScrollable::mousePressEvent(QMouseEvent *event){

    //! setup translating functionality only on left button pressed mouse events
    if(event->button() == Qt::LeftButton){
        //! create a helper point value
        QPoint previousCenter = m_focusRegion.center();

        //! get the position of the event
        QPoint point = event->pos();

        //! create a helper line object
        QLine line(previousCenter,point);

        //! create helper values
        int dx = 0, dy = 0;

        //! check if the new rect right border exceeds the extends' width value
        if(point.x()+ qRound((double)m_focusRegion.width()/2.0) > m_Extent.GetSize()[0] + m_Extent.GetIndex()[0]){
            dx = m_Extent.GetIndex()[0] + m_Extent.GetSize()[0] - qRound((double)m_focusRegion.width()/2.0) - previousCenter.x();
        } //! check if the new rect left border exceeds the extend's index x value
        else if(point.x()- qRound((double)m_focusRegion.width()/2.0) < m_Extent.GetIndex()[0]){
            dx = m_Extent.GetIndex()[0] + qRound((double)m_focusRegion.width()/2.0) - previousCenter.x();
        }else //! else set the dx value equal to the line dx value
            dx = line.dx();

        //! check if the new rect right border exceeds the extends' height value
        if(point.y()+ qRound((double)m_focusRegion.height()/2.0) > m_Extent.GetSize()[1] + m_Extent.GetIndex()[1]){
            dy = m_Extent.GetSize()[1] + m_Extent.GetIndex()[1] - qRound((double)m_focusRegion.height()/2.0) - previousCenter.y();
        } //! check if the new rect left border exceeds the extend's index y value
        else if(point.y()- qRound((double)m_focusRegion.height()/2.0) < m_Extent.GetIndex()[1]){
            dy = m_Extent.GetIndex()[1] + qRound((double)m_focusRegion.height()/2.0) - previousCenter.y();
        }else //! else set the dy value equal to the line dy value
            dy = line.dy();

        //!translate the focus region
        m_focusRegion.translate(dx,dy);

        //! emit signal in order to update the zoomable view
        emit focusRegionTranslated(dx,dy);

        //! update widget
        update();
    }

    QGLWidget::mousePressEvent(event);
}

//!
void ItiOtbVectorQGLWidgetScrollable::mouseMoveEvent(QMouseEvent *event){
    //! get the current image port from the manager
    OTBVectorImagePort *imgPort = (OTBVectorImagePort*)ITIOTBIMAGEMANAGER->port();

    //! check if the image port is valid and connected with other ports
    if(imgPort && imgPort->isConnected()){
        //! get the image from the port
        VectorImageType* imgType =  (VectorImageType*)imgPort->getData();
        if(!imgType){
            QGLWidget::mouseMoveEvent(event);
            return;
        }

        //! get the position
        QPoint point = event->pos();

        VectorIndexType index;
        index[0] = (point.x() - m_Extent.GetIndex()[0])/m_IsotropicZoom;
        index[1] = (point.y() - m_Extent.GetIndex()[1])/m_IsotropicZoom;

        //! construct a text value related to pixel info data
        QString text = ItiOtbVectorImageViewer::constructTextFromImageIndex(index,imgType);

        //! emit the signal
        emit currentIndexChanged(text);
    }

    //! call the parent's class method
    QGLWidget::mouseMoveEvent(event);
}

//!
ItiOtbVectorQGLWidgetScrollable::~ItiOtbVectorQGLWidgetScrollable(){
    ClearBuffer();
}
