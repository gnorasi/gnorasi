#include "QGLotbImageViewerWidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "otbQGLImageWidget.h"

#include "otbImageFileReader.h"

#include "../../ports/otbimageport.h"

#include <QFileDialog>
#include <QDebug>

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
    resize(400, 400);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void QGLOtbImageViewerWidget::initialize(){
    QProcessorWidget::initialize();

    m_pQGLImageWidget = new QGLImageWidget(this);

    QVBoxLayout *vboxlayout = new QVBoxLayout();

    vboxlayout->addWidget(m_pQGLImageWidget);

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


            m_pQGLImageWidget->SetIsotropicZoom(2);
            m_pQGLImageWidget->ReadBuffer(image, region);

            show();
        }else{
            ImageType *imageType = (ImageType*)pP->getData();
            if(!imageType)
                return;

            RegionType region = imageType->GetLargestPossibleRegion();


            m_pQGLImageWidget->SetIsotropicZoom(1);
            m_pQGLImageWidget->ReadBuffer(imageType,region);
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
