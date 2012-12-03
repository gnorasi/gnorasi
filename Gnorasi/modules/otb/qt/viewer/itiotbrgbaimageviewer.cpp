#include "itiotbrgbaimageviewer.h"

#include "itiotbrgbaimagewidget.h"

#include <QFileDialog>
#include <QDebug>

#include <QSplitter>
#include <QVBoxLayout>

using namespace itiviewer;

ItiOtbRgbaImageViewer::ItiOtbRgbaImageViewer(QWidget *parent) :
    ItiOtbImageViewer(parent)
{
    setupLayout();
}

void ItiOtbRgbaImageViewer::setupLayout(){
//    ItiOtbImageManager<RGBAImageType> *manager = ItiOtbImageManager<RGBAImageType>::instance();

    //!
    // create the views opengl views
    m_pItiOtbRgbaImageWidgetScroll = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pItiOtbRgbaImageWidgetFull = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetFull->setMinimumSize(160,160);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_pItiOtbRgbaImageWidgetZoom = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetZoom->setMinimumSize(160,160);
    m_pItiOtbRgbaImageWidgetScroll->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //! TODO
    //!
    //! create the histogram widget, also a widget for showing pixel info metadata
    //!
    m_pHistogramPlaceHolderWidget = new QWidget(this);
    m_pHistogramPlaceHolderWidget->setGeometry(0,0,200,200);
    m_pHistogramPlaceHolderWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //! setup the left layout
    QVBoxLayout *vBoxLayoutLeft = new QVBoxLayout();
    vBoxLayoutLeft->addWidget(m_pLabelNavigationView);
    vBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetFull);
    vBoxLayoutLeft->addWidget(m_pLabelZoomView);
    vBoxLayoutLeft->addWidget(m_pItiOtbRgbaImageWidgetZoom);
    vBoxLayoutLeft->addWidget(m_pHistogramPlaceHolderWidget);
    QWidget *pWidgetLeft = new QWidget(this);
    pWidgetLeft->setLayout(vBoxLayoutLeft);

    //! setup the right layout
    QVBoxLayout *pVboxLayoutRight = new QVBoxLayout();
    pVboxLayoutRight->addWidget(m_pLabelFullResolution);
    pVboxLayoutRight->addWidget(m_pItiOtbRgbaImageWidgetScroll);
    QWidget *pWidgetRight = new QWidget(this);
    pWidgetRight->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    pWidgetRight->setLayout(pVboxLayoutRight);

    //! setup the splitter
    QSplitter *horizontalSplitter = new QSplitter(Qt::Horizontal,this);
    horizontalSplitter->addWidget(pWidgetLeft);
    horizontalSplitter->addWidget(pWidgetRight);
    horizontalSplitter->setStretchFactor(1,1.1);


    QVBoxLayout *vboxlayout = new QVBoxLayout();
    vboxlayout->addWidget(horizontalSplitter);
    setLayout(vboxlayout);
}

//!
void ItiOtbRgbaImageViewer::disassembleWidgets(){

}

//!
void ItiOtbRgbaImageViewer::assembleWidgets(){

}

//!
void ItiOtbRgbaImageViewer::draw(){

}
