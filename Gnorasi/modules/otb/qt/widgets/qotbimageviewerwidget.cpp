#include "qotbimageviewerwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"

#include "../viewer/utils/itiotbimagemanager.h"
#include "../viewer/widgets/itiotbimageviewer.h"
//#include "../viewer/factories/itiotbrgbaimageviewerfactory.h"
#include "../viewer/factories/itiotbvectorimageviewerfactory.h"
#include "../viewer/widgets/panel/itiotbimageviewerpanel.h"
#include "../viewer/rgba_globaldefs.h"

#include "../viewer/utils/itiotblevel.h"

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
    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(this);
    m_pItiOtbImageViewerPanel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
//    m_pItiOtbImageViewerPanel->setMinimumHeight(270);

    //!
    m_pItiOtbImageFactory = new ItiOtbVectorImageViewerFactory(this);
    m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->viewer();

    ItiOtbImageManager *manager = m_pItiOtbImageViewer->manager();

    //
    //  ** IMPORTANT **
    //
    // the manager must be set prior to setting up the rest of the functionality , in order the viewer to work property..
    //
    m_pItiOtbImageViewerPanel->setManager(manager);

    //
    //  ** IMPORTANT **
    //
    // It is necessary also to initialize the viewer's panel
    //
    m_pItiOtbImageViewerPanel->initialize();

    // now setup the panel data , widgets etc..
    m_pItiOtbImageFactory->setupPanelData(m_pItiOtbImageViewerPanel);

    //!
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->viewer();
    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

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
        for(int i = 0 ; i < l.size(); i++)
        {
            Port *port = l.at(i);
            QString className = QString::fromStdString(port->getName());
            if(port->isConnected() && ( !className.compare(QLatin1String("IN Multi Band Image")) || !className.compare(QLatin1String("OTBImage.inport")))){
                setupByPort(port);
                break;
            }
        }
    }
}

//!
void QGLOtbImageViewerWidget::keyPressEvent(QKeyEvent *event){

    OTBImageViewerProcessor* otbivProcessor = dynamic_cast<OTBImageViewerProcessor*>(processor_);

    if(!otbivProcessor)
        return;

    std::vector<Port*> l = otbivProcessor->getInports();
    if(l.empty()){
        qDebug() << "image port list is empty..";

    }else{
        bool val = false;
        for(int i = 0 ; i < l.size(); i++)
        {
            Port *port = l.at(i);
            QString className = QString::fromStdString(port->getName());
            if(port->isConnected() && ( !className.compare(QLatin1String("IN Multi Band Image")) || !className.compare(QLatin1String("OTBImage.inport"))))
                val = true;
        }
        if(val){
            //! check key pressed
            if(event->key() == Qt::Key_F1){
                disassembleWidgets();
            }else if(event->key() == Qt::Key_F9)
                assembleWidgets();
        }
    }

    QWidget::keyPressEvent(event);
}

//!
void QGLOtbImageViewerWidget::disassembleWidgets(){

    //! Due to a lack of removing added itmes functionality on a QSplitter instance
    //! the added widgets must be firstly hidden and then deleted in order to work properly..

    //! hide widgets
    m_pItiOtbImageViewer->hide();
    m_pItiOtbImageViewerPanel->hide();

    voreen::Port *port = m_pItiOtbImageViewer->manager()->port();

    //! delete widgets
    delete m_pItiOtbImageViewer;
    delete m_pItiOtbImageViewerPanel;

    //! create the panel
    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(this);

    //set the properties
    m_pItiOtbImageViewerPanel->setWindowFlags(Qt::Window);
    m_pItiOtbImageViewerPanel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_pItiOtbImageViewerPanel->show();

    //! Create again the viewer and disassemble the sub widgets
    m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->viewer();

    ItiOtbImageManager *manager = m_pItiOtbImageViewer->manager();

    //! set the port to the image manager
    manager->setPort(port);

    // sanity check, the data in the port must not be empty
    if(!manager->isPortEmpty(port))
        manager->setupImage(); //! setup the image

    //
    //  ** IMPORTANT **
    //
    // the manager must be set prior to setting up the rest of the functionality , in order the viewer to work property..
    //
    m_pItiOtbImageViewerPanel->setManager(manager);

    //
    //  ** IMPORTANT **
    //
    // It is necessary also to initialize the viewer's panel
    //
    m_pItiOtbImageViewerPanel->initialize();

    // now setup the panel data , widgets etc..
    m_pItiOtbImageFactory->setupPanelData(m_pItiOtbImageViewerPanel);

    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->disassembleWidgets();

    //!
    m_pItiOtbImageViewer->draw();
}

