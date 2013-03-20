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

#include "itiotbvectorimageviewer.h"

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itiotbvectorqglwidgetfullview.h"
#include "itiotbvectorqglwidgetscrollable.h"
#include "itiotbvectorqglwidgetzoomable.h"
#include "../../utils/itiotbimagemanager.h"
#include "../../observables/itiviewerobservableregion.h"
#include "../itiviewerpixelinfowidget.h"
#include "../../../../ports/otbvectorimageport.h"

using namespace itiviewer;
using namespace voreen;

ItiOtbVectorImageViewer::ItiOtbVectorImageViewer(QWidget *parent) :
    m_pphelperWidgetZoomView(NULL),
    m_pphelperWidgetFullView(NULL),
    m_pphelperWidgetScroll(NULL),
    ItiOtbImageViewer(parent)
{
    //! set the visual mode to packed
    m_vmode                             = VMODE_PACKED;

    //!
    setupLayout();

    //!
    setupObserverMechanism();

    //!
    setupConnections();
}


/*!
 * \brief ItiOtbVectorImageViewer::setupLayout
 */
void ItiOtbVectorImageViewer::setupLayout(){

    //!
    m_labelCss = QString("QLabel { color: blue; font: bold 13px; } ");

    // initialize instances
    m_pLabelScrollableResolution        = new QLabel(tr("Scrollable View"),this);
    m_pLabelFullView                    = new QLabel(tr("Full View"),this);
    m_pLabelZoomView                    = new QLabel(tr("Zoomable View"),this);

    //!
    m_pLabelFullView->setStyleSheet(m_labelCss);
    m_pLabelScrollableResolution->setStyleSheet(m_labelCss);
    m_pLabelZoomView->setStyleSheet(m_labelCss);
    m_pLabelFullView->setAlignment(Qt::AlignHCenter);
    m_pLabelScrollableResolution->setAlignment(Qt::AlignHCenter);
    m_pLabelZoomView->setAlignment(Qt::AlignHCenter);

    //! setup the resize policy
    m_pLabelScrollableResolution->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pLabelFullView->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pLabelZoomView->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    //!
    // create the views opengl views
    m_pItiOtbVectorImageWidgetScroll      = new ItiOtbVectorQGLWidgetScrollable(this);
    m_pItiOtbVectorImageWidgetScroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pItiOtbVectorImageWidgetFullView    = new ItiOtbVectorQGLWidgetFullView(this);
    m_pItiOtbVectorImageWidgetFullView->setMinimumSize(10,10);
    m_pItiOtbVectorImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_pItiOtbVectorImageWidgetZoomable    = new ItiOtbVectorQGLWidgetZoomable(this);
    m_pItiOtbVectorImageWidgetZoomable->setMinimumSize(10,10);
    m_pItiOtbVectorImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //! TODO
    //!
    //! create the metadata widget, also a widget for showing pixel info metadata
    //!
    m_pItiViewerPixelInfoWidget         = new ItiViewerPixelInfoWidget(this);
    m_pItiViewerPixelInfoWidget->setGeometry(0,0,160,140);
    m_pItiViewerPixelInfoWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //! setup the left layout
    m_pvBoxLayoutLeft                   = new QVBoxLayout();
    m_pvBoxLayoutLeft->addWidget(m_pLabelFullView);
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbVectorImageWidgetFullView);
    m_pvBoxLayoutLeft->addWidget(m_pLabelZoomView);
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbVectorImageWidgetZoomable);
    m_pvBoxLayoutLeft->addWidget(m_pItiViewerPixelInfoWidget);
    QWidget *pWidgetLeft                = new QWidget(this);
    pWidgetLeft->setLayout(m_pvBoxLayoutLeft);

    //! setup the right layout
    m_pVboxLayoutRight                  = new QVBoxLayout();
    m_pVboxLayoutRight->addWidget(m_pLabelScrollableResolution);
    m_pVboxLayoutRight->addWidget(m_pItiOtbVectorImageWidgetScroll);
    //! create the widget
    QWidget *pWidgetRight               = new QWidget(this);
    pWidgetRight->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    pWidgetRight->setLayout(m_pVboxLayoutRight);

    //! setup the splitter
    m_pSplitter                         = new QSplitter(Qt::Horizontal,this);
    m_pSplitter->addWidget(pWidgetLeft);
    m_pSplitter->addWidget(pWidgetRight);
    m_pSplitter->setStretchFactor(1,1.1);
    m_pSplitter->setChildrenCollapsible(false);

    //!
    m_pMainLayout                       = new QVBoxLayout();
    m_pMainLayout->addWidget(m_pSplitter);
    m_pMainLayout->setContentsMargins(2,2,2,2);
    setLayout(m_pMainLayout);
}

