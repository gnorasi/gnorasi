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
    QVBoxLayout *layout = new QVBoxLayout;

    m_pButton = new QPushButton(tr("Generate histogram"), this);

    m_pHistogramView = new HistogramView(this);

    layout->addWidget(m_pButton);
    layout->addWidget(m_pHistogramView);

    setLayout(layout);

    connect(m_pButton,SIGNAL(clicked()),this,SLOT(onClicked()));
}


void ItiOtbImageViewerPanelHistogramTab::onClicked()
{
    HistogramGenerator *pHG = new HistogramGenerator(this);

    QString path = m_pItiOtbImageViewerPanel->manager()->imageFile();

    pHG->generateHistogram(path);

    QHash<int,double> rcd = pHG->redChannelData();
    QHash<int,double> gcd = pHG->redChannelData();
    QHash<int,double> bcd = pHG->blueChannelData();

    m_pHistogramView->setupRedChannel(rcd.size(), rcd);
    m_pHistogramView->setupGreenChannel(gcd.size(), gcd);
    m_pHistogramView->setupBlueChannel(bcd.size(), bcd);

    m_pHistogramView->replot();
}
