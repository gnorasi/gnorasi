#include "qotbimageviewerwidget.h"

#include "voreen/qt/voreenapplicationqt.h"

#include "../../ports/otbimageport.h"
#include "../../ports/otbvectorimageport.h"

#include "../viewer/utils/itiotbimagemanager.h"
#include "../viewer/widgets/itiotbimageviewer.h"
//#include "../viewer/factories/itiotbrgbaimageviewerfactory.h"
#include "../viewer/factories/itiotbvectorimageviewerfactory.h"
#include "../viewer/widgets/panel/itiotbimageviewerpanel.h"
#include "../viewer/commands/commandcolorcomposition.h"
#include "../viewer/commands/commandcontrastenhancement.h"
//#include "../viewer/utils/itiotbimagergbachannelprovider.h"
#include "../viewer/utils/itiotbimagevectorchannelprovider.h"
#include "../viewer/rgba_globaldefs.h"

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
    m_pItiOtbImageFactory = new ItiOtbVectorImageViewerFactory(this);
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    //!
    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(this);
    m_pItiOtbImageViewerPanel->setMinimumHeight(270);

    //! create a splitter and add the widgets
    m_pvSplitter = new QSplitter(Qt::Vertical,this);
    m_pvSplitter->addWidget(m_pItiOtbImageViewer);
    m_pvSplitter->addWidget(m_pItiOtbImageViewerPanel);
    m_pvSplitter->setChildrenCollapsible(false);

    //! setup commands
    setupCommands();

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

        //! check if the port is connected, only then setup the viewer by the port
        if(pPort->isConnected())
            setupByPort(pPort);
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
        Port *pPort = l.at(0);
        if(pPort->isConnected()){
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
    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(this);
    m_pItiOtbImageViewerPanel->setWindowFlags(Qt::Window);
    m_pItiOtbImageViewerPanel->show();

    //! setup commands
    setupCommands();

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

    //! delete widgets
    delete m_pItiOtbImageViewer;
    delete m_pItiOtbImageViewerPanel;

    //! Create again the viewer and the panel
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->createViewer();
    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    m_pItiOtbImageViewerPanel = new ItiOtbImageViewerPanel(this);
    m_pItiOtbImageViewerPanel->setMinimumHeight(100);

    //! add the newly added widgets to the splitter
    m_pvSplitter->addWidget(m_pItiOtbImageViewer);
    m_pvSplitter->addWidget(m_pItiOtbImageViewerPanel);
    m_pvSplitter->setChildrenCollapsible(false);

    //! setup commands
    setupCommands();

    //!
    m_pItiOtbImageViewer->draw();
}

//!
void QGLOtbImageViewerWidget::setupCommands(){
    Q_ASSERT(m_pItiOtbImageViewer);
    Q_ASSERT(m_pItiOtbImageViewerPanel);

    //! create command color composition
    CommandColorComposition *pCmdCC = new CommandColorComposition(m_pItiOtbImageViewer,this);
    connect(m_pItiOtbImageViewerPanel,SIGNAL(greyScaleColorCompositionChannelChanged(int)),pCmdCC,SLOT(setGreyScaleMethod(int)));
    connect(m_pItiOtbImageViewerPanel,SIGNAL(rgbColorCompositionChannelsChanged(int,int,int)),pCmdCC,SLOT(setRGBMethod(int,int,int)));
    m_pItiOtbImageViewerPanel->setCommand(ItiOtbImageViewerPanel::SLOT_CC, pCmdCC);

    //! create command for contrast enhancenment
    CommandContrastEnhancement *pCmdCE = new CommandContrastEnhancement(m_pItiOtbImageViewer,this);
    connect(m_pItiOtbImageViewerPanel,SIGNAL(contrastEnhancementChanged(int,double,double)),pCmdCE,SLOT(setContrastEnhancementMethod(int,double,double)));
    m_pItiOtbImageViewerPanel->setCommand(ItiOtbImageViewerPanel::SLOT_CE,pCmdCE);
}

//!
void QGLOtbImageViewerWidget::setupByPort(Port *port){
    //! START OF INCOMING DATA USAGE

    //! set the port to the image manager
    ITIOTBIMAGEMANAGER->setPort(port);

    //! setup the image
    ITIOTBIMAGEMANAGER->setupImage();

    //! create the specialized factory item
    m_pItiOtbImageFactory = new ItiOtbVectorImageViewerFactory(this);

    //! create the specialized provider
    m_pItiOtbImageViewerPanel->setProvider(new ItiOtbImageVectorChannelProvider(ITIOTBIMAGEMANAGER->image(),this));

    //! setup the channels , get number etc..
    m_pItiOtbImageViewerPanel->setupChannels();

    //! create the appropriate viewer
    createViewer();

    //! draw stuff
    m_pItiOtbImageViewer->draw();

    //!
    //! END OF INCOMING DATA USAGE
}

void QGLOtbImageViewerWidget::createViewer(){

    //! In order to un set a QSplitter the objects contained need to be hidden and deleted first

    //!
    m_pItiOtbImageViewer->hide();
    delete m_pItiOtbImageViewer;

    //! create a new ItiOtbImageViewer instance
    m_pItiOtbImageViewer = m_pItiOtbImageFactory->createViewer();

    //! set this widget as viewer's parent
    m_pItiOtbImageViewer->setParent(this);
    m_pItiOtbImageViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    //! setup commands
    setupCommands();

    //! finally add the widget to the splitter
    m_pvSplitter->insertWidget(0,m_pItiOtbImageViewer);
}

QGLOtbImageViewerWidget::~QGLOtbImageViewerWidget(){
    ItiOtbImageManager::deleteInstance();
}

} //namespace voreen
