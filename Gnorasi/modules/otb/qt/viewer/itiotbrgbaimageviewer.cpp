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
    // create the views
    m_pItiOtbRgbaImageWidgetScroll = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetFull = new ItiOtbRgbaImageWidget(this);
    m_pItiOtbRgbaImageWidgetZoom = new ItiOtbRgbaImageWidget(this);

    m_pHistogramPlaceHolderWidget = new QWidget(this);
    m_pHistogramPlaceHolderWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QWidget *pWidgetLeft = new QWidget(this);

    QSplitter *verticalSplitterLeft = new QSplitter(Qt::Vertical,this);
    verticalSplitterLeft->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    verticalSplitterLeft->addWidget(m_pItiOtbRgbaImageWidgetFull);
    verticalSplitterLeft->addWidget(m_pItiOtbRgbaImageWidgetZoom);

    QVBoxLayout *vBoxLayoutLeft = new QVBoxLayout();
    vBoxLayoutLeft->addWidget(verticalSplitterLeft);
    vBoxLayoutLeft->addWidget(m_pHistogramPlaceHolderWidget);

    pWidgetLeft->setLayout(vBoxLayoutLeft);

    QSplitter *horizontalSplitter = new QSplitter(Qt::Horizontal,this);

    horizontalSplitter->addWidget(pWidgetLeft);
    horizontalSplitter->addWidget(m_pItiOtbRgbaImageWidgetScroll);

    QVBoxLayout *vboxlayout = new QVBoxLayout();

    vboxlayout->addWidget(horizontalSplitter);

    setLayout(vboxlayout);
}


//void ItiOtbRgbaImageViewer::setCenter(IndexType index){
//    QOTBImageManager *mgr = QOTBImageManager::instance();

//    mgr->model()->SetExtractRegionCenter(index);
//    mgr->model()->SetScaledExtractRegionCenter(index);
//    mgr->model()->Update();
//}


void ItiOtbRgbaImageViewer::clearLayout(){

}
