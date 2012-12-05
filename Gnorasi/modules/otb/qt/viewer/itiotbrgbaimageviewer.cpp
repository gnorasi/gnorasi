#include "itiotbrgbaimageviewer.h"

#include "itiotbrgbaimagewidget.h"

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

#include <QFileDialog>
#include <QDebug>

using namespace itiviewer;

ItiOtbRgbaImageViewer::ItiOtbRgbaImageViewer(QWidget *parent) :
    ItiOtbImageViewer(parent)
{
    setupLayout();
}

void ItiOtbRgbaImageViewer::setupLayout(){
    // initialize instances
    m_pLabelFullResolution              = new QLabel(tr("Full Resolution View"),this);
    m_pLabelNavigationView              = new QLabel(tr("Navigation View"),this);
    m_pLabelZoomView                    = new QLabel(tr("Zoom View"),this);
    m_pLabelMetadataView                = new QLabel(tr("Metadata View"),this);

    //! setup the resize policy
    m_pLabelFullResolution->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pLabelNavigationView->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pLabelZoomView->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    //!
    // create the views opengl views
    m_pItiOtbRgbaImageWidgetScroll      = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pItiOtbRgbaImageWidgetFull        = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetFull->setMinimumSize(160,160);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_pItiOtbRgbaImageWidgetZoom        = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetZoom->setMinimumSize(160,160);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //! TODO
    //!
    //! create the metadata widget, also a widget for showing pixel info metadata
    //!
    m_pMetadataWidget                   = new QWidget(this);
    m_pMetadataWidget->setGeometry(0,0,200,200);
    m_pMetadataWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //! setup the left layout
    m_pvBoxLayoutLeft                   = new QVBoxLayout();
    m_pvBoxLayoutLeft->addWidget(m_pLabelNavigationView);
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetFull);
    m_pvBoxLayoutLeft->addWidget(m_pLabelZoomView);
    m_pvBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetZoom);
    m_pvBoxLayoutLeft->addWidget(m_pLabelMetadataView);
    m_pvBoxLayoutLeft->addWidget(m_pMetadataWidget);
    QWidget *pWidgetLeft                = new QWidget(this);
    pWidgetLeft->setLayout(m_pvBoxLayoutLeft);

    //! setup the right layout
    m_pVboxLayoutRight                  = new QVBoxLayout();
    m_pVboxLayoutRight->addWidget(m_pLabelFullResolution);
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
void ItiOtbRgbaImageViewer::disassembleWidgets(){
    m_pMainLayout->removeWidget(m_pSplitter);

    //! Due to a lack of removing added itmes functionality on a QSplitter instance
    //! the added widgets must be firstly hidden and then deleted..
    m_pItiOtbRgbaImageWidgetFull->hide();
    m_pItiOtbRgbaImageWidgetScroll->hide();
    m_pItiOtbRgbaImageWidgetZoom->hide();
    delete m_pItiOtbRgbaImageWidgetFull;
    delete m_pItiOtbRgbaImageWidgetScroll;
    delete m_pItiOtbRgbaImageWidgetZoom;

    //! create the widgets
    m_pItiOtbRgbaImageWidgetScroll      = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetScroll->setWindowFlags(Qt::Window);
    m_pItiOtbRgbaImageWidgetScroll->setWindowTitle(m_pLabelFullResolution->text());
    m_pItiOtbRgbaImageWidgetScroll->setGeometry(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200,200,200);
    m_pItiOtbRgbaImageWidgetScroll->show();
    m_pItiOtbRgbaImageWidgetFull        = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetFull->setWindowFlags(Qt::Window);
    m_pItiOtbRgbaImageWidgetFull->setWindowTitle(m_pLabelNavigationView->text());
    m_pItiOtbRgbaImageWidgetFull->setGeometry(QApplication::desktop()->width()/2 - 160,QApplication::desktop()->height()/2 - 160,200,200);
    m_pItiOtbRgbaImageWidgetFull->show();
    m_pItiOtbRgbaImageWidgetZoom        = new ItiOtbRgbaImageWidget(this);
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

//!
void ItiOtbRgbaImageViewer::assembleWidgets(){

}

//!
void ItiOtbRgbaImageViewer::draw(){

}

//!
void ItiOtbRgbaImageViewer::applyGreyScaleColorMode(int band){
    Q_UNUSED(band)
}

//!
void ItiOtbRgbaImageViewer::applyRGBColorMode(int red, int green, int blue){
    Q_UNUSED(red)
    Q_UNUSED(green)
    Q_UNUSED(blue)
}

void ItiOtbRgbaImageViewer::applyContrastEnhancementMethod(CC ce, double aval, double bval){
    Q_UNUSED(ce)
    Q_UNUSED(aval)
    Q_UNUSED(bval)
}
