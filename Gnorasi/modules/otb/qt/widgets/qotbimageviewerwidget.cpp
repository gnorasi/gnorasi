#include "qotbimageviewerwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "otbImageFileReader.h"

#include "../../ports/otbimageport.h"

#include "qotbimageviewer.h"
#include "../utils/qotbimagemanager.h"

using namespace otb;


typedef ImageType::Pointer ImagePointer;

namespace voreen {

const std::string QGLOtbImageViewerWidget::loggerCat_("voreen.QGLOtbImageViewerWidget");

QGLOtbImageViewerWidget::QGLOtbImageViewerWidget(QWidget *parent, OTBImageViewerProcessor *otbImageViewerProcessor)
    : QProcessorWidget(otbImageViewerProcessor, parent)
{
    tgtAssert(otbImageViewerProcessor, "No QOtbImageViewerWidget processor");

    setWindowTitle(QString::fromStdString(otbImageViewerProcessor->getName()));
    resize(600, 400);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void QGLOtbImageViewerWidget::initialize(){
    QProcessorWidget::initialize();

    QVBoxLayout *layout = new QVBoxLayout(this);

    m_pQOTBImageViewer = new QOTBImageViewer(this);

    layout->addWidget(m_pQOTBImageViewer);

    this->setLayout(layout);

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

        QOTBImageManager *manager = QOTBImageManager::instance();

        if(!filePath.isEmpty()){

            manager->load(filePath);

            m_pQOTBImageViewer->update();

            show();
        }else{
            ImageType *imageType = (ImageType*)pP->getData();
            if(!imageType)
                return;

            manager->load(imageType);

            m_pQOTBImageViewer->update();

            show();
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


QGLOtbImageViewerWidget::~QGLOtbImageViewerWidget(){
    QOTBImageManager::deleteInstance();
}

} //namespace voreen
