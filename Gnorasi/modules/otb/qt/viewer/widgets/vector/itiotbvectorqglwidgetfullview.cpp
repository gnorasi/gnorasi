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

#include "itiotbvectorqglwidgetfullview.h"
#include "../../observables/itiviewerobservableregion.h"
#include "itiotbvectorimageviewer.h"
#include "../../utils/itiotbimagemanager.h"
#include "../../../../ports/otbvectorimageport.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "otbVectorRescaleIntensityImageFilter.h"


#include "../../utils/itiotbImageModelRendererFullView.h"
#include "../../utils/itiotbImageViewManipulatorFullView.h"

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
    m_moving(false),
    QGLWidget(parent)
{
    setAutoFillBackground(false);

    m_pImageViewManipulator = new ImageViewManipulatorFullView( this );
    m_pImageModelRenderer   = new ImageModelRendererFullView( this );

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

    ImageRegionType bufferedRegion = m_pImageViewManipulator->bufferRegion();

    double bratio = (double)bufferedRegion.GetSize()[0] / (double)bufferedRegion.GetSize()[1];

    double viewportratio = (double)w/(double)h;

    if(w < h){
        if(bratio > viewportratio)
            m_IsotropicZoom = static_cast<double>(w)/ static_cast<double>(bufferedRegion.GetSize()[0]);
        else
            m_IsotropicZoom = static_cast<double>(h)/ static_cast<double>(bufferedRegion.GetSize()[1]);
    }else{
        if(bratio > viewportratio)
            m_IsotropicZoom = static_cast<double>(w)/ static_cast<double>(bufferedRegion.GetSize()[0]);
        else
            m_IsotropicZoom = static_cast<double>(h)/ static_cast<double>(bufferedRegion.GetSize()[1]);
    }

    // setup the isotropic zoom value, check whether the width is greater than height
//    m_IsotropicZoom = w < h || bratio > viewportratio ? static_cast<double>(w)/ static_cast<double>(bufferedRegion.GetSize()[0]) : static_cast<double>(h)/ static_cast<double>(bufferedRegion.GetSize()[1]);

    VectorSizeType size;
    size [0] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(bufferedRegion.GetSize()[0]));
    size [1] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(bufferedRegion.GetSize()[1]));

    ImageRegionType::IndexType index;
    index[0] = (w - static_cast<int>(size[0])) / 2;
    index[1] = (h - static_cast<int>(size[1])) / 2;

    ImageRegionType extent;

    extent.SetIndex(index);
    extent.SetSize(size);

    m_pImageViewManipulator->setExtent(extent);

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

    qglClearColor(Qt::black);

    // Set the new rendering context to be known in the ModelRendere
    const AbstractImageModel* aiModel=  qobject_cast<AbstractImageModel*>(m_pItiOtbVectorImageViewer->model());

    // setup the rendering context
    if (aiModel)
    {
        setupViewport(width(), height());

        const ImageRegionType extent(m_pImageViewManipulator->extent());

        // Get the buffer region to draw from the ImageViewManipulator navigation
        // context
        const ImageRegionType region(m_pImageViewManipulator->bufferRegion());

        // create a rendering context instance
        ImageModelRendererFullView::RenderingContext context(aiModel, region, extent, this->width(), this->height(), m_IsotropicZoom);

        // use the model renderer to paint the requested region of the image
        m_pImageModelRenderer->paintGL( context );
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //! overpainting
    painter.setPen(m_pen);
    painter.drawRect(m_visibleRegion);

    painter.end();
}

//!
void ItiOtbVectorQGLWidgetFullView::draw(){

    // Set the new rendering context to be known in the ModelRendere
    const VectorImageModel* vModel=  qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());

    if(!vModel)
        return;

    //
    m_pImageViewManipulator->InitializeContext(width(),height());

    //
    m_pImageViewManipulator->SetImageLargestRegion(vModel->GetLargestPossibleRegion());

    //! initialize the column and row related parameters
    setupViewport(width(),height());
}

//!
void ItiOtbVectorQGLWidgetFullView::updateObserver(ItiViewerObservable *observable){
    ItiViewerObservableRegion *region = qobject_cast<ItiViewerObservableRegion*>(observable);
    if(!region)
        return;

    setupViewport(width(),height());

    ImageRegionType extent = m_pImageViewManipulator->extent();

    QRect rect = region->region();

    m_visibleRegion.setX(extent.GetIndex()[0] + (qAbs(rect.x()*m_IsotropicZoom)));
    m_visibleRegion.setY(extent.GetIndex()[1]+(qAbs(rect.y()*m_IsotropicZoom) ));

    //! calculate the new width and height value;
    int nw = m_IsotropicZoom * rect.width();
    int nh = m_IsotropicZoom * rect.height();

    //! set the new width and height to the visible region
    m_visibleRegion.setWidth(nw);
    m_visibleRegion.setHeight(nh);

//    //!
//    //! The following code is dummy,
//    //! There was a problem of not updating the widget correctly
//    //! Forcing the widget to be updated by hiding and showing is a tweek to be updated correctly
//    //!
    hide();
    show();
}

