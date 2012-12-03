#include "qotbimageviewerwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "otbImageFileReader.h"

#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"


#include "../viewer/itiotbimagemanager.h"
#include "../viewer/itiotbimageviewer.h"
#include "../viewer/itiotbrgbaimageviewerfactory.h"
#include "../viewer/itiotbimageviewerpanel.h"

using namespace otb;
using namespace itiviewer;

namespace voreen {

const std::string QGLOtbImageViewerWidget::loggerCat_("voreen.QGLOtbImageViewerWidget");

QGLOtbImageViewerWidget::QGLOtbImageViewerWidget(QWidget *parent, OTBImageViewerProcessor *otbImageViewerProcessor)
    : QProcessorWidget(otbImageViewerProcessor, parent)
{
    tgtAssert(otbImageViewerProcessor, "No QOtbImageViewerWidget processor");

    setWindowTitle(QString::fromStdString(otbImageViewerProcessor->getName()));
    resize(600, 500);
    setPosition(QApplication::desktop()->width()/2 - 200,QApplication::desktop()->height()/2 - 200);
}

//!
void QGLOtbImageViewerWidget::initialize(){
    QProcessorWidget::initialize();

    QVBoxLayout *layout = new QVBoxLayout(this);

    m_pItiOtbImageFactory = new ItiOtbRgbaImageViewerFactory(this);
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(this);

    m_pvSplitter = new QSplitter(Qt::Vertical,this);
    m_pvSplitter->addWidget(m_pItiOtbImageViewer);
    m_pvSplitter->addWidget(m_pItiOtbImageViewerPanel);

    layout->addWidget(m_pvSplitter);

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
        qDebug() << "image port list is empty..";

    }else{
        //! get the first port
        Port *pPort = l.at(0);


        //! START OF INCOMING DATA USAGE

        //! TODO
        //!
        //! Here should be established incoming image data usage protocol
        //!

        //! set the port to the image manager
        ItiOtbImageManager *mgr = ItiOtbImageManager::instance();
        mgr->setImagePort(pPort);

        //! type case checking
        if(dynamic_cast<OTBImagePort*>(pPort)){ // set here the raster image port
            m_pItiOtbImageFactory = new ItiOtbRgbaImageViewerFactory(this);
        }
        else if(dynamic_cast<OTBVectorImagePort*>(pPort)){ // set here the vector image factory

        }

        //! create a new ItiOtbImageViewer instance
        m_pItiOtbImageViewer = m_pItiOtbImageFactory->createViewer();

        //! draw stuff
        m_pItiOtbImageViewer->draw();

        //!
        //! END OF INCOMING DATA USAGE
    }
}

//!
void QGLOtbImageViewerWidget::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_F1){
        disassembleWidgets();
    }else if(event->key() == Qt::Key_F9)
        assembleWidgets();

    QWidget::keyPressEvent(event);
}

//!
void QGLOtbImageViewerWidget::disassembleWidgets(){

    //! TODO
    // setup the functionality for spliting all layouts and setup all widgets
    // into a seperate window

}

//!
void QGLOtbImageViewerWidget::assembleWidgets(){
    //! TODO
    // setup the functionality for merging all layouts and setup all widgets
    // into one single window
}

QGLOtbImageViewerWidget::~QGLOtbImageViewerWidget(){
    ItiOtbImageManager::deleteInstance();
}

} //namespace voreen
