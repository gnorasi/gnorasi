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


//
// Monteverdi includes (sorted by alphabetic order)
#include "../../models/itiotbAbstractImageModel.h"
//#include "mvdApplication.h"
#include "../../utils/itiotbImageModelRenderer.h"
#include "../../utils/itiotbImageViewManipulator.h"

using namespace otb;
using namespace itiviewer;

const double ZOOM_VALUE = 0.125;

ItiOtbVectorQGLWidgetZoomable::ItiOtbVectorQGLWidgetZoomable(QWidget *parent) :
    m_IsotropicZoom(1.0),
    m_SubsamplingRate(1),
    m_ImageViewManipulator( NULL ),
    m_ImageModelRenderer( NULL ),
    QGLWidget(parent)
{
    m_ImageViewManipulator = new ImageViewManipulator( this );
    m_ImageModelRenderer   = new ImageModelRenderer( this );
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
      m_ImageViewManipulator->GetViewportImageRegion() );

    ItiOtbVectorImageViewer *viewer = qobject_cast<ItiOtbVectorImageViewer*>(parent());
    if(!viewer)
        return;

    // Set the new rendering context to be known in the ModelRendere
    const AbstractImageModel* aiModel=  qobject_cast<AbstractImageModel*>(viewer->model());

    if(!aiModel)
        return;

    // setup the rendering context
    if (aiModel)
    {
      ImageModelRenderer::RenderingContext context(aiModel, region, this->width(), this->height());

      // use the model renderer to paint the requested region of the image
      m_ImageModelRenderer->paintGL( context );
    }
}

//!
void ItiOtbVectorQGLWidgetZoomable::mouseMoveEvent(QMouseEvent *event){
    QCursor dragCursor;
    dragCursor.setShape(Qt::ClosedHandCursor) ;
    this->setCursor(dragCursor);

    m_ImageViewManipulator->mouseMoveEvent(event);


    QGLWidget::mouseMoveEvent(event);
}

//!
void ItiOtbVectorQGLWidgetZoomable::draw(){

    //! mouse tracking is disabled on startup, set it on
    setMouseTracking(true);
}

//!
void ItiOtbVectorQGLWidgetZoomable::updateObserver(ItiViewerObservable *observable){

    Q_UNUSED(observable);
}

//!
void ItiOtbVectorQGLWidgetZoomable::wheelEvent(QWheelEvent *event){

}


void ItiOtbVectorQGLWidgetZoomable::onLargestPossibleRegionChanged(const ImageRegionType& largestRegion)
{
  m_ImageViewManipulator->SetImageLargestRegion(largestRegion);
}

//!
ItiOtbVectorQGLWidgetZoomable::~ItiOtbVectorQGLWidgetZoomable(){
//    ClearBuffer();
}
