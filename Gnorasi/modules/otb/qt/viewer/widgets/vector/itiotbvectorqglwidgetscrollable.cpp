#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itiotbvectorimageviewer.h"

#include "itiotbvectorqglwidgetscrollable.h"
#include "../../utils/itiotbimagemanager.h"
#include "../../observables/itiviewerobservableregion.h"
#include "../../../../ports/otbvectorimageport.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "otbVectorRescaleIntensityImageFilter.h"


#include "../../utils/itiotbImageModelRendererAlt.h"
#include "../../utils/itiotbImageViewManipulator.h"

#include "../../models/itiotbVectorImageModel.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "../../models/itiotbAbstractImageModel.h"


using namespace otb;
using namespace itiviewer;
using namespace voreen;

ItiOtbVectorQGLWidgetScrollable::ItiOtbVectorQGLWidgetScrollable(ItiOtbVectorImageViewer *parent) :
    m_IsotropicZoom(1.0),
    m_SubsamplingRate(1),
    m_pImageViewManipulator( NULL ),
    m_pImageModelRenderer( NULL ),
    m_pItiOtbVectorImageViewer(parent),
    QGLWidget(parent)
{
    setAutoFillBackground(false);

    m_pImageViewManipulator = new ImageViewManipulator( this );
    m_pImageModelRenderer   = new ImageModelRendererAlt( this );

    m_pen = QPen(QBrush(Qt::red),2.0);
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

//    //! create a rect
//    QRect rect;

//    rect.setX(m_Extent.GetIndex()[0]);
//    rect.setY(m_Extent.GetIndex()[1]);

//    //! create helper integer values
//    int wt = qMin(static_cast<int>(w),static_cast<int>(m_OpenGlBufferedRegion.GetSize()[0]));
//    int ht = qMin(static_cast<int>(h),static_cast<int>(m_OpenGlBufferedRegion.GetSize()[1]));

//    //! set the helper values
//    rect.setWidth(wt);
//    rect.setHeight(ht);

//    //! emit the signal
//    emit visibleAreaChanged(rect);
}

//!
void ItiOtbVectorQGLWidgetScrollable::setupViewport(int w, int h){
//    VectorSizeType size;
//    size [0] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[0]));
//    size [1] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[1]));

//    VectorRegionType::IndexType index;
//    index[0] = (w - static_cast<int>(size[0])) / 2;
//    index[1] = (h - static_cast<int>(size[1])) / 2;

//    m_Extent.SetIndex(index);
//    m_Extent.SetSize(size);

//    m_W = (GLint)w;
//    m_H = (GLint)h;

//    m_pImageViewManipulator->InitializeContext(w,h);

    glViewport(0, 0, (GLint)w, (GLint)h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLint)w, 0, (GLint)h, -1, 1);
}

//!
void ItiOtbVectorQGLWidgetScrollable::paintEvent(QPaintEvent *event){
    //!
    makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    //
    // TEST

    // Get the region to draw from the ImageViewManipulator navigation
    // context
    const ImageRegionType region(
      m_pImageViewManipulator->GetViewportImageRegion() );

    // Set the new rendering context to be known in the ModelRendere
    const AbstractImageModel* aiModel=  qobject_cast<AbstractImageModel*>(m_pItiOtbVectorImageViewer->model());

    if(!aiModel)
        return;

    // setup the rendering context
    if (aiModel)
    {
      ImageModelRendererAlt::RenderingContext context(aiModel, region, this->width(), this->height());

      // use the model renderer to paint the requested region of the image
      m_pImageModelRenderer->paintGL( context );
    }


    //
    // END OF TEST

    //! overpainting
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(m_pen);
    painter.drawRect(m_focusRegion);

    painter.end();
}

//!
void ItiOtbVectorQGLWidgetScrollable::draw(){

    // Set the new rendering context to be known in the ModelRendere
    const VectorImageModel* vModel=  qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());

    if(!vModel)
        return;

    m_pImageViewManipulator->InitializeContext(this->width(),this->height());

    m_pImageViewManipulator->SetImageLargestRegion(vModel->GetLargestPossibleRegion());

    //! mouse tracking is disabled on startup, set it on
    setMouseTracking(true);
}

