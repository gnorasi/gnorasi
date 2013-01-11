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

#include "../../utils/itiotbImageModelRendererZoomable.h"
#include "../../utils/itiotbImageViewManipulatorZoomable.h"

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
    m_pImageViewManipulator = new ImageViewManipulatorZoomable( this );
    m_pImageModelRenderer   = new ImageModelRendererZoomable( this );
}


void ItiOtbVectorQGLWidgetZoomable::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

///!
void ItiOtbVectorQGLWidgetZoomable::setupViewport(int w, int h){
    ImageRegionType extent;
    ImageRegionType bufferRegion = m_pImageViewManipulator->modelRegion();
    ImageRegionType::SizeType size;

    size [0] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(bufferRegion.GetSize()[0]));
    size [1] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(bufferRegion.GetSize()[1]));

    ImageRegionType::IndexType index;
    index[0] = (w - static_cast<int>(size[0])) / 2;
    index[1] = (h - static_cast<int>(size[1])) / 2;

    extent.SetIndex(index);
    extent.SetSize(size);

    m_pImageViewManipulator->setExtent(extent);

    // initialize with the given size
    if(w && h)
        m_pImageViewManipulator->InitializeContext(w,h);
    else
        m_pImageViewManipulator->InitializeContext(100,100);

    glViewport(0, 0, (GLint)w, (GLint)h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLint)w, 0, (GLint)h, -1, 1);
}


//!
void ItiOtbVectorQGLWidgetZoomable::paintGL(){

    // Clear back-buffer(s) before rendering sub-components.
    glClear( GL_COLOR_BUFFER_BIT );


    // Set the new rendering context to be known in the ModelRendere
    const AbstractImageModel* aiModel=  qobject_cast<AbstractImageModel*>(m_pItiOtbVectorImageViewer->model());

    // setup the rendering context
    if (aiModel)
    {
        // Get the region to draw from the ImageViewManipulator navigation
        // context
        const ImageRegionType region(m_pImageViewManipulator->modelRegion() );

        const ImageRegionType extent(m_pImageViewManipulator->extent());

        ImageModelRendererZoomable::RenderingContext context(aiModel, region, extent, this->width(), this->height());

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

    setupViewport(event->size().width(), event->size().height());

    unsigned int nb_d_cs = 0, nb_d_rs = 0, f_d_c = 0, f_d_r = 0;

    ImageRegionType extent          = m_pImageViewManipulator->extent();
    ImageRegionType bufferedRegion  = m_pImageViewManipulator->modelRegion();

    //!
    //! setup the number of rows and columns to be visualized
    //!
    //! check if the extend's index x value is greater than zero , if yes then the number of columns equals to the number of the buffered region
    if( extent.GetIndex()[0] > 0 ){
        nb_d_cs = bufferedRegion.GetSize()[0];
        f_d_c = 0;
    } else {

        nb_d_cs = event->size().width() / m_IsotropicZoom;

        //!
        //! setup the fisrt display column
        //!
        if(f_d_c + nb_d_cs > bufferedRegion.GetSize()[0])
            f_d_c = bufferedRegion.GetSize()[0] - nb_d_cs;
    }

    //!
    //! check if the extend's index y value is greater than zero , if yes then the number of columns equals to the number of the buffered region
    if( extent.GetIndex()[1] > 0 ){
        nb_d_rs = bufferedRegion.GetSize()[1];
        f_d_r = extent.GetSize()[1] - nb_d_rs;
    } else {
        nb_d_rs = event->size().height() / m_IsotropicZoom;

        //!
        //! setup first display row
        //!
        if(f_d_r + nb_d_rs > bufferedRegion.GetSize()[1])
            f_d_r = bufferedRegion.GetSize()[0] - nb_d_rs;
    }

    m_pImageModelRenderer->setPaintingParameters(nb_d_cs,nb_d_rs,f_d_c,f_d_r);

    //! emit a signal
    setupAndSendSignal();
}

//!
void ItiOtbVectorQGLWidgetZoomable::setupAndSendSignal(){

    //! create a helper QRect value
    QRect rect;

    //!
    //! set the display related values
    //!

    rect.setX(m_pImageModelRenderer->firstDisplayColumn());
    rect.setWidth(m_pImageModelRenderer->nbDisplayColumns());
    rect.setY(m_pImageModelRenderer->firstDisplayRow());
    rect.setHeight(m_pImageModelRenderer->nbDisplayRows());

    //! emit the signal
    emit visibleAreaChanged(rect);
}


void ItiOtbVectorQGLWidgetZoomable::onLargestPossibleRegionChanged(const ImageRegionType& largestRegion)
{
  m_pImageViewManipulator->SetImageLargestRegion(largestRegion);
}

//!
ItiOtbVectorQGLWidgetZoomable::~ItiOtbVectorQGLWidgetZoomable(){
//    ClearBuffer();
}
