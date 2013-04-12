#include "histogramgenerator.h"

#include <QDebug>

#define BINS_OVERSAMPLING_RATE 5


// Connect histogram-generator pipe-section.
typedef
  otb::StreamingHistogramVectorImageFilter< VectorImageType >
  HistogramFilter;

// Connect min/MAX pipe-section.
typedef
    otb::StreamingMinMaxVectorImageFilter< VectorImageType >
        MinMaxFilter;

HistogramGenerator::HistogramGenerator(QObject *parent) :
    QThread(parent)
{

    //! the default mode is RGB mode
    m_rmode = RMODE_RGB;

    m_redChannel    = 0;
    m_greenChannel  = 1;
    m_blueChannel   = 2;

    m_image = NULL;

    abort = false;
}

void HistogramGenerator::parseHistogram(){

//    if(m_rmode == RMODE_GREYSCALE){
//        m_greyscaleChannelData.clear();
//        parseGreyscaleChannel();

//    }else{
//        m_redChannelData.clear();
//        m_greenChannelData.clear();
//        m_blueChannelData.clear();

//        parseRedChannel();
//        parseGreenChannel();
//        parseBlueChannel();
//    }
}


void HistogramGenerator::generateHistogram(VectorImageType *image){


    QMutexLocker locker(&mutex);

    this->m_image = image;

    if (!isRunning()) {
        start();
    } else {
        condition.wakeOne();
    }
}


QHash<int,double> HistogramGenerator::parseGreyscaleChannel(MyHistogramList::Histogram::Pointer hsm){
    // get the histogram output
    // Get histogram of band.
    MyHistogramList::Histogram::Pointer histogram( hsm );

    QHash<int,double> data;

    // get the histgram size
    const unsigned int histogramSize = histogram->Size();

//    qDebug() << "Histogram size " << histogramSize << "\n";

//    std::cout << "Histogram of the red component" << std::endl;

    double helperFreq[BINS_OVERSAMPLING_RATE*256];
    double helperAmpl[BINS_OVERSAMPLING_RATE*256];

    // iterate thgrough the histrgram and set  the values to the hash and list instaces
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, 0 );

        data[bin] = (double)histogram->GetFrequency( bin, 0 );
    }

    return data;
}


//
QHash<int,double> HistogramGenerator::parseRedChannel(MyHistogramList::Histogram::Pointer hsm){

    // get the histogram output
    // Get histogram of band.
    MyHistogramList::Histogram::Pointer histogram( hsm);

    QHash<int,double> data;

    // get the histgram size
    const unsigned int histogramSize = histogram->Size();

//    qDebug() << "Histogram size " << histogramSize << "\n";

//    std::cout << "Histogram of the red component" << std::endl;

    double helperFreq[BINS_OVERSAMPLING_RATE*256];
    double helperAmpl[BINS_OVERSAMPLING_RATE*256];

    // iterate thgrough the histrgram and set  the values to the hash and list instaces
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, 0 );

        data[bin] = (double)histogram->GetFrequency( bin, 0 );
    }

//    m_pRedChannelAmplitude = &helperAmpl[0];
//    m_pRedChannelFrequency = &helperFreq[0];

    return data;
}


QHash<int,double> HistogramGenerator::parseGreenChannel(MyHistogramList::Histogram::Pointer hsm){
    // get the histogram output
    // Get histogram of band.
    MyHistogramList::Histogram::Pointer histogram( hsm );

    QHash<int,double> data;

    // get the histgram size
    const unsigned int histogramSize = histogram->Size();

//    qDebug() << "Histogram size " << histogramSize << "\n";

//    std::cout << "Histogram of the green component" << std::endl;

    double helperFreq[BINS_OVERSAMPLING_RATE*256];
    double helperAmpl[BINS_OVERSAMPLING_RATE*256];

    // iterate thgrough the histrgram and set  the values to the hash and list instaces
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, 0 );

        data[bin] = (double)histogram->GetFrequency( bin, 0 );
    }

//    m_pGreenChannelAmplitude = &helperAmpl[0];
//    m_pGreenChannelFrequency = &helperFreq[0];

    return data;
}


QHash<int,double> HistogramGenerator::parseBlueChannel(MyHistogramList::Histogram::Pointer hsm){

    // get the histogram output
    // Get histogram of band.
    MyHistogramList::Histogram::Pointer histogram( hsm );

    QHash<int,double> data;

    // get the histgram size
    const unsigned int histogramSize = histogram->Size();

//    qDebug() << "Histogram size " << histogramSize << "\n";

//    std::cout << "Histogram of the blue component" << std::endl;

    double helperFreq[BINS_OVERSAMPLING_RATE*256];
    double helperAmpl[BINS_OVERSAMPLING_RATE*256];

    // iterate thgrough the histrgram and set  the values to the hash and list instaces
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, 0 );

        data[bin] = (double)histogram->GetFrequency( bin, 0 );
    }