/*!
 * \brief ItiOtbVectorImageViewer::disassembleWidgets
 */
void ItiOtbVectorImageViewer::disassembleWidgets(){
    //!
    m_vmode                             = VMODE_SPLITTED;

    //! remove the widget
    m_pMainLayout->removeWidget(m_pSplitter);

    //! reset observer mechanism
    resetObserverMechanism();

    //! Due to a lack of removing added itmes functionality on a QSplitter instance
    //! the added widgets must be firstly hidden and then deleted..
    m_pItiOtbVectorImageWidgetFullView->hide();
    m_pItiOtbVectorImageWidgetScroll->hide();
    m_pItiOtbVectorImageWidgetZoomable->hide();

    delete m_pItiOtbVectorImageWidgetFullView;
    delete m_pItiOtbVectorImageWidgetScroll;
    delete m_pItiOtbVectorImageWidgetZoomable;

    //! create the views

    //! scrollable view
    m_pItiOtbVectorImageWidgetScroll      = new ItiOtbVectorQGLWidgetScrollable(this);
    m_pItiOtbVectorImageWidgetScroll->setWindowTitle(m_pLabelScrollableResolution->text());

    //! full view
    m_pItiOtbVectorImageWidgetFullView    = new ItiOtbVectorQGLWidgetFullView(this);
    m_pItiOtbVectorImageWidgetFullView->setWindowTitle(m_pLabelFullView->text());

    //! zoomable view
    m_pItiOtbVectorImageWidgetZoomable    = new ItiOtbVectorQGLWidgetZoomable(this);
    m_pItiOtbVectorImageWidgetZoomable->setWindowTitle(m_pLabelZoomView->text());

    //! pixel info view
    m_pItiViewerPixelInfoWidget         = new ItiViewerPixelInfoWidget(this);
    m_pItiViewerPixelInfoWidget->setWindowTitle(m_pItiViewerPixelInfoWidget->title());
    m_pItiViewerPixelInfoWidget->setWindowFlags(Qt::Window);
    m_pItiViewerPixelInfoWidget->setGeometry(QApplication::desktop()->width()/2 - 80,QApplication::desktop()->height()/2 - 80,200,200);

    //!
    //! Because of a bug causing crashes when the viewer was set in split mode, we create container widgets for each seperate view
    //!

    //! setup the zoom viewe
    m_pphelperWidgetScroll = new QWidget(this,Qt::Window);
    m_pphelperWidgetScroll->setWindowTitle(m_pLabelScrollableResolution->text());
    m_pphelperWidgetScroll->setGeometry(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200,200,200);
    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->addWidget(m_pItiOtbVectorImageWidgetScroll);
    m_pphelperWidgetScroll->setLayout(vboxLayout);

    //! setup the full viewe
    m_pphelperWidgetFullView = new QWidget(this,Qt::Window);
    m_pphelperWidgetFullView->setWindowTitle(m_pLabelFullView->text());
    m_pphelperWidgetFullView->setGeometry(QApplication::desktop()->width()/2 - 160,QApplication::desktop()->height()/2 - 160,200,200);
    QVBoxLayout *vboxLayout1 = new QVBoxLayout();
    vboxLayout1->addWidget(m_pItiOtbVectorImageWidgetFullView);
    m_pphelperWidgetFullView->setLayout(vboxLayout1);


    //! setup the zoom viewe
    m_pphelperWidgetZoomView = new QWidget(this,Qt::Window);
    m_pphelperWidgetZoomView->setWindowTitle(m_pLabelZoomView->text());
    m_pphelperWidgetZoomView->setGeometry(QApplication::desktop()->width()/2 - 120,QApplication::desktop()->height()/2 - 120,200,200);
    QVBoxLayout *vboxLayout2 = new QVBoxLayout();
    vboxLayout2->addWidget(m_pItiOtbVectorImageWidgetZoomable);
    m_pphelperWidgetZoomView->setLayout(vboxLayout2);

    //! establish signals and slot connections
    setupConnections();

    //! setup the observer mechanism
    setupObserverMechanism();

    //
    readDisplaySettings();

    //! show the widgets
    m_pphelperWidgetScroll->show();
    m_pphelperWidgetFullView->show();
    m_pphelperWidgetZoomView->show();
    m_pItiViewerPixelInfoWidget->show();
}

