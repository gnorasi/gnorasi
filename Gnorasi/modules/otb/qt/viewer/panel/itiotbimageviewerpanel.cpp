#include "itiotbimageviewerpanel.h"

#include "itiotbimageviewerpaneldatatab.h"
#include "itiotbimageviewerpanelsetuptab.h"
#include "itiotbimageviewerpanelhistogramtab.h"
#include "itiotbimageviewerpanelpixeldescriptiontab.h"
#include "../widgets/itiotbimageviewer.h"
#include "../commands/command.h"
#include "../vector_globaldefs.h"
#include "../utils/itiotbimagemanager.h"

#include <QVBoxLayout>


using namespace itiviewer;

ItiOtbImageViewerPanel::ItiOtbImageViewerPanel(QWidget *parent) :
    QWidget(parent)
{
    initialize();
}

//!
void ItiOtbImageViewerPanel::initialize(){

    //! the setu tab
    m_pSetupTab = new ItiOtbImageViewerPanelSetupTab(this);

    //! the tab widget
    m_pTabWidget = new QTabWidget(this);

    //! add widgets to the tab
    m_pTabWidget->addTab(m_pSetupTab,m_pSetupTab->windowTitle());

    //! setup connections
    connect(m_pSetupTab,SIGNAL(colorCompositionApplyButtonClicked()),this,SLOT(applyColorComposition()));
    connect(m_pSetupTab,SIGNAL(contrastEnhancementApplyButtonClicked()),this,SLOT(applyConstrastEnhancement()));
    //! connections to establish updating command parameters
    connect(m_pSetupTab,SIGNAL(contrastEnhancementChanged(int,double,double)),this,SIGNAL(contrastEnhancementChanged(int,double,double)));
    connect(m_pSetupTab,SIGNAL(greyScaleColorCompositionChannelChanged(int)),SIGNAL(greyScaleColorCompositionChannelChanged(int)));
    connect(m_pSetupTab,SIGNAL(rgbColorCompositionChannelsChanged(int,int,int)),SIGNAL(rgbColorCompositionChannelsChanged(int,int,int)));

    //! setup layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_pTabWidget);
    setLayout(layout);
}

//!
void ItiOtbImageViewerPanel::applyColorComposition(){
    Command *pCommand = m_commandHash[(int)SLOT_CC];
    if(pCommand)
        pCommand->execute();
}

//!
void ItiOtbImageViewerPanel::applyConstrastEnhancement(){
    Command *pCommand = m_commandHash[(int)SLOT_CE];
    if(pCommand)
        pCommand->execute();
}

//!
void ItiOtbImageViewerPanel::setCommand(SLOT_KEY cmdk, Command *pC){
    m_commandHash[(int)cmdk] = pC;
}

//!
void ItiOtbImageViewerPanel::setupChannels(){
    VectorImageType *img = ITIOTBIMAGEMANAGER->image();
    if(!img)
        return;
}