//    m_pBlueChannelAmplitude = &helperAmpl[0];
//    m_pBlueChannelFrequency = &helperFreq[0];

    return data;
}


void HistogramGenerator::run(){

    mutex.lock();
    VectorImageType::PixelType minPixel = this->m_MinPixel;
    VectorImageType::PixelType maxPixel = this->m_MaxPixel;
    VectorImageType *img                = this->m_image;
    unsigned int cgyc                   = this->m_currentGreyChannel;
    unsigned int crc                    = this->m_redChannel;
    unsigned int cgnc                   = this->m_greenChannel;
    unsigned int cbc                    = this->m_blueChannel;
    RMODE mode                          = this->m_rmode;
    mutex.unlock();

    QTime lMain;
    QTime lPass1;
    QTime lPass2;

    while(!abort){

        lMain.restart();

        qDebug() << tr( "%1: Generating histogram (I)..." )
          .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

        Q_ASSERT( img!=NULL );

        //
        // 1st pass: process min/MAX for each band.

        qDebug() << tr( "%1: Pass #1 - finding pixel min/maxes..." )
          .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

        lPass1.restart();

        MinMaxFilter::Pointer filterMinMax( MinMaxFilter::New() );

        filterMinMax->SetInput(img);
        filterMinMax->GetFilter()->SetNoDataFlag(true);
        filterMinMax->Update();

        /*
        // Extract min/MAX intensities for each band.
        // itk::VariableLengthVector< FLOAT_TYPE >
        typename MinMaxFilter::PixelType lSrcMin( f );
        typename MinMaxFilter::PixelType lSrcMax( filterMinMax->GetMaximum() );
        */

        // Extract-convert-remember min/MAX intensities for each band.
        minPixel = filterMinMax->GetMinimum();
        maxPixel = filterMinMax->GetMaximum();

        qDebug() << tr( "%1: Pass #1 - done (%2 ms)." )
          .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
          .arg( lPass1.elapsed() );

        //
        // 2nd pass: compute histogram.

        qDebug() << tr( "%1: Pass #2 - computing histogram..." )
          .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

        lPass2.restart();

        HistogramFilter::Pointer histogramFilter( HistogramFilter::New() );

        histogramFilter->SetInput(img);

        // Setup histogram filter.
        histogramFilter->GetFilter()->SetHistogramMin( minPixel );
        histogramFilter->GetFilter()->SetHistogramMax( maxPixel );
        histogramFilter->GetFilter()->SetNumberOfBins( BINS_OVERSAMPLING_RATE* 256 );
        histogramFilter->GetFilter()->SetSubSamplingRate( 1 );

        // Go.
        histogramFilter->Update();

        qDebug() << tr( "%1: Pass #2 - done (%2 ms)." )
          .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
          .arg( lPass2.elapsed()  );

        MyHistogramList::HistogramList::Pointer histograms(histogramFilter->GetHistogramList());

        //
        // Reference result.

        qDebug() << tr( "%1: Histogram (I) generated (%2 ms)." )
          .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
          .arg( lMain.elapsed() );


        MyHistogramList *list = new MyHistogramList();
        list->setHistogramList(histograms);

        if(mode == RMODE_GREYSCALE){
            MyHistogramList::Histogram::Pointer greyhsm(histograms->GetNthElement(cgyc));
            QHash<int,double> greycd    = parseGreyscaleChannel(greyhsm);
            list->setGreyChannelData(greycd);
        }else{
            MyHistogramList::Histogram::Pointer redhsm(histograms->GetNthElement(crc));
            MyHistogramList::Histogram::Pointer greenhsm(histograms->GetNthElement(cgnc));
            MyHistogramList::Histogram::Pointer bluehsm(histograms->GetNthElement(cbc));

            QHash<int,double> redcd     = parseRedChannel(redhsm);
            QHash<int,double> greencd   = parseGreenChannel(greenhsm);
            QHash<int,double> bluecd    = parseBlueChannel(bluehsm);

            list->setBlueChannelData(bluecd);
            list->setGreenChannelData(greencd);
            list->setRedChannelData(redcd);
        }

        mutex.lock();
        emit histogramGeneratedFinished(list);

        condition.wait(&mutex);
        mutex.unlock();
    }
}

HistogramGenerator::~HistogramGenerator(){
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}
