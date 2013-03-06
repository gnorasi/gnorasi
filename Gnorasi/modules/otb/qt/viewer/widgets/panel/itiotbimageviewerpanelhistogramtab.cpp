#include "itiotbimageviewerpanelhistogramtab.h"
#include "itiotbimageviewerpanel.h"
#include "../../utils/itiotbimagemanager.h"

#include "../../utils/itiotbimagechannelprovider.h"

#include "../../../histogram/histogramgenerator.h"
#include "../../../histogram/histogramview.h"

using namespace itiviewer;

ItiOtbImageViewerPanelHistogramTab::ItiOtbImageViewerPanelHistogramTab(ItiOtbImageViewerPanel *panel) :
    m_pItiOtbImageViewerPanel(panel)
{
    setWindowTitle(tr("Histogram"));

    initialize();
}

//!
void ItiOtbImageViewerPanelHistogramTab::initialize(){
    m_pHistogramGenerator = new HistogramGenerator(this);

    QVBoxLayout *layout = new QVBoxLayout;

    m_pHistogramView = new HistogramView(this);

    layout->addWidget(m_pHistogramView);

    setLayout(layout);
}

void ItiOtbImageViewerPanelHistogramTab::setupHistogram(){

//    QString path = m_pItiOtbImageViewerPanel->manager()->imageFile();
    VectorImageType *image = m_pItiOtbImageViewerPanel->manager()->image();


    if(m_pItiOtbImageViewerPanel->isGreyscale()){
        int currentGreyscaleChannel = m_pItiOtbImageViewerPanel->currentGreyscaleChannel();
        m_pHistogramGenerator->setCurrentGreyChannel(currentGreyscaleChannel);
        m_pHistogramGenerator->setRMode(HistogramGenerator::RMODE_GREYSCALE);
        m_pHistogramGenerator->generateHistogram(image);

        m_pHistogramView->setupGreyScaleMode();

        QHash<int,double> grcd = m_pHistogramGenerator->greyscaleChannelData();

        m_pHistogramView->setupGreyChannel(grcd);
    }
    else{
        m_pHistogramGenerator->setRMode(HistogramGenerator::RMODE_RGB);
        m_pHistogramGenerator->generateHistogram(image);

        m_pHistogramView->setupRGBMode();

        QHash<int,double> rcd = m_pHistogramGenerator->redChannelData();
        QHash<int,double> gcd = m_pHistogramGenerator->greenChannelData();
        QHash<int,double> bcd = m_pHistogramGenerator->blueChannelData();

        m_pHistogramView->setupRedChannel(rcd);
        m_pHistogramView->setupGreenChannel(gcd);
        m_pHistogramView->setupBlueChannel(bcd);
    }

    m_pHistogramView->replot();
}

