#include "itiotbrgbaimageviewer.h"

#include "itiotbrgbaresizablewidget.h"
#include "itiotbrgbascrollablewidget.h"
#include "itiotbrgbazoomablewidget.h"
#include "../utils/itiotbimagemanager.h"
#include "itiotbrgbaqglwidget.h"
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
    //!
    setupLayout();

    //!
    setupObserverMechanism();
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
    m_pItiOtbRgbaImageWidgetScroll      = new ItiOtbRgbaScrollableWidget(this);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pItiOtbRgbaImageWidgetResizable   = new ItiOtbRgbaResizableWidget(this);
    m_pItiOtbRgbaImageWidgetResizable->setMinimumSize(160,160);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_pItiOtbRgbaImageWidgetZoom        = new ItiOtbRgbaZoomableWidget(this);
    m_pItiOtbRgbaImageWidgetZoom->setMinimumSize(160,160);
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
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetResizable);
    m_pvBoxLayoutLeft->addWidget(m_pLabelZoomView);
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetZoom);
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
    m_pFocusRegion->registerObserver(m_pItiOtbRgbaImageWidgetZoom);
    m_pVisibleRegion                    = new ItiViewerObservableRegion(this);
    m_pVisibleRegion->registerObserver(m_pItiOtbRgbaImageWidgetScroll);
    m_pVisibleRegion->registerObserver(m_pItiOtbRgbaImageWidgetResizable);
}

/*!
 * \brief ItiOtbRgbaImageViewer::disassembleWidgets
 */
void ItiOtbRgbaImageViewer::disassembleWidgets(){
    m_pMainLayout->removeWidget(m_pSplitter);

    //! Due to a lack of removing added itmes functionality on a QSplitter instance
    //! the added widgets must be firstly hidden and then deleted..
    m_pItiOtbRgbaImageWidgetResizable->hide();
    m_pItiOtbRgbaImageWidgetScroll->hide();
    m_pItiOtbRgbaImageWidgetZoom->hide();
    delete m_pItiOtbRgbaImageWidgetResizable;
    delete m_pItiOtbRgbaImageWidgetScroll;
    delete m_pItiOtbRgbaImageWidgetZoom;

    //! create the widgets
    m_pItiOtbRgbaImageWidgetScroll      = new ItiOtbRgbaScrollableWidget(this);
    m_pItiOtbRgbaImageWidgetScroll->setWindowFlags(Qt::Window);
    m_pItiOtbRgbaImageWidgetScroll->setWindowTitle(m_pLabelScrollableResolution->text());
    m_pItiOtbRgbaImageWidgetScroll->setGeometry(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200,200,200);
    m_pItiOtbRgbaImageWidgetScroll->show();
    m_pItiOtbRgbaImageWidgetResizable   = new ItiOtbRgbaResizableWidget(this);
    m_pItiOtbRgbaImageWidgetResizable->setWindowFlags(Qt::Window);
    m_pItiOtbRgbaImageWidgetResizable->setWindowTitle(m_pLabelFullView->text());
    m_pItiOtbRgbaImageWidgetResizable->setGeometry(QApplication::desktop()->width()/2 - 160,QApplication::desktop()->height()/2 - 160,200,200);
    m_pItiOtbRgbaImageWidgetResizable->show();
    m_pItiOtbRgbaImageWidgetZoom        = new ItiOtbRgbaZoomableWidget(this);
    m_pItiOtbRgbaImageWidgetZoom->setWindowFlags(Qt::Window);
    m_pItiOtbRgbaImageWidgetZoom->setWindowTitle(m_pLabelZoomView->text());
    m_pItiOtbRgbaImageWidgetZoom->show();
    m_pItiOtbRgbaImageWidgetZoom->setGeometry(QApplication::desktop()->width()/2 - 120,QApplication::desktop()->height()/2 - 120,200,200);
    m_pMetadataWidget                   = new QWidget(this);
    m_pMetadataWidget->setWindowFlags(Qt::Window);
    m_pMetadataWidget->setWindowTitle(m_pLabelMetadataView->text());
    m_pMetadataWidget->setGeometry(QApplication::desktop()->width()/2 - 80,QApplication::desktop()->height()/2 - 80,200,200);
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
}

/*!
 * \brief ItiOtbRgbaImageViewer::draw
 *  This is where all the painting is done.
 */
void ItiOtbRgbaImageViewer::draw(){
    //!
    m_pItiOtbRgbaImageWidgetScroll->draw();

    //!
    m_pItiOtbRgbaImageWidgetResizable->draw();

    //!
    m_pItiOtbRgbaImageWidgetZoom->draw();
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
