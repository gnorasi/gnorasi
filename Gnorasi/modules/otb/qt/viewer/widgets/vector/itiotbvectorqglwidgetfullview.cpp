#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itiotbvectorqglwidgetfullview.h"
#include "../../observables/itiviewerobservableregion.h"
#include "itiotbvectorimageviewer.h"
#include "../../utils/itiotbimagemanager.h"
#include "../../../../ports/otbvectorimageport.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "otbVectorRescaleIntensityImageFilter.h"


#include "../../utils/itiotbImageModelRenderer.h"
#include "../../utils/itiotbImageViewManipulator.h"

#include "../../models/itiotbVectorImageModel.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "../../models/itiotbAbstractImageModel.h"

using namespace otb;
using namespace itiviewer;
using namespace voreen;

ItiOtbVectorQGLWidgetFullView::ItiOtbVectorQGLWidgetFullView(ItiOtbVectorImageViewer *parent) :
    m_IsotropicZoom(1.0),
    m_SubsamplingRate(1),
    m_pImageViewManipulator( NULL ),
    m_pImageModelRenderer( NULL ),
    m_pItiOtbVectorImageViewer(parent),
    QGLWidget(parent)
{
    setAutoFillBackground(false);

    m_pImageViewManipulator = new ImageViewManipulator( this );
    m_pImageModelRenderer   = new ImageModelRenderer( this );

    m_pen = QPen(QBrush(Qt::red),2.0);
}


void ItiOtbVectorQGLWidgetFullView::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}


void ItiOtbVectorQGLWidgetFullView::resizeGL(int w, int h)
{
    setupViewport(w,h);
}

void ItiOtbVectorQGLWidgetFullView::setupViewport(int w, int h){
//    if(!m_OpenGlBuffer)
//        return;

//    m_IsotropicZoom = w < h ? static_cast<double>(w)/ static_cast<double>(m_OpenGlBufferedRegion.GetSize()[0]) : static_cast<double>(h)/ static_cast<double>(m_OpenGlBufferedRegion.GetSize()[1]);

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

    glViewport(0, 0, (GLint)w, (GLint)h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLint)w, 0, (GLint)h, -1, 1);
}

//!
void ItiOtbVectorQGLWidgetFullView::paintEvent(QPaintEvent *event){
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
      ImageModelRenderer::RenderingContext context(aiModel, region, this->width(), this->height());

      // use the model renderer to paint the requested region of the image
      m_pImageModelRenderer->paintGL( context );
    }


    //
    // END OF TEST

//    unsigned int nb_displayed_rows;
//    unsigned int nb_displayed_cols;
//    unsigned int first_displayed_row;
//    unsigned int first_displayed_col;

//    if( m_Extent.GetIndex()[0] >= 0 )
//    {
//        nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
//        first_displayed_col = 0;
//    }
//    else
//    {
//        nb_displayed_cols = m_W / m_IsotropicZoom;
//        first_displayed_col = (m_OpenGlBufferedRegion.GetSize()[0] - nb_displayed_cols) / 2;
//    }

//    if( m_Extent.GetIndex()[1] >= 0 )
//    {
//        nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
//        first_displayed_row = 0;
//    }
//    else
//    {
//        nb_displayed_rows = m_H / m_IsotropicZoom;
//        first_displayed_row = (m_OpenGlBufferedRegion.GetSize()[1] - nb_displayed_rows) / 2;
//    }


//    VectorIndexType startPosition = m_Extent.GetIndex();
//    startPosition[0] = startPosition[0] < 0 ? 0 : startPosition[0];
//    startPosition[1] = startPosition[1] < 0 ? 0 : startPosition[1];

//    qglClearColor(Qt::black);

//    setupViewport(width(), height());

//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_OpenGlBufferedRegion.GetSize()[0]);
//    glPixelStorei(GL_UNPACK_SKIP_PIXELS, first_displayed_col);
//    glPixelStorei(GL_UNPACK_SKIP_ROWS,first_displayed_row);

//    glClear(GL_COLOR_BUFFER_BIT);
//    glPixelZoom(m_IsotropicZoom,m_IsotropicZoom);

