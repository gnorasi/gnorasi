/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

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
#include "../../utils/itiotblevel.h"
#include "../../utils/itiotbregion.h"

using namespace otb;
using namespace itiviewer;

ItiOtbVectorQGLWidgetZoomable::ItiOtbVectorQGLWidgetZoomable(ItiOtbVectorImageViewer *parent) :
    m_IsotropicZoom(1.0),
    m_SubsamplingRate(1),
    m_pImageViewManipulator( NULL ),
    m_pImageModelRenderer( NULL ),
    m_pItiOtbVectorImageViewer(parent),
    m_currentLevelId(1),
    QGLWidget(parent)
{
    setAutoFillBackground(false);

    m_pImageViewManipulator = new ImageViewManipulatorZoomable( this );
    m_pImageModelRenderer   = new ImageModelRendererZoomable( this );

    m_pen = QPen(QBrush(Qt::green),1.0);
}


void ItiOtbVectorQGLWidgetZoomable::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}


//!
void ItiOtbVectorQGLWidgetZoomable::resizeGL(int w, int h)
{
    m_pImageViewManipulator->resize(w,h);

    //! firstly setup the viweport with the new width and height
    setupViewport(w,h);

    unsigned int nb_d_cs = m_pImageModelRenderer->nbDisplayColumns();
    unsigned int nb_d_rs = m_pImageModelRenderer->nbDisplayRows();
    unsigned int f_d_c = m_pImageModelRenderer->firstDisplayColumn();
    unsigned int f_d_r = m_pImageModelRenderer->firstDisplayRow();

    ImageRegionType extent          = m_pImageViewManipulator->extent();
    ImageRegionType bufferedRegion  = m_pImageViewManipulator->bufferRegion();

    //!
    //! setup the number of rows and columns to be visualized
    //!
    //! check if the extend's index x value is greater than zero , if yes then the number of columns equals to the number of the buffered region
    if( extent.GetIndex()[0] > 0 ){
        nb_d_cs = bufferedRegion.GetSize()[0];
        f_d_c = 0;
    } else {

        nb_d_cs = w / m_IsotropicZoom;

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
        nb_d_rs = h / m_IsotropicZoom;

        //!
        //! setup first display row
        //!
        if(f_d_r + nb_d_rs > bufferedRegion.GetSize()[1])
            f_d_r = bufferedRegion.GetSize()[0] - nb_d_rs;
    }

    m_pImageModelRenderer->setPaintingParameters(nb_d_cs,nb_d_rs,f_d_c,f_d_r);

    //
    emit readyToClearZoom();

    //! emit a signal
    setupAndSendSignal();
}

