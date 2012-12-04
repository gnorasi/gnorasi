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
    setToolTip(tr("Press F1 to dissasembe sub widgets into seperate windows, press F9 to assemble widgets into one single window."));
}

//!
void QGLOtbImageViewerWidget::initialize(){
    QProcessorWidget::initialize();

    //!
    m_pItiOtbImageFactory = new ItiOtbRgbaImageViewerFactory(this);
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    //!
    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(m_pItiOtbImageViewer, this);
    m_pItiOtbImageViewerPanel->setMinimumHeight(270);

    //! create a splitter and add the widgets
    m_pvSplitter = new QSplitter(Qt::Vertical,this);
    m_pvSplitter->addWidget(m_pItiOtbImageViewer);
    m_pvSplitter->addWidget(m_pItiOtbImageViewerPanel);
    m_pvSplitter->setChildrenCollapsible(false);

    //! create a vertical box layout and add the splitter on it
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_pvSplitter);
    this->setLayout(layout);

    //!
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

    //! check key pressed
    if(event->key() == Qt::Key_F1){
        disassembleWidgets();
    }else if(event->key() == Qt::Key_F9)
        assembleWidgets();

    QWidget::keyPressEvent(event);
}

//!
void QGLOtbImageViewerWidget::disassembleWidgets(){

    //! Due to a lack of removing added itmes functionality on a QSplitter instance
    //! the added widgets must be firstly hidden and then deleted..

    //! hide widgets
    m_pItiOtbImageViewer->hide();
    m_pItiOtbImageViewerPanel->hide();

    //! delete widgets
    delete m_pItiOtbImageViewer;
    delete m_pItiOtbImageViewerPanel;

    //! Create again the viewer and disassemble the sub widgets
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->disassembleWidgets();

    //! create the panel
    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(m_pItiOtbImageViewer, this);
    m_pItiOtbImageViewerPanel->setWindowFlags(Qt::Window);
    m_pItiOtbImageViewerPanel->show();

}

//!
void QGLOtbImageViewerWidget::assembleWidgets(){

    //! Due to a lack of removing added itmes functionality on a QSplitter instance
    //! the added widgets must be firstly hidden and then deleted..

    //! hide widgets
    m_pItiOtbImageViewer->hide();
    m_pItiOtbImageViewerPanel->hide();

    //! delete widgets
    delete m_pItiOtbImageViewer;
    delete m_pItiOtbImageViewerPanel;

    //! Create again the viewer and the panel
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(m_pItiOtbImageViewer, this);
    m_pItiOtbImageViewerPanel->setMinimumHeight(100);

    //! add the newly added widgets to the splitter
    m_pvSplitter->addWidget(m_pItiOtbImageViewer);
    m_pvSplitter->addWidget(m_pItiOtbImageViewerPanel);
    m_pvSplitter->setChildrenCollapsible(false);

}

QGLOtbImageViewerWidget::~QGLOtbImageViewerWidget(){
    ItiOtbImageManager::deleteInstance();
}

} //namespace voreen
