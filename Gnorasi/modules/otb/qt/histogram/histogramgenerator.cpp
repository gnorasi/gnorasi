#include "histogramgenerator.h"

#include <QDebug>

#define BINS_OVERSAMPLING_RATE 5

HistogramGenerator::HistogramGenerator(QObject *parent) :
    QObject(parent)
{

    //! the default mode is RGB mode
    m_rmode = RMODE_RGB;

    m_redChannel    = 0;
    m_greenChannel  = 1;
    m_blueChannel   = 2;
}

void HistogramGenerator::parseHistogram(){

    if(m_rmode == RMODE_GREYSCALE){
        m_greyscaleChannelData.clear();
        parseGreyscaleChannel();

    }else{
        m_redChannelData.clear();
        m_greenChannelData.clear();
        m_blueChannelData.clear();


        parseRedChannel();
        parseGreenChannel();
        parseBlueChannel();
    }
}


void HistogramGenerator::generateHistogram(VectorImageType *image){
    QTime lMain;
    QTime lPass1;
    QTime lPass2;

    lMain.start();

    qDebug() << tr( "%1: Generating histogram (I)..." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

    Q_ASSERT( image!=NULL );

    //
    // 1st pass: process min/MAX for each band.

    qDebug() << tr( "%1: Pass #1 - finding pixel min/maxes..." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

    lPass1.start();

    // Connect min/MAX pipe-section.
    typedef
      otb::StreamingMinMaxVectorImageFilter< VectorImageType >
      MinMaxFilter;

    MinMaxFilter::Pointer filterMinMax( MinMaxFilter::New() );

    filterMinMax->SetInput(image);

    filterMinMax->Update();

    /*
    // Extract min/MAX intensities for each band.
    // itk::VariableLengthVector< FLOAT_TYPE >
    typename MinMaxFilter::PixelType lSrcMin( f );
    typename MinMaxFilter::PixelType lSrcMax( filterMinMax->GetMaximum() );
    */

    // Extract-convert-remember min/MAX intensities for each band.
    m_MinPixel = filterMinMax->GetMinimum();
    m_MaxPixel = filterMinMax->GetMaximum();

    qDebug() << tr( "%1: Pass #1 - done (%2 ms)." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
      .arg( lPass1.elapsed() );

    //
    // 2nd pass: compute histogram.

    qDebug() << tr( "%1: Pass #2 - computing histogram..." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

    lPass2.start();

    // Connect histogram-generator pipe-section.
    typedef
      otb::StreamingHistogramVectorImageFilter< VectorImageType >
      HistogramFilter;

    HistogramFilter::Pointer histogramFilter( HistogramFilter::New() );

    histogramFilter->SetInput(image);

    // Setup histogram filter.
    histogramFilter->GetFilter()->SetHistogramMin( m_MinPixel );
    histogramFilter->GetFilter()->SetHistogramMax( m_MaxPixel );
//    histogramFilter->SetNumberOfBins( BINS_OVERSAMPLING_RATE * 256 );
    histogramFilter->GetFilter()->SetSubSamplingRate( 1 );

    // Go.
    histogramFilter->Update();

    qDebug() << tr( "%1: Pass #2 - done (%2 ms)." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
      .arg( lPass2.elapsed()  );

    //
    // Reference result.
    m_Histograms = histogramFilter->GetHistogramList();

    qDebug() << tr( "%1: Histogram (I) generated (%2 ms)." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
      .arg( lMain.elapsed() );

    parseHistogram();
}


void HistogramGenerator::parseGreyscaleChannel(){
    // get the histogram output
    // Get histogram of band.
    Histogram::Pointer histogram( m_Histograms->GetNthElement( m_redChannel ) );

    // get the histgram size
    const unsigned int histogramSize = histogram->Size();

    qDebug() << "Histogram size " << histogramSize << "\n";

    std::cout << "Histogram of the red component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    // iterate thgrough the histrgram and set  the values to the hash and list instaces
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, 0 );

        m_greyscaleChannelData[bin] = (double)histogram->GetFrequency( bin, 0 );
    }
}

//
void HistogramGenerator::parseRedChannel(){

    // get the histogram output
    // Get histogram of band.
    Histogram::Pointer histogram( m_Histograms->GetNthElement( m_redChannel ) );

    // get the histgram size
    const unsigned int histogramSize = histogram->Size();

    qDebug() << "Histogram size " << histogramSize << "\n";

    std::cout << "Histogram of the red component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    // iterate thgrough the histrgram and set  the values to the hash and list instaces
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, 0 );

        m_redChannelData[bin] = (double)histogram->GetFrequency( bin, 0 );
    }

    m_pRedChannelAmplitude = &helperAmpl[0];
    m_pRedChannelFrequency = &helperFreq[0];
}


void HistogramGenerator::parseGreenChannel(){
    // get the histogram output
    // Get histogram of band.
    Histogram::Pointer histogram( m_Histograms->GetNthElement( m_greenChannel ) );

    // get the histgram size
    const unsigned int histogramSize = histogram->Size();

    qDebug() << "Histogram size " << histogramSize << "\n";

    std::cout << "Histogram of the green component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    // iterate thgrough the histrgram and set  the values to the hash and list instaces
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, 0 );

        m_greenChannelData[bin] = (double)histogram->GetFrequency( bin, 0 );
    }

    m_pGreenChannelAmplitude = &helperAmpl[0];
    m_pGreenChannelFrequency = &helperFreq[0];
}


void HistogramGenerator::parseBlueChannel(){

    // get the histogram output
    // Get histogram of band.
    Histogram::Pointer histogram( m_Histograms->GetNthElement( m_blueChannel ) );

    // get the histgram size
    const unsigned int histogramSize = histogram->Size();

    qDebug() << "Histogram size " << histogramSize << "\n";

    std::cout << "Histogram of the blue component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    // iterate thgrough the histrgram and set  the values to the hash and list instaces
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, 0 );

        m_blueChannelData[bin] = (double)histogram->GetFrequency( bin, 0 );
    }

    m_pBlueChannelAmplitude = &helperAmpl[0];
    m_pBlueChannelFrequency = &helperFreq[0];
}

HistogramGenerator::~HistogramGenerator(){

}