///!
void ItiOtbVectorQGLWidgetZoomable::setupViewport(int w, int h){
    ImageRegionType extent;
    ImageRegionType bufferRegion = m_pImageViewManipulator->bufferRegion();
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
void ItiOtbVectorQGLWidgetZoomable::initializeColumnRowParameters(){

    setupViewport(width(),height());

    ImageRegionType extent = m_pImageViewManipulator->extent();
    ImageRegionType bufferedRegion = m_pImageViewManipulator->bufferRegion();

    unsigned int nb_d_cs    = 0;
    unsigned int nb_d_rs    = 0;
    unsigned int f_d_c      = 0;
    unsigned int f_d_r      = 0;

    if( extent.GetIndex()[0] > 0 )
        nb_d_cs = bufferedRegion.GetSize()[0];
    else
        nb_d_cs = width() / m_IsotropicZoom;

    if( extent.GetIndex()[1] > 0 )
        nb_d_rs = bufferedRegion.GetSize()[1];
    else
        nb_d_rs = height() / m_IsotropicZoom;

    //!
    //! the values along the y-axes follow some kind of a transormation matrix [1,-1]
    //!
    f_d_r = extent.GetSize()[1] - nb_d_rs;

    //!
    m_pImageModelRenderer->setPaintingParameters(nb_d_cs,nb_d_rs,f_d_c,f_d_r);
}

bool ItiOtbVectorQGLWidgetZoomable::isRegionPolygonInsideVisibleArea(const QPolygonF &pol) const{
//    ImageRegionType bufferedRegion = m_pImageViewManipulator->bufferRegion();
//    ImageRegionType extentRegion = m_pImageViewManipulator->extent();

//    QRectF rect = constructHelperRect();

//    QRectF brect = pol.boundingRect();

//    return rect.contains(brect);
    return true;
}


//!
void ItiOtbVectorQGLWidgetZoomable::paintEvent(QPaintEvent *event){

    //!
    makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();


    // Set the new rendering context to be known in the ModelRendere
    const AbstractImageModel* aiModel=  qobject_cast<AbstractImageModel*>(m_pItiOtbVectorImageViewer->model());

    // setup the rendering context
    if (aiModel)
    {
        // Get the region to draw from the ImageViewManipulator navigation
        // context
        const ImageRegionType region(m_pImageViewManipulator->bufferRegion() );

        const ImageRegionType extent(m_pImageViewManipulator->extent());

        ImageModelRendererZoomable::RenderingContext context(aiModel, region, extent, this->width(), this->height(),m_IsotropicZoom);

        // use the model renderer to paint the requested region of the image
        m_pImageModelRenderer->paintGL( context );
    }


    //! overpainting
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect = constructHelperRect();

    Level *pLevel = ITIOTBIMAGEMANAGER->levelById(m_currentLevelId);
    if(pLevel){

        ImageRegionType extent = m_pImageViewManipulator->extent();

        QList<Region*> regions = pLevel->regions();
        QList<Region*>::const_iterator i;
        for(i = regions.constBegin(); i != regions.constEnd(); i++){
            Region *pRegion = *i;
            if(pRegion->visible() && isRegionPolygonInsideVisibleArea(pRegion->area()))
                pRegion->drawRegion(&painter, extent, rect, m_IsotropicZoom);
        }
    }

    painter.end();
}

//!
void ItiOtbVectorQGLWidgetZoomable::mouseMoveEvent(QMouseEvent *event){

    m_pImageViewManipulator->mouseMoveEvent(event);

    // pixel info related functionality follows

    if(hasMouseTracking()){

        VectorImageModel *vModel = m_pItiOtbVectorImageViewer->vModel();
        if(vModel){
            QString text;

            ImageRegionType extent = m_pImageViewManipulator->extent();

            // check if both x y extent values are negative then this means that the
            // mouse position is definetely inside the image boundaries
            if(extent.GetIndex()[0] < 0 && extent.GetIndex()[1] < 0){
                ImageRegionType::IndexType idx  = indexFromPoint(event->pos());

                text = ITIOTBIMAGEMANAGER->constructInfoByIndex(idx,vModel);
            }else{
                QPoint point(event->pos().x()- extent.GetIndex()[0],event->pos().y()- extent.GetIndex()[1]);

                ImageRegionType::IndexType idx;
                idx[0] = point.x();
                idx[1] = point.y();

                // check whether the point is inside the image boundaries
                if(!ItiOtbImageManager::isInsideTheImage(extent,point))
                    text = ITIOTBIMAGEMANAGER->constructInfoByIndexAlt(idx);
                else{
                    text = ITIOTBIMAGEMANAGER->constructInfoByIndex(idx,vModel);
                }
            }

            // emit the signal containing the text of the pixel info
            emit currentIndexChanged(text);
        }
    }

    // now call the parent widget class mousemoveevent
    QGLWidget::mouseMoveEvent(event);
}

//!
void ItiOtbVectorQGLWidgetZoomable::draw(){

    // Set the new rendering context to be known in the ModelRendere
    const VectorImageModel* vModel=  qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());

    if(!vModel)
        return;

    m_pImageViewManipulator->InitializeContext(width(),height());

    m_pImageViewManipulator->SetImageLargestRegion(vModel->GetLargestPossibleRegion());

    //! initialize the column and row related parameters
    initializeColumnRowParameters();

    // emit the signal in order to update the focus region on the scrollable view
    setupAndSendSignal();
}

