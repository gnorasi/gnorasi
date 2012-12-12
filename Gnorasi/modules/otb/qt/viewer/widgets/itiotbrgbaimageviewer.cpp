#include "itiotbrgbaimageviewer.h"

#include "itiotbrgbaqglwidgetfullview.h"
#include "itiotbrgbaqglwidgetscrollable.h"
#include "itiotbrgbaqglwidgetzoomable.h"
#include "../utils/itiotbimagemanager.h"
#include "itiotbrgbaqglwidgetscrollable.h"
#include "itiotbrgbaqglwidgetzoomable.h"
#include "itiotbrgbaqglwidgetfullview.h"
#include "../observables/itiviewerobservableregion.h"

#include "../../../ports/otbimageport.h"

#include "otbImage.h"

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

#include <QFileDialog>
#include <QDebug>

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
    // initialize instances
    m_pLabelScrollableResolution        = new QLabel(tr("Scrollable View"),this);
    m_pLabelFullView                    = new QLabel(tr("Full View"),this);
    m_pLabelZoomView                    = new QLabel(tr("Zoomable View"),this);
    m_pLabelMetadataView                = new QLabel(tr("Metadata View"),this);

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
    m_pMetadataWidget                   = new QWidget(this);
    m_pMetadataWidget->setGeometry(0,0,160,160);
    m_pMetadataWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    //! setup the left layout
    m_pvBoxLayoutLeft                   = new QVBoxLayout();
    m_pvBoxLayoutLeft->addWidget(m_pLabelFullView);
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetFullView);
    m_pvBoxLayoutLeft->addWidget(m_pLabelZoomView);
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetZoomable);
    m_pvBoxLayoutLeft->addWidget(m_pLabelMetadataView);
    m_pvBoxLayoutLeft->addWidget(m_pMetadataWidget);
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

//!
void ItiOtbRgbaImageViewer::setupObserverMechanism(){
    //! setup observer mechanism
    m_pFocusRegion                      = new ItiViewerObservableRegion(this);
    m_pFocusRegion->registerObserver(m_pItiOtbRgbaImageWidgetScroll);
    m_pFocusRegion->registerObserver(m_pItiOtbRgbaImageWidgetZoomable);
    m_pVisibleRegion                    = new ItiViewerObservableRegion(this);
    m_pVisibleRegion->registerObserver(m_pItiOtbRgbaImageWidgetScroll);
    m_pVisibleRegion->registerObserver(m_pItiOtbRgbaImageWidgetFullView);
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
    m_pItiOtbRgbaImageWidgetZoomable        = new ItiOtbRgbaQGLWidgetZoomable(this);
    m_pItiOtbRgbaImageWidgetZoomable->setWindowFlags(Qt::Window);
    m_pItiOtbRgbaImageWidgetZoomable->setWindowTitle(m_pLabelZoomView->text());
    m_pItiOtbRgbaImageWidgetZoomable->show();
    m_pItiOtbRgbaImageWidgetZoomable->setGeometry(QApplication::desktop()->width()/2 - 120,QApplication::desktop()->height()/2 - 120,200,200);
    m_pMetadataWidget                   = new QWidget(this);
    m_pMetadataWidget->setWindowFlags(Qt::Window);
    m_pMetadataWidget->setWindowTitle(m_pLabelMetadataView->text());
    m_pMetadataWidget->setGeometry(QApplication::desktop()->width()/2 - 80,QApplication::desktop()->height()/2 - 80,200,200);

    //!
    setupObserverMechanism();

    //!
    setupConnections();

    //!
    m_vmode                             = VMODE_SPLITTED;

    //!
    m_pMetadataWidget->show();
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
void ItiOtbRgbaImageViewer::setupConnections(){
    //!
    connect(m_pItiOtbRgbaImageWidgetScroll, SIGNAL(visibleAreaChanged(QRect)),this,SLOT(onScrollableWidgetSizeChanged(QRect)));
    connect(m_pItiOtbRgbaImageWidgetZoomable, SIGNAL(visibleAreaChanged(QRect)),this,SLOT(onZoomableWidgetSizeChanged(QRect)));
}

//!
void ItiOtbRgbaImageViewer::onScrollableWidgetSizeChanged(const QRect &size){
    qDebug() << size;

//    QRect rect = m_pVisibleRegion->region();
//    rect.setWidth(size.width());
//    rect.setHeight(size.height());

    m_pVisibleRegion->setRegion(size);
}


void ItiOtbRgbaImageViewer::onZoomableWidgetSizeChanged(const QRect &size){

    QRect rect = m_pVisibleRegion->region();
    rect.setWidth(size.width());
    rect.setHeight(size.height());

    m_pFocusRegion->setRegion(rect);
}
