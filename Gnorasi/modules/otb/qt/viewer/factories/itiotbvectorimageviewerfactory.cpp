#include "itiotbvectorimageviewerfactory.h"

#include "../widgets/vector/itiotbvectorimageviewer.h"

#include "../widgets/panel/itiotbimageviewerpanel.h"
#include "../widgets/panel/itiotbimageviewerpanelsetuptab.h"

#include "../commands/commandcontrastenhancementgaussian.h"
#include "../commands/commandcontrastenhancementlinear0255.h"
#include "../commands/commandcontrastenhancementlinearxperc.h"
#include "../commands/commandcontrastenhancementsquareroot.h"
#include "../commands/commandcolorcompositiongreyscale.h"
#include "../commands/commandcolorcompositionrgb.h"
#include "../commands/commandtoggleclassificationlabelvisibiltiy.h"

#include "../utils/itiotbimagevectorchannelprovider.h"

#include "../models/itiotbVectorImageModel.h"

using namespace itiviewer;

ItiOtbVectorImageViewerFactory::ItiOtbVectorImageViewerFactory(QObject *parent)
    : ItiOtbImageViewerFactory(parent)
{
}

//!
void ItiOtbVectorImageViewerFactory::createViewer(){
    m_pItiOtbImageViewer = new ItiOtbVectorImageViewer();
}

void ItiOtbVectorImageViewerFactory::setupPanelData(ItiOtbImageViewerPanel *panel){
    VectorImageModel *model = new VectorImageModel(m_pItiOtbImageViewer->manager(),this);

    m_pItiOtbImageViewer->setModel(model);

    ItiOtbImageVectorChannelProvider *provider = new ItiOtbImageVectorChannelProvider(model,this);
    panel->setProvider(provider);

    ItiOtbImageViewerPanelSetupTab *sTab = panel->setupTab();

    if(sTab){
        Command *cmdGaussian        = createCommandContrastEnhancementGaussian(sTab);
        if(cmdGaussian)
            panel->setCommand(ItiOtbImageViewerPanel::SLOT_CE_GAUSSIAN,cmdGaussian);

        Command *cmdLinear0_255     = createCommandContrastEnhancementLinear0_255(sTab);
        if(cmdLinear0_255)
            panel->setCommand(ItiOtbImageViewerPanel::SLOT_CE_LINEAR_0_255,cmdLinear0_255);

        Command *cmdLinearXPerc     = createCommandContrastEnhancementLinearXPerc(sTab);
        if(cmdLinearXPerc)
            panel->setCommand(ItiOtbImageViewerPanel::SLOT_CE_LINEARXPERC,cmdLinearXPerc);

        Command *cmdSquareRoot      = createCommandContrastEnhancementSquareRoot(sTab);
        if(cmdSquareRoot)
            panel->setCommand(ItiOtbImageViewerPanel::SLOT_CE_SQUAREROOT,cmdSquareRoot);

        Command *cmdSquareGreyscale = createCommandColorCompositionGreyscale(sTab);
        if(cmdSquareRoot)
            panel->setCommand(ItiOtbImageViewerPanel::SLOT_CC_GREYSCALE,cmdSquareGreyscale);

        Command *cmdSquareRGB = createCommandColorCompositionRGB(sTab);
        if(cmdSquareRoot)
            panel->setCommand(ItiOtbImageViewerPanel::SLOT_CC_RGB,cmdSquareRGB);

        Command *cmdTV              = createCommandToggleClassLabelVisible(sTab);
        if(cmdTV)
            panel->setCommand(ItiOtbImageViewerPanel::SLOT_CL_TOGGLEVISIBLE,cmdTV);

        connect(provider,SIGNAL(channelsChanged()),panel,SLOT(setupChannels()));
        connect(provider,SIGNAL(channelsChanged()),panel,SLOT(setupHistogram()));
    }
}


Command* ItiOtbVectorImageViewerFactory::createCommandContrastEnhancementGaussian(ItiOtbImageViewerPanelSetupTab *sTab){
    ItiOtbVectorImageViewer *vv  = qobject_cast<ItiOtbVectorImageViewer*>(m_pItiOtbImageViewer);

    if(!vv)
        return 0;

    CommandContrastEnhancementGaussian *cg = new CommandContrastEnhancementGaussian(vv,this);

    connect(sTab, SIGNAL(deviationChanged(double)),cg, SLOT(updateDeviation(double)));

    return cg;
}


