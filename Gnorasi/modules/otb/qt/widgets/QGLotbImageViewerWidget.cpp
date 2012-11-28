#include "QGLotbImageViewerWidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "otbQGLImageWidget.h"

#include "otbImageFileReader.h"

#include "../../ports/otbimageport.h"

#include <QFileDialog>
#include <QDebug>

#include <QSplitter>

using namespace otb;

typedef otb::ImageFileReader<ImageType>   ReaderType;

typedef ImageType::Pointer ImagePointer;

namespace voreen {

const std::string QGLOtbImageViewerWidget::loggerCat_("voreen.QGLOtbImageViewerWidget");

QGLOtbImageViewerWidget::QGLOtbImageViewerWidget(QWidget *parent, OTBImageViewerProcessor *otbImageViewerProcessor)
    : QProcessorWidget(otbImageViewerProcessor, parent)
{
    tgtAssert(otbImageViewerProcessor, "No QGLOtbImageViewerWidget processor");

    setWindowTitle(QString::fromStdString(otbImageViewerProcessor->getName()));
    resize(600, 400);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void QGLOtbImageViewerWidget::initialize(){
    QProcessorWidget::initialize();

    m_pQGLImageWidgetMain = new QGLImageWidget(this);
    m_pQGLImageWidgetQuick = new QGLImageWidget(this);
    m_pQGLImageWidgetZoom = new QGLImageWidget(this);

    m_pHistogramPlaceHolderWidget = new QWidget(this);
    m_pHistogramPlaceHolderWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QWidget *pWidgetLeft = new QWidget(this);

    QSplitter *horizontalSplitterLeft = new QSplitter(Qt::Vertical,this);
    horizontalSplitterLeft->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    horizontalSplitterLeft->addWidget(m_pQGLImageWidgetQuick);
    horizontalSplitterLeft->addWidget(m_pQGLImageWidgetZoom);

    QVBoxLayout *vBoxLayoutLeft = new QVBoxLayout();
    vBoxLayoutLeft->addWidget(horizontalSplitterLeft);
    vBoxLayoutLeft->addWidget(m_pHistogramPlaceHolderWidget);

    pWidgetLeft->setLayout(vBoxLayoutLeft);

    QSplitter *horizontalSplitter = new QSplitter(Qt::Horizontal,this);

    horizontalSplitter->addWidget(pWidgetLeft);
    horizontalSplitter->addWidget(m_pQGLImageWidgetMain);

    QVBoxLayout *vboxlayout = new QVBoxLayout();

    vboxlayout->addWidget(horizontalSplitter);

    setLayout(vboxlayout);

    initialized_ = true;

    hide();
}


//!
void QGLOtbImageViewerWidget::updateFromProcessor(){
    OTBImageViewerProcessor* otbivProcessor = dynamic_cast<OTBImageViewerProcessor*>(processor_);

    if(!otbivProcessor)
        return;

    std::vector<Port*> l = otbivProcessor->getInports();
    if(l.empty()){


    }else{
        OTBImagePort *pP = dynamic_cast<OTBImagePort*>(l.at(0));

        if(!pP)
            return;

        QString filePath = QString::fromStdString(otbivProcessor->getFilePath());

        if(!filePath.isEmpty()){

            ReaderType::Pointer reader = ReaderType::New();
            reader->SetFileName(otbivProcessor->getFilePath());
            reader->Update();
//            reader->GenerateOutputInformation();

            ImageType::Pointer image = reader->GetOutput();
            RegionType region = image->GetLargestPossibleRegion();


            m_pQGLImageWidgetMain->SetIsotropicZoom(1);
            m_pQGLImageWidgetQuick->SetIsotropicZoom(0.5);
            m_pQGLImageWidgetMain->ReadBuffer(image, region);
            m_pQGLImageWidgetQuick->ReadBuffer(image,region);

            show();
        }else{
            ImageType *imageType = (ImageType*)pP->getData();
            if(!imageType)
                return;

            RegionType region = imageType->GetLargestPossibleRegion();


            m_pQGLImageWidgetMain->SetIsotropicZoom(1);
            m_pQGLImageWidgetQuick->SetIsotropicZoom(0.5);
            m_pQGLImageWidgetMain->ReadBuffer(imageType,region);
        }
    }
}

//QString QGLOtbImageViewerWidget::constructMetadataInfo(){
//    QString text;

//    qDebug() <<  QString::fromStdString(m_pImageType->GetProjectionRef());

//    return text;
//}

//void QGLOtbImageViewerWidget::mouseMoveEvent(QMouseEvent *event){
//    qDebug() << event->pos();

//    QWidget::mouseMoveEvent(event);
//}

} //namespace voreen
