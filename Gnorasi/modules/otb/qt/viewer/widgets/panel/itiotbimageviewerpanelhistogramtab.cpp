#include "itiotbimageviewerpanelhistogramtab.h"
#include "itiotbimageviewerpanel.h"
#include "../../utils/itiotbimagemanager.h"

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

    QString path = m_pItiOtbImageViewerPanel->manager()->imageFile();

    m_pHistogramGenerator->generateHistogram(path);

    QHash<int,double> rcd = m_pHistogramGenerator->redChannelData();
    QHash<int,double> gcd = m_pHistogramGenerator->greenChannelData();
    QHash<int,double> bcd = m_pHistogramGenerator->blueChannelData();

    m_pHistogramView->setupRedChannel(rcd.size(), rcd);
    m_pHistogramView->setupGreenChannel(gcd.size(), gcd);
    m_pHistogramView->setupBlueChannel(bcd.size(), bcd);

    m_pHistogramView->replot();
}