Command* ItiOtbVectorImageViewerFactory::createCommandContrastEnhancementLinear0_255(ItiOtbImageViewerPanelSetupTab *sTab){
    ItiOtbVectorImageViewer *vv  = qobject_cast<ItiOtbVectorImageViewer*>(m_pItiOtbImageViewer);

    if(!vv)
        return 0;

    CommandContrastEnhancementLinear0255 *cl1 = new CommandContrastEnhancementLinear0255(vv,this);

    return cl1;
}


Command* ItiOtbVectorImageViewerFactory::createCommandContrastEnhancementLinearXPerc(ItiOtbImageViewerPanelSetupTab *sTab){
    ItiOtbVectorImageViewer *vv  = qobject_cast<ItiOtbVectorImageViewer*>(m_pItiOtbImageViewer);

    if(!vv)
        return 0;

    CommandContrastEnhancementLinearXPerc *cl2 = new CommandContrastEnhancementLinearXPerc(vv,this);

    connect(sTab,SIGNAL(lowerQuantileChanged(double)),cl2,SLOT(updateLowerQuantile(double)));
    connect(sTab,SIGNAL(upperQuantileChanged(double)),cl2,SLOT(updateUpperQuantile(double)));

    return cl2;
}


Command* ItiOtbVectorImageViewerFactory::createCommandContrastEnhancementSquareRoot(ItiOtbImageViewerPanelSetupTab *sTab){
    ItiOtbVectorImageViewer *vv  = qobject_cast<ItiOtbVectorImageViewer*>(m_pItiOtbImageViewer);

    if(!vv)
        return 0;

    CommandContrastEnhancementSquareRoot *cs = new CommandContrastEnhancementSquareRoot(vv,this);

    connect(sTab,SIGNAL(lowerQuantileChanged(double)),cs,SLOT(updateLowerQuantile(double)));
    connect(sTab,SIGNAL(upperQuantileChanged(double)),cs,SLOT(updateUpperQuantile(double)));

    return cs;
}


Command* ItiOtbVectorImageViewerFactory::createCommandColorCompositionGreyscale(ItiOtbImageViewerPanelSetupTab *sTab){
    ItiOtbVectorImageViewer *vv  = qobject_cast<ItiOtbVectorImageViewer*>(m_pItiOtbImageViewer);

    if(!vv)
        return 0;

    CommandColorCompositionGreyscale *cg = new CommandColorCompositionGreyscale(vv,this);

    connect(sTab,SIGNAL(greyScaleColorCompositionChannelChanged(int)),cg,SLOT(updateChannel(int)));

    return cg;
}

Command* ItiOtbVectorImageViewerFactory::createCommandColorCompositionRGB(ItiOtbImageViewerPanelSetupTab *sTab){
    ItiOtbVectorImageViewer *vv  = qobject_cast<ItiOtbVectorImageViewer*>(m_pItiOtbImageViewer);

    if(!vv)
        return 0;

    CommandColorCompositionRGB *cg = new CommandColorCompositionRGB(vv,this);

    connect(sTab,SIGNAL(rgbColorCompositionChannelsChanged(int,int,int)),cg,SLOT(updateChannels(int,int,int)));

    return cg;
}


Command* ItiOtbVectorImageViewerFactory::createCommandToggleClassLabelVisible(ItiOtbImageViewerPanelSetupTab *sTab){
    ItiOtbVectorImageViewer *vv  = qobject_cast<ItiOtbVectorImageViewer*>(m_pItiOtbImageViewer);

    if(!vv)
        return 0;

    CommandToggleClassificationLabelVisibiltiy *cd = new CommandToggleClassificationLabelVisibiltiy(vv,this);

    connect(sTab,SIGNAL(classLabelToggled(bool,int)),cd,SLOT(updateData(bool,int)));

    return cd;

}

ItiOtbVectorImageViewerFactory::~ItiOtbVectorImageViewerFactory(){

}