//!
void ItiOtbVectorQGLWidgetZoomable::updateObserver(ItiViewerObservable *observable){

    Q_UNUSED(observable);
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
void ItiOtbVectorQGLWidgetZoomable::setupAndSendSignal(){

    //! create a helper QRect value
    QRect rect = visibleArea();
    //! emit the signal
    emit focusAreaChanged(rect,m_IsotropicZoom);
}

QRect ItiOtbVectorQGLWidgetZoomable::visibleArea() const{
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
    return rect;
}


void ItiOtbVectorQGLWidgetZoomable::onLargestPossibleRegionChanged(const ImageRegionType& largestRegion)
{
  m_pImageViewManipulator->SetImageLargestRegion(largestRegion);
}


//!
void ItiOtbVectorQGLWidgetZoomable::zoomIn(){

    //! advance the zoom level by a fixed zoom scale value
    m_IsotropicZoom = m_IsotropicZoom + ZOOM_VALUE;

    //! setup the viewport in order to update the extend values
    setupViewport(width(),height());

    unsigned int nb_d_cs    = m_pImageModelRenderer->nbDisplayColumns();
    unsigned int nb_d_rs    = m_pImageModelRenderer->nbDisplayRows();
    unsigned int f_d_c      = m_pImageModelRenderer->firstDisplayColumn();
    unsigned int f_d_r      = m_pImageModelRenderer->firstDisplayRow();

    //!
    ImageRegionType extent          = m_pImageViewManipulator->extent();
    ImageRegionType bufferedRegion  = m_pImageViewManipulator->bufferRegion();

    //! calculate displaying - visualizing properties

    //! check if the extend's index x value is greater than zero
    if( extent.GetIndex()[0] > 0 ){
        //! the number of columns equal to the buffered regions size along the x-axes
        nb_d_cs = bufferedRegion.GetSize()[0];
        //! the first display column equals to zero
        f_d_c = 0;
    }
    else{
        //! the number of columns equal to the width of the window divided by the zoom value
        nb_d_cs = qRound((double)width() / m_IsotropicZoom);

        //! create a helper value equal to the current f_d_c plus a number that occurs due to the fact that the newly created visible rectangle will have less size
        int helperX = f_d_c + qRound(((double)nb_d_cs*ZOOM_VALUE)/2.0);

        //! check that the helperX value is between the extend's x-axes limits
        //! fistly check that it is less than zero
        if(helperX < 0)
            f_d_c = 0;
        else if(helperX > bufferedRegion.GetSize()[0] - nb_d_cs) //! then check that it is less that the bufferedRegion's size x value minus the number of displayable columns
            f_d_c = bufferedRegion.GetSize()[0] - nb_d_cs;
        else //! else set the f_d_c equal to the helper value
            f_d_c = helperX;
    }


    //! check if the extend's index y value is greater than zero
    if( extent.GetIndex()[1] > 0 ){
        //! set the appropriate values
        nb_d_rs = bufferedRegion.GetSize()[1];
        f_d_r = 0;
    }
    else{
        //! calculate the displayable rows
        nb_d_rs = qRound((double)height() / m_IsotropicZoom);

        //! create a helper value equal to the current f_d_r minus a number that occurs due to the fact that the newly created visible rectangle will have less size
        //!
        //! take notice that the y-axes display rows follow a transformation matrix [1,-1] , and this is the reason why the rounded value is being subtracted by the f_d_r
        //!
        int helperY = f_d_r - qRound(((double)nb_d_rs*ZOOM_VALUE)/2.0);

        //! check that the helperY value is between the extend's y-axes limits
        //! fistly check that it is less than zero
        if(helperY < 0)
            f_d_r = 0;
        else if(helperY > bufferedRegion.GetSize()[1] - nb_d_rs) //! then check that the helperY value is between the buffered region, if not set it to fit the right border of the image
            f_d_r = bufferedRegion.GetSize()[1] - nb_d_rs;
        else // else se the f_d_r equal to the helperY value
            f_d_r = helperY;
    }

    m_pImageModelRenderer->setPaintingParameters(nb_d_cs,nb_d_rs,f_d_c,f_d_r);

    //!
    update();

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

    unsigned int nb_d_cs = m_pImageModelRenderer->nbDisplayColumns();
    unsigned int nb_d_rs = m_pImageModelRenderer->nbDisplayRows();
    unsigned int f_d_c = m_pImageModelRenderer->firstDisplayColumn();
    unsigned int f_d_r = m_pImageModelRenderer->firstDisplayRow();

    ImageRegionType extent = m_pImageViewManipulator->extent();
    ImageRegionType bufferedRegion  = m_pImageViewManipulator->bufferRegion();

    //! setup the viewport in order to update the extend values
    setupViewport(width(),height());

    //! check if the extend's index x value is greater than zero
    if( extent.GetIndex()[0] > 0 ){

        //! the number of columns equal to the buffered regions size along the x-axes
        nb_d_cs = bufferedRegion.GetSize()[0];
        //! the first display column equals to zero
        f_d_c = 0;
    }
    else{
        //! the number of columns equal to the width of the window divided by the zoom value
        nb_d_cs = qRound(width() / m_IsotropicZoom);

        //! create a helper value equal to the current f_d_c plus a number that occurs due to the fact that the newly created visible rectangle will have less size
        int helperX = f_d_c - qRound((nb_d_cs*ZOOM_VALUE)/2);

        //! check that the helperX value is between the extend's x-axes limits
        //! fistly check that it is less than zero
        if(helperX < 0)
            f_d_c = 0;
        else if(helperX > bufferedRegion.GetSize()[0] - nb_d_cs) //! then check that it is less that the bufferedRegion's size x value minus the number of displayable columns
            f_d_c = bufferedRegion.GetSize()[0] - nb_d_cs;
        else //! else set the f_d_c equal to the helper value
            f_d_c = helperX;
    }

    //! check if the extend's index y value is greater than zero
    if( extent.GetIndex()[1] > 0 ){
        //! set the appropriate values
        nb_d_rs = bufferedRegion.GetSize()[1];
        f_d_r = 0;
    }
    else{
        //! calculate the displayable rows
        nb_d_rs = qRound(height() / m_IsotropicZoom);

        //! create a helper value equal to the current f_d_r minus a number that occurs due to the fact that the newly created visible rectangle will have less size
        //!
        //! take notice that the y-axes display rows follow a transformation matrix [1,-1] , and this is the reason why the rounded value is being subtracted by the f_d_r
        //!
        int helperY = f_d_r + qRound((nb_d_rs*ZOOM_VALUE)/2);


        //! check that the helperY value is between the extend's y-axes limits
        //! fistly check that the helperY value is less than zero
        if(helperY < 0)
            f_d_r = 0; //! then check that the helperY value is between the buffered region, if not set it to fit the right border of the image
        else if(helperY > bufferedRegion.GetSize()[1] - nb_d_rs)
            f_d_r = bufferedRegion.GetSize()[1] - nb_d_rs;
        else //! else se the f_d_r equal to the helperY value
            f_d_r = helperY;
    }

    m_pImageModelRenderer->setPaintingParameters(nb_d_cs,nb_d_rs,f_d_c,f_d_r);

    //! update the opengl painting..
    update();

    //! setup and send signal
    setupAndSendSignal();
}

/*!
 * \brief ItiOtbVectorQGLWidgetZoomable::translate
 * \param rect
 */
void ItiOtbVectorQGLWidgetZoomable::onFocusRegionChanged(const QRect &rect){

    unsigned int nb_d_cs = m_pImageModelRenderer->nbDisplayColumns();
    unsigned int nb_d_rs = m_pImageModelRenderer->nbDisplayRows();
    unsigned int f_d_c = m_pImageModelRenderer->firstDisplayColumn();
    unsigned int f_d_r = m_pImageModelRenderer->firstDisplayRow();

    m_IsotropicZoom = (double)width() / (double)rect.width()  ;

    f_d_c = rect.x();
    nb_d_cs = width() / m_IsotropicZoom;
    nb_d_rs = height() / m_IsotropicZoom;
    f_d_r = rect.y();

    ImageRegionType extent = m_pImageViewManipulator->extent();

    if(f_d_c + nb_d_cs > extent.GetSize()[0])
        f_d_c = extent.GetSize()[0];

    if(f_d_r + nb_d_rs > extent.GetSize()[1])
        f_d_r = extent.GetSize()[0] - nb_d_rs;


    //!
    m_pImageModelRenderer->setPaintingParameters(nb_d_cs,nb_d_rs,f_d_c,f_d_r);

    //
    update();
}

QRectF ItiOtbVectorQGLWidgetZoomable::constructHelperRect() const {
    ImageRegionType extent = m_pImageViewManipulator->extent();

    QRectF rect;
    rect.setX(m_pImageModelRenderer->firstDisplayColumn());
    if(extent.GetIndex()[1] > 0)
        rect.setY(0);
    else
        rect.setY(extent.GetSize()[1] - m_pImageModelRenderer->firstDisplayRow()-m_pImageModelRenderer->nbDisplayRows());

    rect.setWidth(m_pImageModelRenderer->nbDisplayColumns());
    rect.setHeight(m_pImageModelRenderer->nbDisplayRows());

    return rect;

}

ImageRegionType::IndexType ItiOtbVectorQGLWidgetZoomable::indexFromPoint(const QPoint &p){
    ImageRegionType::IndexType idx;

    ImageRegionType region = m_pImageViewManipulator->bufferRegion();

    unsigned int f_d_c      = m_pImageModelRenderer->firstDisplayColumn();
    unsigned int f_d_r      = m_pImageModelRenderer->firstDisplayRow();
    unsigned int nb_d_rs    = m_pImageModelRenderer->nbDisplayRows();

    idx[0] = f_d_c + (m_IsotropicZoom * p.x());
    idx[1] = region.GetSize()[1] - nb_d_rs - f_d_r + (m_IsotropicZoom * p.y());

    return idx;
}

void ItiOtbVectorQGLWidgetZoomable::enableMouseTracking(){
    //! mouse tracking is disabled on startup, set it on
    setMouseTracking(true);
}

//!
ItiOtbVectorQGLWidgetZoomable::~ItiOtbVectorQGLWidgetZoomable(){
//    ClearBuffer();
}