//! The observer gets notified on a zoomable view's change ( either zooming or resizing events)
void ItiOtbVectorQGLWidgetScrollable::updateObserver(ItiViewerObservable *observable){
    //!
//    if(!m_OpenGlBuffer)
//        return;

//    ItiViewerObservableRegion *region = qobject_cast<ItiViewerObservableRegion*>(observable);
//    if(!region)
//        return;

//    //! get the current rect from the region
//    QRect rregion = region->region();

//    //! create helper values from the rect's parameters
//    int x       = rregion.x();
//    int y       = rregion.y();
//    int width   = rregion.width();
//    int height  = rregion.height();

//    m_focusRegion.setX(m_Extent.GetIndex()[0] + x);
//    //!
//    //! Take notice that the y-axes is inverted [1,-1]. That's why the focus resion's y value equals to
//    //! the following calculated value
//    //!
//    m_focusRegion.setY(m_Extent.GetIndex()[1] + m_Extent.GetSize()[1] - height - y);
//    m_focusRegion.setWidth(width);
//    m_focusRegion.setHeight(height);

    //! finally update the view
    update();
}

//!
void ItiOtbVectorQGLWidgetScrollable::wheelEvent(QWheelEvent *event){
    double deltaval = (double)event->delta();

    //! check the delta value if is more than zero, if yes zoomin, else zoomout
//    if(deltaval>0)
//        emit zoomIn();
//    else
//        emit zoomOut();

    event->accept();
}

//!
void ItiOtbVectorQGLWidgetScrollable::mousePressEvent(QMouseEvent *event){

    //! setup translating functionality only on left button pressed mouse events
    if(event->button() == Qt::LeftButton){
//        //! create a helper point value
//        QPoint previousCenter = m_focusRegion.center();

//        //! get the position of the event
//        QPoint point = event->pos();

//        //! create a helper line object
//        QLine line(previousCenter,point);

//        //! create helper values
//        int dx = 0, dy = 0;

//        //! check if the new rect right border exceeds the extends' width value
//        if(point.x()+ qRound((double)m_focusRegion.width()/2.0) > m_Extent.GetSize()[0] + m_Extent.GetIndex()[0]){
//            dx = m_Extent.GetIndex()[0] + m_Extent.GetSize()[0] - qRound((double)m_focusRegion.width()/2.0) - previousCenter.x();
//        } //! check if the new rect left border exceeds the extend's index x value
//        else if(point.x()- qRound((double)m_focusRegion.width()/2.0) < m_Extent.GetIndex()[0]){
//            dx = m_Extent.GetIndex()[0] + qRound((double)m_focusRegion.width()/2.0) - previousCenter.x();
//        }else //! else set the dx value equal to the line dx value
//            dx = line.dx();

//        //! check if the new rect right border exceeds the extends' height value
//        if(point.y()+ qRound((double)m_focusRegion.height()/2.0) > m_Extent.GetSize()[1] + m_Extent.GetIndex()[1]){
//            dy = m_Extent.GetSize()[1] + m_Extent.GetIndex()[1] - qRound((double)m_focusRegion.height()/2.0) - previousCenter.y();
//        } //! check if the new rect left border exceeds the extend's index y value
//        else if(point.y()- qRound((double)m_focusRegion.height()/2.0) < m_Extent.GetIndex()[1]){
//            dy = m_Extent.GetIndex()[1] + qRound((double)m_focusRegion.height()/2.0) - previousCenter.y();
//        }else //! else set the dy value equal to the line dy value
//            dy = line.dy();

//        //!translate the focus region
//        m_focusRegion.translate(dx,dy);

//        //! emit signal in order to update the zoomable view
//        emit focusRegionTranslated(dx,dy);

//        //! update widget
//        update();
    }

    QGLWidget::mousePressEvent(event);
}

//!
void ItiOtbVectorQGLWidgetScrollable::mouseMoveEvent(QMouseEvent *event){
    //! get the current image port from the manager
    Port *port = ITIOTBIMAGEMANAGER->port();

    //! check if the image port is valid and connected with other ports
    if(port && port->isConnected()){
        //! get the image from the port
//        VectorImageType* imgType =  (VectorImageType*)ITIOTBIMAGEMANAGER->image();
//        if(!imgType){
//            QGLWidget::mouseMoveEvent(event);
//            return;
//        }

//        //! get the position
//        QPoint point = event->pos();

//        VectorIndexType index;
//        index[0] = (point.x() - m_Extent.GetIndex()[0])/m_IsotropicZoom;
//        index[1] = (point.y() - m_Extent.GetIndex()[1])/m_IsotropicZoom;

//        //! construct a text value related to pixel info data
//        QString text = ItiOtbVectorImageViewer::constructTextFromImageIndex(index,imgType);

//        //! emit the signal
//        emit currentIndexChanged(text);
    }

    //! call the parent's class method
    QGLWidget::mouseMoveEvent(event);
}


//void ItiOtbVectorQGLWidgetScrollable::resizeEvent(QResizeEvent *event){
//    m_pImageViewManipulator->resizeEvent(event);
//}

//!
ItiOtbVectorQGLWidgetScrollable::~ItiOtbVectorQGLWidgetScrollable(){
//    ClearBuffer();
}
