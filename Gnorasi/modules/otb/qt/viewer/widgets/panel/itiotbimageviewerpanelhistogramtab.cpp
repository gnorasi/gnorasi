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

    QHBoxLayout *layout = new QHBoxLayout;

    m_pHistogramViewRed         = new HistogramView(this);
    m_pHistogramViewBlue        = new HistogramView(this);
    m_pHistogramViewGreen       = new HistogramView(this);
    m_pHistogramViewGreyscale   = new HistogramView(this);

    m_pHistogramViewBlue->setTitle(tr("Blue Channel"));
    m_pHistogramViewGreen->setTitle(tr("Green Channel"));
    m_pHistogramViewRed->setTitle(tr("Red Channel"));
    m_pHistogramViewGreyscale->setTitle(tr("Greyscale Channel"));

    m_pHistogramViewBlue->setColor(QColor(Qt::blue));
    m_pHistogramViewGreen->setColor(QColor(Qt::green));
    m_pHistogramViewRed->setColor(QColor(Qt::red));

    m_pHistogramViewGreyscale->setColor(QColor(Qt::darkGray));

    m_pHistogramViewGreyscale->setVisible(false);

    layout->addWidget(m_pHistogramViewRed);
    layout->addWidget(m_pHistogramViewBlue);
    layout->addWidget(m_pHistogramViewGreen);

    layout->addWidget(m_pHistogramViewGreyscale);

    setLayout(layout);
}

void ItiOtbImageViewerPanelHistogramTab::setupHistogram(){

    VectorImageType *image = m_pItiOtbImageViewerPanel->manager()->image();


    if(m_pItiOtbImageViewerPanel->isGreyscale()){

        int currentGreyscaleChannel = m_pItiOtbImageViewerPanel->currentGreyscaleChannel();
        m_pHistogramGenerator->setCurrentGreyChannel(currentGreyscaleChannel);
        m_pHistogramGenerator->setRMode(HistogramGenerator::RMODE_GREYSCALE);
        m_pHistogramGenerator->generateHistogram(image);

        m_pHistogramViewBlue->setVisible(false);
        m_pHistogramViewGreen->setVisible(false);
        m_pHistogramViewRed->setVisible(false);

        m_pHistogramViewGreyscale->setVisible(true);

        QHash<int,double> grcd = m_pHistogramGenerator->greyscaleChannelData();

        m_pHistogramViewGreyscale->setupData(grcd);
        m_pHistogramViewGreyscale->replot();
    }
    else{
        m_pHistogramGenerator->setRMode(HistogramGenerator::RMODE_RGB);
        m_pHistogramGenerator->generateHistogram(image);

        m_pHistogramViewGreyscale->setVisible(false);
        m_pHistogramViewBlue->setVisible(true);
        m_pHistogramViewGreen->setVisible(true);
        m_pHistogramViewRed->setVisible(true);

        QHash<int,double> rcd = m_pHistogramGenerator->redChannelData();
        QHash<int,double> gcd = m_pHistogramGenerator->greenChannelData();
        QHash<int,double> bcd = m_pHistogramGenerator->blueChannelData();

        m_pHistogramViewBlue->setupData(bcd);
        m_pHistogramViewGreen->setupData(gcd);
        m_pHistogramViewRed->setupData(rcd);

        m_pHistogramViewRed->replot();
        m_pHistogramViewGreen->replot();
        m_pHistogramViewBlue->replot();
    }
}