//!
void ItiOtbVectorQGLWidgetFullView::mouseMoveEvent(QMouseEvent *event){

    if(m_moving){
        //! create a helper point value
        QPoint previousCenter = m_visibleRegion.center();

        //! get the position of the event
        QPoint point = event->pos();

        translate(previousCenter,point);
    }

    // pixel info related functionality follows
    if(hasMouseTracking()){
        VectorImageModel *vModel = qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());
        if(vModel){
            ImageRegionType extent = m_pImageViewManipulator->extent();

            QPoint point((event->pos().x()- extent.GetIndex()[0])/m_IsotropicZoom,(event->pos().y()- extent.GetIndex()[1])/m_IsotropicZoom);

            ImageRegionType::IndexType idx;
            idx[0] = point.x();
            idx[1] = point.y();

            VectorImageType *img = m_pItiOtbVectorImageViewer->manager()->image();
            if(img){

                QString text;

                // check whether the point is inside the image boundaries
                if(!ItiOtbImageManager::isInsideTheImage(extent,point,m_IsotropicZoom))
                    text = m_pItiOtbVectorImageViewer->manager()->constructInfoByIndexAlt(idx);
                else{
                    VectorImageType::PixelType pixelValue = img->GetPixel(idx);

                    RenderingFilterType* filter = vModel->filter();

                    const std::string pixeldata = filter->GetRenderingFunction()->Describe(pixelValue);

                    QString pdt = QString::fromStdString(pixeldata);

                    text = m_pItiOtbVectorImageViewer->manager()->constructInfoByIndex(idx,pdt);
                }


                emit currentIndexChanged(text);
            }
        }
    }

    // now call the parent widget class mousemoveevent

    QGLWidget::mouseMoveEvent(event);
}

//!
void ItiOtbVectorQGLWidgetFullView::mousePressEvent(QMouseEvent *event){

    //! setup translating functionality only on left button pressed mouse events
    if(event->button() == Qt::LeftButton){

        ImageRegionType region = m_pImageViewManipulator->bufferRegion();

        if(!region.GetSize()[0] || !region.GetSize()[1]){
            QGLWidget::mousePressEvent(event);
            return;
        }

        m_moving = true;

        QCursor dragCursor;
        dragCursor.setShape(Qt::ClosedHandCursor) ;
        this->setCursor(dragCursor);

        //! create a helper point value
        QPoint previousCenter = m_visibleRegion.center();

        //! get the position of the event
        QPoint point = event->pos();

        // translate function
        translate(previousCenter,point);
    }

    QGLWidget::mousePressEvent(event);
}


void ItiOtbVectorQGLWidgetFullView::translate(const QPoint &previousCenter, const QPoint& point){
    //! create a helper line object
    QLine line(previousCenter,point);

    ImageRegionType extent = m_pImageViewManipulator->extent();

    //! create helper values
    int dx = 0, dy = 0;

    //!
    //! check if the new rect right border exceeds the extends' width value
    //!
    if(point.x()+ qRound((double)m_visibleRegion.width()/2.0) > extent.GetSize()[0] + extent.GetIndex()[0]){
        dx = extent.GetIndex()[0] + extent.GetSize()[0] - qRound((double)m_visibleRegion.width()/2.0) - previousCenter.x();
    } //! check if the new rect left border exceeds the extend's index x value
    else if(point.x()- qRound((double)m_visibleRegion.width()/2.0) < extent.GetIndex()[0]){
        dx = extent.GetIndex()[0] + qRound((double)m_visibleRegion.width()/2.0) - previousCenter.x();
    }else //! else set the dx value equal to the line dx value
        dx = line.dx();

    //! check if the new rect right border exceeds the extends' height value
    if(point.y()+ qRound((double)m_visibleRegion.height()/2.0) > extent.GetSize()[1] + extent.GetIndex()[1]){
        dy = extent.GetSize()[1] + extent.GetIndex()[1] - qRound((double)m_visibleRegion.height()/2.0) - previousCenter.y();
    } //! check if the new rect left border exceeds the extend's index y value
    else if(point.y()- qRound((double)m_visibleRegion.height()/2.0) < extent.GetIndex()[1]){
        dy = extent.GetIndex()[1] + qRound((double)m_visibleRegion.height()/2.0) - previousCenter.y();
    }else //! else set the dy value equal to the line dy value
        dy = line.dy();

    //!translate the focus region
    m_visibleRegion.translate(dx,dy);

    //! emit signal in order to update the zoomable view
    emit visibleAreaTranslated(dx/m_IsotropicZoom,dy/m_IsotropicZoom);

    //! update widget
    update();
}

void ItiOtbVectorQGLWidgetFullView::mouseReleaseEvent(QMouseEvent *event){
    m_moving = false;

    QCursor dragCursor;
    dragCursor.setShape(Qt::ArrowCursor) ;
    this->setCursor(dragCursor);

    QGLWidget::mouseReleaseEvent(event);
}


//!
ItiOtbVectorQGLWidgetFullView::~ItiOtbVectorQGLWidgetFullView(){

}
