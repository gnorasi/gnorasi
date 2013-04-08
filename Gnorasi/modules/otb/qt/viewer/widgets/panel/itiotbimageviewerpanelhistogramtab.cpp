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

    // instanciate the objects
    m_pHistogramViewRed         = new HistogramView(this);
    m_pHistogramViewBlue        = new HistogramView(this);
    m_pHistogramViewGreen       = new HistogramView(this);
    m_pHistogramViewGreyscale   = new HistogramView(this);

    // setup titles and stuff
    m_pHistogramViewBlue->setTitle(tr("Blue Channel"));
    m_pHistogramViewGreen->setTitle(tr("Green Channel"));
    m_pHistogramViewRed->setTitle(tr("Red Channel"));
    m_pHistogramViewGreyscale->setTitle(tr("Greyscale Channel"));

    // set color
    m_pHistogramViewBlue->setColor(QColor(Qt::blue));
    m_pHistogramViewGreen->setColor(QColor(Qt::green));
    m_pHistogramViewRed->setColor(QColor(Qt::red));

    // set Axis
    m_pHistogramViewRed->setAxisTitle(QwtPlot::yLeft, tr("Number of pixels"));

    // set colors
    m_pHistogramViewGreyscale->setColor(QColor(Qt::darkGray));

    // set visiblities, at first rgb mode is set by default
    m_pHistogramViewGreyscale->setVisible(false);

    // setup the layout
    layout->addWidget(m_pHistogramViewRed);
    layout->addWidget(m_pHistogramViewGreen);
    layout->addWidget(m_pHistogramViewBlue);
    layout->addWidget(m_pHistogramViewGreyscale);

    // set the layout
    setLayout(layout);
}

void ItiOtbImageViewerPanelHistogramTab::setupHistogram(){

    // get the imge and perform an error check
    VectorImageType *image = m_pItiOtbImageViewerPanel->manager()->image();

    if(!image )
        return;

    if(m_pItiOtbImageViewerPanel->isGreyscale()){

        // get the graychannel value from the panel
        int currentGreyscaleChannel = m_pItiOtbImageViewerPanel->currentGreyscaleChannel();

        // set the channel to the histogram generator
        m_pHistogramGenerator->setCurrentGreyChannel(currentGreyscaleChannel);

        // set the mode to greyscale
        m_pHistogramGenerator->setRMode(HistogramGenerator::RMODE_GREYSCALE);

        // generate the histogram
        m_pHistogramGenerator->generateHistogram(image);

        // set visibilities
        m_pHistogramViewBlue->setVisible(false);
        m_pHistogramViewGreen->setVisible(false);
        m_pHistogramViewRed->setVisible(false);

        m_pHistogramViewGreyscale->setVisible(true);

        // get the data
        QHash<int,double> grcd = m_pHistogramGenerator->greyscaleChannelData();

        // setup the data and replot
        m_pHistogramViewGreyscale->setupData(grcd);
        m_pHistogramViewGreyscale->replot();
    }
    else{

        // set selected channels
        m_pHistogramGenerator->setCurrentRGBChannels(m_pItiOtbImageViewerPanel->currentRedChannel()-1,
                                                     m_pItiOtbImageViewerPanel->currentGreenChannel()-1,
                                                     m_pItiOtbImageViewerPanel->currentBlueChannel()-1);

        // set the RGB mode
        m_pHistogramGenerator->setRMode(HistogramGenerator::RMODE_RGB);

        // generate the histogam
        m_pHistogramGenerator->generateHistogram(image);

        // set visibilities
        m_pHistogramViewGreyscale->setVisible(false);
        m_pHistogramViewBlue->setVisible(true);
        m_pHistogramViewGreen->setVisible(true);
        m_pHistogramViewRed->setVisible(true);

        // get channel data
        QHash<int,double> rcd = m_pHistogramGenerator->redChannelData();
        QHash<int,double> gcd = m_pHistogramGenerator->greenChannelData();
        QHash<int,double> bcd = m_pHistogramGenerator->blueChannelData();

        // setup the data
        m_pHistogramViewBlue->setupData(bcd);
        m_pHistogramViewGreen->setupData(gcd);
        m_pHistogramViewRed->setupData(rcd);

        // now replot the histogram
        m_pHistogramViewRed->replot();
        m_pHistogramViewGreen->replot();
        m_pHistogramViewBlue->replot();
    }

    m_pItiOtbImageViewerPanel->manager()->setHistogramGenerator(m_pHistogramGenerator);
}

