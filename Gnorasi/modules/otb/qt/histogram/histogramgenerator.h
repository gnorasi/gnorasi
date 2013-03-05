#ifndef HISTOGRAMGENERATOR_H
#define HISTOGRAMGENERATOR_H

#include <QObject>

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkImageToHistogramGenerator.h"
#include "itkImage.h"
#include "itkRGBPixel.h"

#include "itkImageFileReader.h"

#include <QHash>

const unsigned int                            _Dimension = 2;

class HistogramGenerator : public QObject
{
    Q_OBJECT
public:

    typedef unsigned char                                   PixelComponentType;


    typedef itk::RGBPixel< PixelComponentType >             RGBPixelType;


    typedef itk::Image< RGBPixelType, _Dimension >          RGBImageType;


    typedef itk::Statistics::ImageToHistogramGenerator<
                                RGBImageType >              HistogramGeneratorType;


    typedef HistogramGeneratorType::SizeType                SizeType;


    typedef itk::ImageFileReader< RGBImageType >  ReaderType;


    typedef HistogramGeneratorType::HistogramType  HistogramType;


    explicit HistogramGenerator(QObject *parent = 0);

    /*!
     * \brief generateHistogram
     * \param path
     */
    void generateHistogram(const QString &path);


    double* redChannelFrequency() const { return m_pRedChannelFrequency; }
    double* greenChannelFrequency() const { return m_pGreenChannelFrequency; }
    double* blueChannelFrequency() const { return m_pBlueChannelFrequency; }

    double* redChannelAmplitude() const { return m_pRedChannelAmplitude; }
    double* greenChannelAmplitude() const { return m_pGreenChannelAmplitude; }
    double* blueChannelAmplitude() const { return m_pBlueChannelAmplitude; }

    QHash<int,double> redChannelData() const { return m_redChannelData; }
    QHash<int,double> greenChannelData() const { return m_greenChannelData; }
    QHash<int,double> blueChannelData() const { return m_blueChannelData; }

    
signals:
    
public slots:

private:
    HistogramGeneratorType::Pointer histogramGenerator;

    ReaderType::Pointer reader;

    const HistogramType * histogram;

    /*!
     * \brief parseRedChannel
     */
    void parseRedChannel();

    /*!
     * \brief parseGreenChannel
     */
    void parseGreenChannel();

    /*!
     * \brief parseBlueChannel
     */
    void parseBlueChannel();

    double *m_pRedChannelFrequency;
    double *m_pGreenChannelFrequency;
    double *m_pBlueChannelFrequency;

    double *m_pRedChannelAmplitude;
    double *m_pGreenChannelAmplitude;
    double *m_pBlueChannelAmplitude;

    QHash<int, double> m_redChannelData;
    QHash<int, double> m_greenChannelData;
    QHash<int, double> m_blueChannelData;

};

#endif // HISTOGRAMGENERATOR_H