/*!
 * \brief ItiOtbVectorImageViewer::assembleWidgets
 */
void ItiOtbVectorImageViewer::assembleWidgets(){
    //!
    //! TODO
    //! Add functionality if needed when assembling operation is handled,
    //! Though it may needed , as  the assembling operation creates a new instance of this class
    //!

    m_vmode                             = VMODE_PACKED;
}

/*!
 * \brief ItiOtbVectorImageViewer::draw
 *  This is where all the painting is done.
 */
void ItiOtbVectorImageViewer::draw(){
    //!
    setupModel();

    //!
    m_pItiOtbVectorImageWidgetScroll->draw();

    //!
    m_pItiOtbVectorImageWidgetFullView->draw();

    //!
    m_pItiOtbVectorImageWidgetZoomable->draw();
}

//!
void ItiOtbVectorImageViewer::resetObserverMechanism(){
    m_pFocusRegion->unRegisterObserver(m_pItiOtbVectorImageWidgetScroll);
    m_pFocusRegion->unRegisterObserver(m_pItiOtbVectorImageWidgetZoomable);

    m_pVisibleRegion->unRegisterObserver(m_pItiOtbVectorImageWidgetFullView);
}

//!
void ItiOtbVectorImageViewer::setupObserverMechanism(){
    //! setup observer mechanism
    m_pFocusRegion                      = new ItiViewerObservableRegion(this);
    m_pFocusRegion->registerObserver(m_pItiOtbVectorImageWidgetScroll);
    m_pFocusRegion->registerObserver(m_pItiOtbVectorImageWidgetZoomable);
    m_pVisibleRegion                    = new ItiViewerObservableRegion(this);
//    m_pVisibleRegion->registerObserver(m_pItiOtbVectorImageWidgetScroll);
    m_pVisibleRegion->registerObserver(m_pItiOtbVectorImageWidgetFullView);
}

//!
void ItiOtbVectorImageViewer::setupConnections(){
    //!
    connect(m_pItiOtbVectorImageWidgetScroll, SIGNAL(visibleAreaChanged(QRect)),this,SLOT(onScrollableWidgetSizeChanged(QRect)));
    connect(m_pItiOtbVectorImageWidgetScroll, SIGNAL(focusRegionChanged(QRect)),m_pItiOtbVectorImageWidgetZoomable,SLOT(onFocusRegionChanged(QRect)));
    connect(m_pItiOtbVectorImageWidgetZoomable, SIGNAL(focusAreaChanged(QRect,double)),this,SLOT(onZoomableWidgetSizeChanged(QRect,double)));
    connect(m_pItiOtbVectorImageWidgetFullView,SIGNAL(visibleAreaTranslated(int,int)),m_pItiOtbVectorImageWidgetScroll,SLOT(translate(int,int)));
//    connect(m_pItiOtbVectorImageWidgetFullView,SIGNAL(visibleAreaTranslated(int,int)),m_pItiOtbVectorImageWidgetZoomable,SLOT(translate(int,int)));
    connect(m_pItiOtbVectorImageWidgetZoomable,SIGNAL(readyToClearZoom()),m_pItiOtbVectorImageWidgetScroll,SLOT(resetZoom()));
    connect(m_pItiOtbVectorImageWidgetFullView,SIGNAL(currentIndexChanged(QString)),m_pItiViewerPixelInfoWidget,SLOT(updateText(QString)));
    connect(m_pItiOtbVectorImageWidgetZoomable,SIGNAL(currentIndexChanged(QString)),m_pItiViewerPixelInfoWidget,SLOT(updateText(QString)));
    connect(m_pItiOtbVectorImageWidgetScroll,SIGNAL(currentIndexChanged(QString)),m_pItiViewerPixelInfoWidget,SLOT(updateText(QString)));
}

//! update regions to notify observers
void ItiOtbVectorImageViewer::onScrollableWidgetSizeChanged(const QRect &rect){
    //!
    m_pVisibleRegion->updateRegion(rect);

    m_pFocusRegion->updateRegion(m_pItiOtbVectorImageWidgetZoomable->visibleArea());
}

