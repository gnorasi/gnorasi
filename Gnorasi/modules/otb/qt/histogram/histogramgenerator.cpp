#include "histogramgenerator.h"

#include <QDebug>

HistogramGenerator::HistogramGenerator(QObject *parent) :
    QObject(parent)
{
    histogramGenerator  = HistogramGeneratorType::New();
    reader              = ReaderType::New();
}


void HistogramGenerator::generateHistogram(const QString &path){
    reader->SetFileName(path.toStdString());
    try
    {
        reader->Update();

        m_redChannelData.clear();
        m_greenChannelData.clear();
        m_blueChannelData.clear();

        parseRedChannel();
        parseGreenChannel();
        parseBlueChannel();
    }
    catch( itk::ExceptionObject & excp )
    {
        qDebug() << "Problem encoutered while reading image file : " << path << "\n";
        return;
    }
}


void HistogramGenerator::parseRedChannel(){
    SizeType size;

    size[0] = 255;        // number of bins for the Red   channel
    size[1] =   1;        // number of bins for the Green channel
    size[2] =   1;        // number of bins for the Blue  channel

    histogramGenerator->SetNumberOfBins( size );

    histogramGenerator->SetMarginalScale( 10.0 );

    histogramGenerator->SetInput(  reader->GetOutput()  );

    histogramGenerator->Compute();

    histogram = histogramGenerator->GetOutput();

    const unsigned int histogramSize = histogram->Size();

    qDebug() << "Histogram size " << histogramSize << "\n";

    unsigned int channel = 0;  // red channel

    std::cout << "Histogram of the red component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;
        helperAmpl[bin] = (double)histogram->GetFrequency( bin, channel );

        m_redChannelData[bin] = (double)histogram->GetFrequency( bin, channel );
    }

    m_pRedChannelAmplitude = &helperAmpl[0];
    m_pRedChannelFrequency = &helperFreq[0];
}


void HistogramGenerator::parseGreenChannel(){
    SizeType size;

    size[0] =   1;  // number of bins for the Red   channel
    size[1] = 255;  // number of bins for the Green channel
    size[2] =   1;  // number of bins for the Blue  channel

    histogramGenerator->SetNumberOfBins( size );

    histogramGenerator->Compute();

    const unsigned int histogramSize = histogram->Size();

    unsigned int channel = 1;  // green channel

    std::cout << "Histogram of the green component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;
        helperAmpl[bin] = (double)histogram->GetFrequency( bin, channel );

        m_greenChannelData[bin] = (double)histogram->GetFrequency( bin, channel );
    }

    m_pGreenChannelAmplitude = &helperAmpl[0];
    m_pGreenChannelFrequency = &helperFreq[0];
}


void HistogramGenerator::parseBlueChannel(){
    SizeType size;

    size[0] =   1;  // number of bins for the Red   channel
    size[1] =   1;  // number of bins for the Green channel
    size[2] =   255;  // number of bins for the Blue  channel

    histogramGenerator->SetNumberOfBins( size );

    histogramGenerator->Compute();

    const unsigned int histogramSize = histogram->Size();

    unsigned int channel = 2;  // green channel

    std::cout << "Histogram of the blue component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;
        helperAmpl[bin] = (double)histogram->GetFrequency( bin, channel );

        m_blueChannelData[bin] = (double)histogram->GetFrequency( bin, channel );
    }

    m_pBlueChannelAmplitude = &helperAmpl[0];
    m_pBlueChannelFrequency = &helperFreq[0];
}
