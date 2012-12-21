#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itiotbvectorimageviewer.h"

#include "itiotbvectorqglwidgetzoomable.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "../../utils/itiotbimagemanager.h"
#include "../../observables/itiviewerobservableregion.h"
#include "../../../ports/otbvectorimageport.h"

using namespace otb;
using namespace itiviewer;

const double ZOOM_VALUE = 0.125;

ItiOtbVectorQGLWidgetZoomable::ItiOtbVectorQGLWidgetZoomable(QWidget *parent) :
    m_IsotropicZoom(1.0), m_OpenGlBuffer(NULL), m_OpenGlBufferedRegion(), m_Extent(), m_SubsamplingRate(1), m_first_displayed_col(0), m_first_displayed_row(0), m_nb_displayed_cols(0), m_nb_displayed_rows(0), QGLWidget(parent)
{

}

//!
void ItiOtbVectorQGLWidgetZoomable::ReadBuffer(const VectorImageType *image, const VectorRegionType &region){
    // Before doing anything, check if region is inside the buffered
    // region of image
    if (!image->GetBufferedRegion().IsInside(region)){
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

    while (!it.IsAtEnd()){

        // Fill the buffer
        unsigned int index = 0;

        // compute the linear index (buffer is flipped around X axis
        // when gl acceleration is disabled
        index = ItiOtbVectorImageViewer::ComputeXAxisFlippedBufferIndex(it.GetIndex(), region);

        // Fill the buffer
//        m_OpenGlBuffer[index]  = it.Get()[0];
//        m_OpenGlBuffer[index + 1] = it.Get()[1];
//        m_OpenGlBuffer[index + 2] = it.Get()[2];
        ++it;
    }

    //! Last, updating buffer size
    m_OpenGlBufferedRegion = region;

    //! setup the vieport
    setupViewport(width(),height());

    //! initialize the column and row related parameters
    initializeColumnRowParameters();

    //! emit a signal
    setupAndSendSignal();

    //! update the scene
    update();
}

//!
void ItiOtbVectorQGLWidgetZoomable::ClearBuffer(){
    // Delete previous buffer if needed
    if (m_OpenGlBuffer != NULL){

        //!
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

    resizeGL(width(),height());

    update();
}


void ItiOtbVectorQGLWidgetZoomable::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

//!
void ItiOtbVectorQGLWidgetZoomable::resizeGL(int w, int h)
{
    //! firstly setup the viweport with the new width and height
    setupViewport(w,h);

    //!
    //! setup the number of rows and columns to be visualized
    //!
    //! check if the extend's index x value is greater than zero , if yes then the number of columns equals to the number of the buffered region
    if( m_Extent.GetIndex()[0] > 0 ){
        m_nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
        m_first_displayed_col = 0;
    } else {

        m_nb_displayed_cols = m_W / m_IsotropicZoom;

        //!
        //! setup the fisrt display column
        //!
        if(m_first_displayed_col + m_nb_displayed_cols > m_OpenGlBufferedRegion.GetSize()[0])
            m_first_displayed_col = m_OpenGlBufferedRegion.GetSize()[0] - m_nb_displayed_cols;
    }

    //!
    //! check if the extend's index y value is greater than zero , if yes then the number of columns equals to the number of the buffered region
    if( m_Extent.GetIndex()[1] > 0 ){
        m_nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
        m_first_displayed_row = m_Extent.GetSize()[1] - m_nb_displayed_rows;
    } else {
        m_nb_displayed_rows = m_H / m_IsotropicZoom;

        //!
        //! setup first display row
        //!
        if(m_first_displayed_row + m_nb_displayed_rows > m_OpenGlBufferedRegion.GetSize()[1])
            m_first_displayed_row = m_OpenGlBufferedRegion.GetSize()[0] - m_nb_displayed_rows;

    }

    //! emit a signal
    setupAndSendSignal();
}

///!
void ItiOtbVectorQGLWidgetZoomable::setupViewport(int w, int h){
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
void ItiOtbVectorQGLWidgetZoomable::initializeColumnRowParameters(){

    if( m_Extent.GetIndex()[0] > 0 )
        m_nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
    else
        m_nb_displayed_cols = m_W / m_IsotropicZoom;

    if( m_Extent.GetIndex()[1] > 0 )
        m_nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
    else
        m_nb_displayed_rows = m_H / m_IsotropicZoom;

    //!
    //! the values along the y-axes follow some kind of a transormation matrix [1,-1]
    //!
    m_first_displayed_row = m_Extent.GetSize()[1] - m_nb_displayed_rows;
}

//!
void ItiOtbVectorQGLWidgetZoomable::paintGL()
{

    VectorIndexType startPosition = m_Extent.GetIndex();
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
void ItiOtbVectorQGLWidgetZoomable::mouseMoveEvent(QMouseEvent *event){
    OTBVectorImagePort *imgPort = (OTBVectorImagePort*)ITIOTBIMAGEMANAGER->port();
    if(imgPort && imgPort->isConnected()){
        VectorImageType* imgType =  (VectorImageType*)imgPort->getData();
        if(!imgType){
            QGLWidget::mouseMoveEvent(event);
            return;
        }

        //! get the position
        QPoint point = event->pos();

        //! the relative values are helper values in order to calculate the index values on a scaled (zoomed) image
        int relativeX = qRound((double)point.x()/m_IsotropicZoom);
        int relativeY = qRound((double)point.y()/m_IsotropicZoom);

        //! create an index
        VectorIndexType index;

        //! check if the extend's index x value is greater than zero
        //! if yes set the index x value equal to the relativeX value plus the x extend's index value
        if(m_Extent.GetIndex()[0] > 0)
            index[0] = relativeX + m_Extent.GetIndex()[0];
        else //! if not the index x value equal to the relativeX plus the m_first_displayed_col value
            index[0] = m_first_displayed_col + relativeX;

        //! check if the extend's index x value is greater than zero
        //! check y related value accordingly
        //!
        //! Take notice that the y-axes values follow a transformation matrix [1,-1]
        //! so the y-axes seem to be reversed in order to achieve the right results
        if(m_Extent.GetIndex()[1] > 0)
            index[1] =  m_Extent.GetSize()[1] - m_Extent.GetIndex()[1] + relativeY;
        else
            index[1] = m_OpenGlBufferedRegion.GetSize()[1] - m_nb_displayed_rows - m_first_displayed_row + relativeY;

        //! construct the text
        QString text = ItiOtbVectorImageViewer::constructTextFromImageIndex(index,imgType);

        //! emit the signal
        emit currentIndexChanged(text);
    }


    QGLWidget::mouseMoveEvent(event);
}

//!
void ItiOtbVectorQGLWidgetZoomable::draw(){
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

//!
void ItiOtbVectorQGLWidgetZoomable::updateObserver(ItiViewerObservable *observable){

    Q_UNUSED(observable);
}

//!
void ItiOtbVectorQGLWidgetZoomable::setupAndSendSignal(){

    //! create a helper QRect value
    QRect rect;

    //!
    //! set the display related values
    //!

    rect.setX(m_first_displayed_col);
    rect.setWidth(m_nb_displayed_cols);
    rect.setY(m_first_displayed_row);
    rect.setHeight(m_nb_displayed_rows);

    //! emit the signal
    emit visibleAreaChanged(rect);
}

//!
void ItiOtbVectorQGLWidgetZoomable::wheelEvent(QWheelEvent *event){
    if(event->delta() > 0)
        zoomIn();
    else
        zoomOut();

    //! accept the event
    event->accept();
}

//!
void ItiOtbVectorQGLWidgetZoomable::zoomIn(){

    //! advance the zoom level by a fixed zoom scale value
    m_IsotropicZoom = m_IsotropicZoom + ZOOM_VALUE;

    //! setup the viewport in order to update the extend values
    setupViewport(width(),height());

    //! calculate displaying - visualizing properties

    //! check if the extend's index x value is greater than zero
    if( m_Extent.GetIndex()[0] > 0 ){
        //! the number of columns equal to the buffered regions size along the x-axes
        m_nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
        //! the first display column equals to zero
        m_first_displayed_col = 0;
    }
    else{
        //! the number of columns equal to the width of the window divided by the zoom value
        m_nb_displayed_cols = qRound((double)m_W / m_IsotropicZoom);

        //! create a helper value equal to the current m_first_displayed_col plus a number that occurs due to the fact that the newly created visible rectangle will have less size
        int helperX = m_first_displayed_col + qRound(((double)m_nb_displayed_cols*ZOOM_VALUE)/2.0);

        //! check that the helperX value is between the extend's x-axes limits
        //! fistly check that it is less than zero
        if(helperX < 0)
            m_first_displayed_col = 0;
        else if(helperX > m_OpenGlBufferedRegion.GetSize()[0] - m_nb_displayed_cols) //! then check that it is less that the m_OpenGlBufferedRegion's size x value minus the number of displayable columns
            m_first_displayed_col = m_OpenGlBufferedRegion.GetSize()[0] - m_nb_displayed_cols;
        else //! else set the m_first_displayed_col equal to the helper value
            m_first_displayed_col = helperX;
    }


    //! check if the extend's index y value is greater than zero
    if( m_Extent.GetIndex()[1] > 0 ){
        //! set the appropriate values
        m_nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
        m_first_displayed_row = 0;
    }
    else{
        //! calculate the displayable rows
        m_nb_displayed_rows = qRound((double)m_H / m_IsotropicZoom);

        //! create a helper value equal to the current m_first_displayed_row minus a number that occurs due to the fact that the newly created visible rectangle will have less size
        //!
        //! take notice that the y-axes display rows follow a transformation matrix [1,-1] , and this is the reason why the rounded value is being subtracted by the m_first_displayed_row
        //!
        int helperY = m_first_displayed_row - qRound(((double)m_nb_displayed_rows*ZOOM_VALUE)/2.0);

        //! check that the helperY value is between the extend's y-axes limits
        //! fistly check that it is less than zero
        if(helperY < 0)
            m_first_displayed_row = 0;
        else if(helperY > m_OpenGlBufferedRegion.GetSize()[1] - m_nb_displayed_rows) //! then check that the helperY value is between the buffered region, if not set it to fit the right border of the image
            m_first_displayed_row = m_OpenGlBufferedRegion.GetSize()[1] - m_nb_displayed_rows;
        else // else se the m_first_displayed_row equal to the helperY value
            m_first_displayed_row = helperY;
    }

    //!
    updateGL();

    //! setup and send signal
    setupAndSendSignal();
}

//!
void ItiOtbVectorQGLWidgetZoomable::zoomOut(){

    //! decrease the zoom level
    m_IsotropicZoom = m_IsotropicZoom - ZOOM_VALUE;

    //! the minimum zoom level equals to 1
    if(m_IsotropicZoom < 1.0){
        m_IsotropicZoom = 1.0;
        return;
    }

    //! setup the viewport in order to update the extend values
    setupViewport(width(),height());

    //! check if the extend's index x value is greater than zero
    if( m_Extent.GetIndex()[0] > 0 ){

        //! the number of columns equal to the buffered regions size along the x-axes
        m_nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
        //! the first display column equals to zero
        m_first_displayed_col = 0;
    }
    else{
        //! the number of columns equal to the width of the window divided by the zoom value
        m_nb_displayed_cols = qRound(m_W / m_IsotropicZoom);

        //! create a helper value equal to the current m_first_displayed_col plus a number that occurs due to the fact that the newly created visible rectangle will have less size
        int helperX = m_first_displayed_col - qRound((m_nb_displayed_cols*ZOOM_VALUE)/2);

        //! check that the helperX value is between the extend's x-axes limits
        //! fistly check that it is less than zero
        if(helperX < 0)
            m_first_displayed_col = 0;
        else if(helperX > m_OpenGlBufferedRegion.GetSize()[0] - m_nb_displayed_cols) //! then check that it is less that the m_OpenGlBufferedRegion's size x value minus the number of displayable columns
            m_first_displayed_col = m_OpenGlBufferedRegion.GetSize()[0] - m_nb_displayed_cols;
        else //! else set the m_first_displayed_col equal to the helper value
            m_first_displayed_col = helperX;
    }

    //! check if the extend's index y value is greater than zero
    if( m_Extent.GetIndex()[1] > 0 ){
        //! set the appropriate values
        m_nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
        m_first_displayed_row = 0;
    }
    else{
        //! calculate the displayable rows
        m_nb_displayed_rows = qRound(m_H / m_IsotropicZoom);

        //! create a helper value equal to the current m_first_displayed_row minus a number that occurs due to the fact that the newly created visible rectangle will have less size
        //!
        //! take notice that the y-axes display rows follow a transformation matrix [1,-1] , and this is the reason why the rounded value is being subtracted by the m_first_displayed_row
        //!
        int helperY = m_first_displayed_row + qRound((m_nb_displayed_rows*ZOOM_VALUE)/2);


        //! check that the helperY value is between the extend's y-axes limits
        //! fistly check that the helperY value is less than zero
        if(helperY < 0)
            m_first_displayed_row = 0; //! then check that the helperY value is between the buffered region, if not set it to fit the right border of the image
        else if(helperY > m_OpenGlBufferedRegion.GetSize()[1] - m_nb_displayed_rows)
            m_first_displayed_row = m_OpenGlBufferedRegion.GetSize()[1] - m_nb_displayed_rows;
        else //! else se the m_first_displayed_row equal to the helperY value
            m_first_displayed_row = helperY;
    }

    //! update the opengl painting..
    updateGL();

    //! setup and send signal
    setupAndSendSignal();
}

/*!
 * \brief ItiOtbVectorQGLWidgetZoomable::translate
 * \param rect
 */
void ItiOtbVectorQGLWidgetZoomable::translate(int dx, int dy){

    //! create a helper value equal to the m_first_displayed_col plus the dx parameter
    int helperX = m_first_displayed_col + dx;

    //! check that the helperX value is less than zero
    if(helperX < 0)
        m_first_displayed_col = 0;
    //! check if it is beween the buffered region extends
    else if(helperX > m_OpenGlBufferedRegion.GetSize()[0] - m_nb_displayed_cols)
        m_first_displayed_col = m_OpenGlBufferedRegion.GetSize()[0] - m_nb_displayed_cols;
    else
        m_first_displayed_col = helperX;

    //!
    int helperY = m_first_displayed_row - dy;

    //! check that it is less than zero
    if(helperY < 0)
        m_first_displayed_row = 0;
    //! check that it is between the buffered region's y-axes size
    else if(helperY > m_OpenGlBufferedRegion.GetSize()[1] - m_nb_displayed_rows)
        m_first_displayed_row = m_OpenGlBufferedRegion.GetSize()[1] - m_nb_displayed_rows;
    else //! else set the m_first_displayed_row equal to the helperY value
        m_first_displayed_row = helperY;

    //! update painting
    updateGL();
}

//!
ItiOtbVectorQGLWidgetZoomable::~ItiOtbVectorQGLWidgetZoomable(){
    ClearBuffer();
}