//!
void QGLOtbImageViewerWidget::assembleWidgets(){

    //! Due to a lack of removing added itmes functionality on a QSplitter instance
    //! the added widgets must be firstly hidden and then deleted..

    //! hide widgets
    m_pItiOtbImageViewer->hide();
    m_pItiOtbImageViewerPanel->hide();

    voreen::Port *port = m_pItiOtbImageViewer->manager()->port();

    //! delete widgets
    delete m_pItiOtbImageViewer;
    delete m_pItiOtbImageViewerPanel;

    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(this);
    m_pItiOtbImageViewerPanel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
//    m_pItiOtbImageViewerPanel->setMinimumHeight(100);

    //! Create again the viewer and the panel
    m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->viewer();

    ItiOtbImageManager *manager = m_pItiOtbImageViewer->manager();
    //! set the port to the image manager
    manager->setPort(port);

    // sanity check, the data in the port must not be empty
    if(!manager->isPortEmpty(port)){

        //! setup the image
        manager->setupImage();
    }

    //
    //  ** IMPORTANT **
    //
    // the manager must be set prior to setting up the rest of the functionality , in order the viewer to work property..
    //
    m_pItiOtbImageViewerPanel->setManager(manager);

    //
    //  ** IMPORTANT **
    //
    // It is necessary also to initialize the viewer's panel
    //
    m_pItiOtbImageViewerPanel->initialize();

    // now setup the panel data , widgets etc..
    m_pItiOtbImageFactory->setupPanelData(m_pItiOtbImageViewerPanel);

    // setup viewer properties
    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    //! add the newly added widgets to the splitter
    m_pvSplitter->addWidget(m_pItiOtbImageViewer);
    m_pvSplitter->addWidget(m_pItiOtbImageViewerPanel);
    m_pvSplitter->setChildrenCollapsible(false);

    //!
    m_pItiOtbImageViewer->draw();
}

//!
void QGLOtbImageViewerWidget::setupByPort(Port *port){
    //! START OF INCOMING DATA USAGE

    if(!m_pItiOtbImageViewer)
        return;

    //
    //                          STATISTICS

    QTime time;
    qDebug() << "Start of incoming image data , setting up the image...";
    time.restart();

    //! get the mode of the viewer before creating a new viewer
    //! use it after the creation process has finished, to dissasemble the widgets.
    int vmode = m_pItiOtbImageViewer->vmode();

    //! create the specialized factory item , now this is the default viewer
    m_pItiOtbImageFactory = new ItiOtbVectorImageViewerFactory(this);

    //! create the discrete viewer
    createViewer();

    // get the manager from the viewer object
    ItiOtbImageManager *manager = m_pItiOtbImageViewer->manager();

    //! set the port to the image manager
    manager->setPort(port);

    // sanity check, the data in the port must not be empty
    if(!manager->isPortEmpty(port))
        manager->setupImage(); //! setup the image

    //
    //  ** IMPORTANT **
    //
    // the manager must be set prior to setting up the rest of the functionality , in order the viewer to work property..
    //
    m_pItiOtbImageViewerPanel->setManager(manager);

    // setup the panel data, commands , connections etc..
    m_pItiOtbImageFactory->setupPanelData(m_pItiOtbImageViewerPanel);

    //! setup the channels , get number etc..
    m_pItiOtbImageViewerPanel->setupChannels();
    m_pItiOtbImageViewerPanel->setupHistogram();

    //! draw stuff
    m_pItiOtbImageViewer->draw();

    //! use the visual mode of the viewer previously got and dissasemble the widgets if the viewer was previously on splitted mode
    if(vmode){
        disassembleWidgets();
        m_pItiOtbImageViewerPanel->readDisplaySettings();
    }


    //
    //                          STATISTICS

//    if(!ITIOTBIMAGEMANAGER->levels().isEmpty()){
        qDebug() << "End of image setup \nmilliseconds elapsed : " << time.elapsed()
                 /*<< "\nRegions created : " << ITIOTBIMAGEMANAGER->levels().last()->regions().size()*/;
//    }

    //
    //                          STATISTICS

    //!
    //! END OF INCOMING DATA USAGE
}

void QGLOtbImageViewerWidget::createViewer(){

    //! In order to un set a QSplitter the objects contained need to be hidden and deleted first

    //!
    m_pItiOtbImageViewer->hide();
    delete m_pItiOtbImageViewer;

    //! create a new ItiOtbImageViewer instance
    m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->viewer();

    //! set this widget as viewer's parent
    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    //! finally add the widget to the splitter
    m_pvSplitter->insertWidget(0,m_pItiOtbImageViewer);
}


QGLOtbImageViewerWidget::~QGLOtbImageViewerWidget(){
//    ItiOtbImageManager::deleteInstance();
}

} //namespace voreen
