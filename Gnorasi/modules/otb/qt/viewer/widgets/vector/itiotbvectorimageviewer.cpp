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
    m_pItiOtbVectorImageWidgetFullView->setMinimumSize(160,160);
    m_pItiOtbVectorImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_pItiOtbVectorImageWidgetZoomable    = new ItiOtbVectorQGLWidgetZoomable(this);
    m_pItiOtbVectorImageWidgetZoomable->setMinimumSize(160,160);
    m_pItiOtbVectorImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //! TODO
    //!
    //! create the metadata widget, also a widget for showing pixel info metadata
    //!
    m_pItiViewerPixelInfoWidget         = new ItiViewerPixelInfoWidget(this);
    m_pItiViewerPixelInfoWidget->setGeometry(0,0,160,160);
    m_pItiViewerPixelInfoWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

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
    QWidget *phelperWidgetScroll = new QWidget(this,Qt::Window);
    phelperWidgetScroll->setWindowTitle(m_pLabelScrollableResolution->text());
    phelperWidgetScroll->setMinimumSize(QSize(150,150));
    phelperWidgetScroll->setGeometry(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200,200,200);
    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->addWidget(m_pItiOtbVectorImageWidgetScroll);
    phelperWidgetScroll->setLayout(vboxLayout);

    //! setup the full viewe
    QWidget *phelperWidgetFullView = new QWidget(this,Qt::Window);
    phelperWidgetFullView->setMinimumSize(QSize(150,150));
    phelperWidgetFullView->setWindowTitle(m_pLabelFullView->text());
    phelperWidgetFullView->setGeometry(QApplication::desktop()->width()/2 - 160,QApplication::desktop()->height()/2 - 160,200,200);
    QVBoxLayout *vboxLayout1 = new QVBoxLayout();
    vboxLayout1->addWidget(m_pItiOtbVectorImageWidgetFullView);
    phelperWidgetFullView->setLayout(vboxLayout1);


    //! setup the zoom viewe
    QWidget *phelperWidgetZoomView = new QWidget(this,Qt::Window);
    phelperWidgetZoomView->setMinimumSize(QSize(150,150));
    phelperWidgetZoomView->setWindowTitle(m_pLabelZoomView->text());
    phelperWidgetZoomView->setGeometry(QApplication::desktop()->width()/2 - 120,QApplication::desktop()->height()/2 - 120,200,200);
    QVBoxLayout *vboxLayout2 = new QVBoxLayout();
    vboxLayout2->addWidget(m_pItiOtbVectorImageWidgetZoomable);
    phelperWidgetZoomView->setLayout(vboxLayout2);

    //! establish signals and slot connections
    setupConnections();

    //! setup the observer mechanism
    setupObserverMechanism();

    //! show the widgets
    phelperWidgetScroll->show();
    phelperWidgetFullView->show();
    phelperWidgetZoomView->show();
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

    //! initial setup
    QRect rect;
    rect.setWidth(m_pItiOtbVectorImageWidgetZoomable->width());
    rect.setHeight(m_pItiOtbVectorImageWidgetZoomable->height());
     m_pFocusRegion->setRegion(rect);
}

//!
void ItiOtbVectorImageViewer::setupConnections(){
    //!
    connect(m_pItiOtbVectorImageWidgetScroll, SIGNAL(visibleAreaChanged(QRect)),this,SLOT(onScrollableWidgetSizeChanged(QRect)));
    connect(m_pItiOtbVectorImageWidgetScroll, SIGNAL(zoomIn()),m_pItiOtbVectorImageWidgetZoomable,SLOT(zoomIn()));
    connect(m_pItiOtbVectorImageWidgetScroll, SIGNAL(zoomOut()),m_pItiOtbVectorImageWidgetZoomable,SLOT(zoomOut()));
    connect(m_pItiOtbVectorImageWidgetScroll, SIGNAL(focusRegionTranslated(int,int)),m_pItiOtbVectorImageWidgetZoomable,SLOT(translate(int,int)));
    connect(m_pItiOtbVectorImageWidgetZoomable, SIGNAL(visibleAreaChanged(QRect)),this,SLOT(onZoomableWidgetSizeChanged(QRect)));
    connect(m_pItiOtbVectorImageWidgetFullView,SIGNAL(currentIndexChanged(QString)),m_pItiViewerPixelInfoWidget,SLOT(updateText(QString)));
    connect(m_pItiOtbVectorImageWidgetZoomable,SIGNAL(currentIndexChanged(QString)),m_pItiViewerPixelInfoWidget,SLOT(updateText(QString)));
    connect(m_pItiOtbVectorImageWidgetScroll,SIGNAL(currentIndexChanged(QString)),m_pItiViewerPixelInfoWidget,SLOT(updateText(QString)));
}

//!
void ItiOtbVectorImageViewer::onScrollableWidgetSizeChanged(const QRect &size){
    //!
    m_pVisibleRegion->setRegion(size);

    m_pFocusRegion->setRegion(m_pFocusRegion->region());
}

//!
void ItiOtbVectorImageViewer::onZoomableWidgetSizeChanged(const QRect &size){
    //!
    m_pFocusRegion->setRegion(size);
}

//!
void ItiOtbVectorImageViewer::onFocusRegionTranslated(const QRect &rect){
    //!
    m_pFocusRegion->setRegion(rect);
}