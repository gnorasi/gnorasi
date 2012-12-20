#include "itiotbrgbaimageviewer.h"

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "itiotbrgbaqglwidgetfullview.h"
#include "itiotbrgbaqglwidgetscrollable.h"
#include "itiotbrgbaqglwidgetzoomable.h"
#include "../utils/itiotbimagemanager.h"
#include "itiotbrgbaqglwidgetscrollable.h"
#include "itiotbrgbaqglwidgetzoomable.h"
#include "itiotbrgbaqglwidgetfullview.h"
#include "../observables/itiviewerobservableregion.h"
#include "itiviewerpixelinfowidget.h"
#include "../../../ports/otbimageport.h"

#include "otbImage.h"

using namespace itiviewer;
using namespace voreen;

ItiOtbRgbaImageViewer::ItiOtbRgbaImageViewer(QWidget *parent) :
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
 * \brief ItiOtbRgbaImageViewer::setupLayout
 */
void ItiOtbRgbaImageViewer::setupLayout(){

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
    m_pItiOtbRgbaImageWidgetScroll      = new ItiOtbRgbaQGLWidgetScrollable(this);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pItiOtbRgbaImageWidgetFullView    = new ItiOtbRgbaQGLWidgetFullView(this);
    m_pItiOtbRgbaImageWidgetFullView->setMinimumSize(160,160);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_pItiOtbRgbaImageWidgetZoomable    = new ItiOtbRgbaQGLWidgetZoomable(this);
    m_pItiOtbRgbaImageWidgetZoomable->setMinimumSize(160,160);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

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
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetFullView);
    m_pvBoxLayoutLeft->addWidget(m_pLabelZoomView);
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetZoomable);
    m_pvBoxLayoutLeft->addWidget(m_pItiViewerPixelInfoWidget);
    QWidget *pWidgetLeft                = new QWidget(this);
    pWidgetLeft->setLayout(m_pvBoxLayoutLeft);

    //! setup the right layout
    m_pVboxLayoutRight                  = new QVBoxLayout();
    m_pVboxLayoutRight->addWidget(m_pLabelScrollableResolution);
    m_pVboxLayoutRight->addWidget(m_pItiOtbRgbaImageWidgetScroll);
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
 * \brief ItiOtbRgbaImageViewer::disassembleWidgets
 */
void ItiOtbRgbaImageViewer::disassembleWidgets(){
    m_pMainLayout->removeWidget(m_pSplitter);

    //! Due to a lack of removing added itmes functionality on a QSplitter instance
    //! the added widgets must be firstly hidden and then deleted..
    m_pItiOtbRgbaImageWidgetFullView->hide();
    m_pItiOtbRgbaImageWidgetScroll->hide();
    m_pItiOtbRgbaImageWidgetZoomable->hide();
    delete m_pItiOtbRgbaImageWidgetFullView;
    delete m_pItiOtbRgbaImageWidgetScroll;
    delete m_pItiOtbRgbaImageWidgetZoomable;

    //! create the widgets
    m_pItiOtbRgbaImageWidgetScroll      = new ItiOtbRgbaQGLWidgetScrollable(this);
    m_pItiOtbRgbaImageWidgetScroll->setWindowFlags(Qt::Window);
    m_pItiOtbRgbaImageWidgetScroll->setWindowTitle(m_pLabelScrollableResolution->text());
    m_pItiOtbRgbaImageWidgetScroll->setGeometry(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200,200,200);
    m_pItiOtbRgbaImageWidgetScroll->show();
    m_pItiOtbRgbaImageWidgetFullView    = new ItiOtbRgbaQGLWidgetFullView(this);
    m_pItiOtbRgbaImageWidgetFullView->setWindowFlags(Qt::Window);
    m_pItiOtbRgbaImageWidgetFullView->setWindowTitle(m_pLabelFullView->text());
    m_pItiOtbRgbaImageWidgetFullView->setGeometry(QApplication::desktop()->width()/2 - 160,QApplication::desktop()->height()/2 - 160,200,200);
    m_pItiOtbRgbaImageWidgetFullView->show();
    m_pItiOtbRgbaImageWidgetZoomable    = new ItiOtbRgbaQGLWidgetZoomable(this);
    m_pItiOtbRgbaImageWidgetZoomable->setWindowFlags(Qt::Window);
    m_pItiOtbRgbaImageWidgetZoomable->setWindowTitle(m_pLabelZoomView->text());
    m_pItiOtbRgbaImageWidgetZoomable->show();
    m_pItiOtbRgbaImageWidgetZoomable->setGeometry(QApplication::desktop()->width()/2 - 120,QApplication::desktop()->height()/2 - 120,200,200);
    m_pItiViewerPixelInfoWidget         = new ItiViewerPixelInfoWidget(this);
    m_pItiViewerPixelInfoWidget->setWindowFlags(Qt::Window);
    m_pItiViewerPixelInfoWidget->setGeometry(QApplication::desktop()->width()/2 - 80,QApplication::desktop()->height()/2 - 80,200,200);

    //!
    setupObserverMechanism();

    //!
    setupConnections();

    //!
    m_vmode                             = VMODE_SPLITTED;

    //!
    m_pItiViewerPixelInfoWidget->show();
}