//! update regions to notify observers
void ItiOtbVectorImageViewer::onZoomableWidgetSizeChanged(const QRect &rect, double z){
    //!
    m_pFocusRegion->updateRegion(rect,z);
}

//! update regions to notify observers
void ItiOtbVectorImageViewer::onFocusRegionChanged(const QRect &rect){
    //!
    m_pFocusRegion->updateRegion(rect);
}


void ItiOtbVectorImageViewer::setupModel(){
    QString path = m_pManager->imageFile();

    if(path.isEmpty())
        return;

    VectorImageModel *model = qobject_cast<VectorImageModel*>(m_pAbstractImageModel);

    if(model)
        model->loadFile(path);

    m_pItiOtbVectorImageWidgetFullView->enableMouseTracking();
    m_pItiOtbVectorImageWidgetScroll->enableMouseTracking();
    m_pItiOtbVectorImageWidgetZoomable->enableMouseTracking();

    emit modelChanged();
}

void ItiOtbVectorImageViewer::saveDisplaySettings(){
    if(!m_pphelperWidgetFullView || !m_pphelperWidgetScroll || !m_pphelperWidgetZoomView)
        return;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Gnorasi","ItiOtbVectorImageViewer");

    settings.beginGroup("FullView");
    settings.setValue("size", m_pphelperWidgetFullView->size());
    settings.setValue("pos", m_pphelperWidgetFullView->pos());
    settings.endGroup();

    settings.beginGroup("Scrollable");
    settings.setValue("size", m_pphelperWidgetScroll->size());
    settings.setValue("pos", m_pphelperWidgetScroll->pos());
    settings.endGroup();

    settings.beginGroup("Zoomable");
    settings.setValue("size", m_pphelperWidgetZoomView->size());
    settings.setValue("pos", m_pphelperWidgetZoomView->pos());
    settings.endGroup();

    settings.beginGroup("PixelInformation");
    settings.setValue("size",m_pItiViewerPixelInfoWidget->size());
    settings.setValue("pos",m_pItiViewerPixelInfoWidget->pos());
    settings.endGroup();
}


void ItiOtbVectorImageViewer::readDisplaySettings(){
    if(!m_pphelperWidgetFullView || !m_pphelperWidgetScroll || !m_pphelperWidgetZoomView)
        return;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Gnorasi","ItiOtbVectorImageViewer");

    settings.beginGroup("FullView");
    QSize size1     = settings.value("size", QSize(250,250)).toSize();
    QPoint pos1     = settings.value("pos", QPoint(200,200)).toPoint();
    m_pphelperWidgetFullView->resize(size1);
    m_pphelperWidgetFullView->move(pos1);
    settings.endGroup();

    settings.beginGroup("Scrollable");
    QSize size2     = settings.value("size", QSize(250,250)).toSize();
    QPoint pos2     = settings.value("pos", QPoint(300,300)).toPoint();
    m_pphelperWidgetScroll->resize(size2);
    m_pphelperWidgetScroll->move(pos2);
    settings.endGroup();

    settings.beginGroup("Zoomable");
    QSize size3     = settings.value("size", QSize(250,250)).toSize();
    QPoint pos3     = settings.value("pos", QPoint(400,400)).toPoint();
    m_pphelperWidgetZoomView->resize(size3);
    m_pphelperWidgetZoomView->move(pos3);
    settings.endGroup();

    settings.beginGroup("PixelInformation");
    QSize size4     = settings.value("size",QSize(200,200)).toSize();
    QPoint pos4     = settings.value("pos",QPoint(300,350)).toPoint();
    m_pItiViewerPixelInfoWidget->resize(size4);
    m_pItiViewerPixelInfoWidget->move(pos4);
    settings.endGroup();
}

void ItiOtbVectorImageViewer::forceUpdates(){
    m_pItiOtbVectorImageWidgetFullView->update();
    m_pItiOtbVectorImageWidgetScroll->update();
    m_pItiOtbVectorImageWidgetZoomable->update();
}


ItiOtbVectorImageViewer::~ItiOtbVectorImageViewer(){

    if(m_vmode == VMODE_SPLITTED){  // meaning this is the splitted mode
        saveDisplaySettings();
    }
}
