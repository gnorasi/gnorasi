#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itiotbvectorimageviewer.h"

#include "itiotbvectorqglwidgetzoomable.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "../../utils/itiotbimagemanager.h"
#include "../../observables/itiviewerobservableregion.h"
#include "../../../../ports/otbvectorimageport.h"


#include "../../models/itiotbVectorImageModel.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "../../models/itiotbAbstractImageModel.h"

#include "../../utils/itiotbImageModelRenderer.h"
#include "../../utils/itiotbImageViewManipulator.h"

using namespace otb;
using namespace itiviewer;

const double ZOOM_VALUE = 0.125;

ItiOtbVectorQGLWidgetZoomable::ItiOtbVectorQGLWidgetZoomable(ItiOtbVectorImageViewer *parent) :
    m_IsotropicZoom(1.0),
    m_SubsamplingRate(1),
    m_pImageViewManipulator( NULL ),
    m_pImageModelRenderer( NULL ),
    m_pItiOtbVectorImageViewer(parent),
    QGLWidget(parent)
{
    m_pImageViewManipulator = new ImageViewManipulator( this );
    m_pImageModelRenderer   = new ImageModelRenderer( this );
}


void ItiOtbVectorQGLWidgetZoomable::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

//!
void ItiOtbVectorQGLWidgetZoomable::resizeGL(int width, int height)
{
    // TODO: Replace (GLint) casts with safer casts or no cast (if there is no compile-time warning).
    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLint)width, 0, (GLint)height, -1, 1);
}

//!
void ItiOtbVectorQGLWidgetZoomable::paintGL(){

    // Clear back-buffer(s) before rendering sub-components.
    glClear( GL_COLOR_BUFFER_BIT );

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
      ImageModelRenderer::RenderingContext context(aiModel, region, this->width(), this->height());

      // use the model renderer to paint the requested region of the image
      m_pImageModelRenderer->paintGL( context );
    }
}

//!
void ItiOtbVectorQGLWidgetZoomable::mouseMoveEvent(QMouseEvent *event){
    QCursor dragCursor;
    dragCursor.setShape(Qt::ClosedHandCursor) ;
    this->setCursor(dragCursor);

    m_pImageViewManipulator->mouseMoveEvent(event);


    QGLWidget::mouseMoveEvent(event);
}

//!
void ItiOtbVectorQGLWidgetZoomable::draw(){

    //! mouse tracking is disabled on startup, set it on
    setMouseTracking(true);

    // Set the new rendering context to be known in the ModelRendere
    const VectorImageModel* vModel=  qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());

    if(!vModel)
        return;

    m_pImageViewManipulator->InitializeContext(width(),height());

    m_pImageViewManipulator->SetImageLargestRegion(vModel->GetLargestPossibleRegion());
}

//!
void ItiOtbVectorQGLWidgetZoomable::updateObserver(ItiViewerObservable *observable){

    Q_UNUSED(observable);
}

//!
void ItiOtbVectorQGLWidgetZoomable::wheelEvent(QWheelEvent *event){

}


void ItiOtbVectorQGLWidgetZoomable::resizeEvent(QResizeEvent *event){
    m_pImageViewManipulator->resizeEvent(event);
}


void ItiOtbVectorQGLWidgetZoomable::onLargestPossibleRegionChanged(const ImageRegionType& largestRegion)
{
  m_pImageViewManipulator->SetImageLargestRegion(largestRegion);
}

//!
ItiOtbVectorQGLWidgetZoomable::~ItiOtbVectorQGLWidgetZoomable(){
//    ClearBuffer();
}