/*!
 * \brief ItiOtbRgbaImageViewer::assembleWidgets
 */
void ItiOtbRgbaImageViewer::assembleWidgets(){
    //!
    //! TODO
    //! Add functionality if needed when assembling operation is handled,
    //! Though it may needed , as  the assembling operation creates a new instance of this class
    //!

    m_vmode                             = VMODE_PACKED;
}

/*!
 * \brief ItiOtbRgbaImageViewer::draw
 *  This is where all the painting is done.
 */
void ItiOtbRgbaImageViewer::draw(){
    //!
    m_pItiOtbRgbaImageWidgetScroll->draw();

    //!
    m_pItiOtbRgbaImageWidgetFullView->draw();

    //!
    m_pItiOtbRgbaImageWidgetZoomable->draw();
}

/*!
 * \brief ItiOtbRgbaImageViewer::applyGreyScaleColorMode
 * \param band
 */
void ItiOtbRgbaImageViewer::applyGreyScaleColorMode(int band){
    Q_UNUSED(band)
}

/*!
 * \brief ItiOtbRgbaImageViewer::applyRGBColorMode
 * \param red
 * \param green
 * \param blue
 */
void ItiOtbRgbaImageViewer::applyRGBColorMode(int red, int green, int blue){
    Q_UNUSED(red)
    Q_UNUSED(green)
    Q_UNUSED(blue)
}

/*!
 * \brief ItiOtbRgbaImageViewer::applyContrastEnhancementMethod
 * \param ce
 * \param aval
 * \param bval
 */
void ItiOtbRgbaImageViewer::applyContrastEnhancementMethod(CC ce, double aval, double bval){
    Q_UNUSED(ce)

    Q_UNUSED(aval)
    Q_UNUSED(bval)
}

//!
void ItiOtbRgbaImageViewer::setupObserverMechanism(){
    //! setup observer mechanism
    m_pFocusRegion                      = new ItiViewerObservableRegion(this);
    m_pFocusRegion->registerObserver(m_pItiOtbRgbaImageWidgetScroll);
    m_pFocusRegion->registerObserver(m_pItiOtbRgbaImageWidgetZoomable);
    m_pVisibleRegion                    = new ItiViewerObservableRegion(this);
//    m_pVisibleRegion->registerObserver(m_pItiOtbRgbaImageWidgetScroll);
    m_pVisibleRegion->registerObserver(m_pItiOtbRgbaImageWidgetFullView);

    //! initial setup
    QRect rect;
    rect.setWidth(m_pItiOtbRgbaImageWidgetZoomable->width());
    rect.setHeight(m_pItiOtbRgbaImageWidgetZoomable->height());
     m_pFocusRegion->setRegion(rect);
}

//!
void ItiOtbRgbaImageViewer::setupConnections(){
    //!
    connect(m_pItiOtbRgbaImageWidgetScroll, SIGNAL(visibleAreaChanged(QRect)),this,SLOT(onScrollableWidgetSizeChanged(QRect)));
    connect(m_pItiOtbRgbaImageWidgetScroll, SIGNAL(zoomIn()),m_pItiOtbRgbaImageWidgetZoomable,SLOT(zoomIn()));
    connect(m_pItiOtbRgbaImageWidgetScroll, SIGNAL(zoomOut()),m_pItiOtbRgbaImageWidgetZoomable,SLOT(zoomOut()));
    connect(m_pItiOtbRgbaImageWidgetScroll, SIGNAL(focusRegionTranslated(int,int)),m_pItiOtbRgbaImageWidgetZoomable,SLOT(translate(int,int)));
    connect(m_pItiOtbRgbaImageWidgetZoomable, SIGNAL(visibleAreaChanged(QRect)),this,SLOT(onZoomableWidgetSizeChanged(QRect)));
    connect(m_pItiOtbRgbaImageWidgetFullView,SIGNAL(currentIndexChanged(QString)),m_pItiViewerPixelInfoWidget,SLOT(updateText(QString)));
    connect(m_pItiOtbRgbaImageWidgetZoomable,SIGNAL(currentIndexChanged(QString)),m_pItiViewerPixelInfoWidget,SLOT(updateText(QString)));
    connect(m_pItiOtbRgbaImageWidgetScroll,SIGNAL(currentIndexChanged(QString)),m_pItiViewerPixelInfoWidget,SLOT(updateText(QString)));
}

//!
void ItiOtbRgbaImageViewer::onScrollableWidgetSizeChanged(const QRect &size){
    //!
    m_pVisibleRegion->setRegion(size);

    m_pFocusRegion->setRegion(m_pFocusRegion->region());
}

//!
void ItiOtbRgbaImageViewer::onZoomableWidgetSizeChanged(const QRect &size){
    //!
    m_pFocusRegion->setRegion(size);
}

//!
void ItiOtbRgbaImageViewer::onFocusRegionTranslated(const QRect &rect){
    //!
    m_pFocusRegion->setRegion(rect);
}