//    glRasterPos2f(startPosition[0], startPosition[1]);
//    glDrawPixels(nb_displayed_cols,
//             nb_displayed_rows,
//             GL_RGB,
//             GL_UNSIGNED_BYTE,
//             m_OpenGlBuffer);

//    glFlush();

//    glShadeModel(GL_FLAT);
//    glDisable(GL_CULL_FACE);
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);

//    glMatrixMode(GL_MODELVIEW);
//    glPopMatrix();

//    DebugOpenGL();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //! overpainting
    painter.setPen(m_pen);
    painter.drawRect(m_visibleRegion);

    painter.end();
}

//!
void ItiOtbVectorQGLWidgetFullView::draw(){
    //!
//    VectorImageType* imgType =  ITIOTBIMAGEMANAGER->image();
//    if(!imgType)
//        return;

//    VectorRegionType region = imgType->GetLargestPossibleRegion();

//    //!
//    ReadBuffer(imgType,region);

    // Set the new rendering context to be known in the ModelRendere
    const VectorImageModel* vModel=  qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());

    if(!vModel)
        return;

    m_pImageViewManipulator->InitializeContext(width(),height());

    m_pImageViewManipulator->SetImageLargestRegion(vModel->GetLargestPossibleRegion());

    //!
    setMouseTracking(true);
}

//!
void ItiOtbVectorQGLWidgetFullView::updateObserver(ItiViewerObservable *observable){
//    ItiViewerObservableRegion *region = qobject_cast<ItiViewerObservableRegion*>(observable);
//    if(!region)
//        return;

//    QRect rect = region->region();
//    //! check if the x coordinate of the given rect is greater than zero
//    if(rect.x()>=0)
//        m_visibleRegion.setX(m_Extent.GetIndex()[0]);
//    else //! if not , this means that the scrollable view has been resized to a value smaller than the original size of the image
//        m_visibleRegion.setX(m_Extent.GetIndex()[0] + (qAbs(rect.x()*m_IsotropicZoom)));

//    //! same type of checking as the checking on the previous if statement
//    if(rect.y() >= 0)
//        m_visibleRegion.setY(m_Extent.GetIndex()[1]);
//    else
//        m_visibleRegion.setY(m_Extent.GetIndex()[1]+(qAbs(rect.y()*m_IsotropicZoom) ));

//    //! calculate the new width and height value;
//    int nw = m_IsotropicZoom * rect.width();
//    int nh = m_IsotropicZoom * rect.height();

//    //! set the new width and height to the visible region
//    m_visibleRegion.setWidth(nw);
//    m_visibleRegion.setHeight(nh);

//    //! update the widget
//    update();

//    //!
//    //! The following code is dummy,
//    //! There was a problem of not updating the widget correctly
//    //! Forcing the widget to be updated by hiding and showing is a tweek to be updated correctly
//    //!
//    hide();
//    show();
}

//!
void ItiOtbVectorQGLWidgetFullView::mouseMoveEvent(QMouseEvent *event){

    OTBVectorImagePort *imgPort = (OTBVectorImagePort*)ITIOTBIMAGEMANAGER->port();
    if(imgPort && imgPort->isConnected()){
//        VectorImageType* imgType =  (VectorImageType*)imgPort->getData();
//        if(!imgType){
//            QGLWidget::mouseMoveEvent(event);
//            return;
//        }

//        //! get the position
//        QPoint point = event->pos();

//        VectorIndexType index;
//        index[0] = (point.x() - m_Extent.GetIndex()[0])/m_IsotropicZoom;
//        index[1] = (point.y() - m_Extent.GetIndex()[1])/m_IsotropicZoom;

//        QString text = ItiOtbVectorImageViewer::constructTextFromImageIndex(index,imgType);

//        emit currentIndexChanged(text);
    }

    QGLWidget::mouseMoveEvent(event);
}


//!
ItiOtbVectorQGLWidgetFullView::~ItiOtbVectorQGLWidgetFullView(){
//    ClearBuffer();
}
