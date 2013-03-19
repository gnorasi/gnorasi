/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

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

#include "otbImageFileWriter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "../viewer/vector_globaldefs.h"

#include <QHash>

const unsigned int                            _Dimension = 2;

class HistogramGenerator : public QObject
{
    Q_OBJECT
public:

    enum RMODE{
        RMODE_GREYSCALE = 0,
        RMODE_RGB       = 1
    };


    typedef unsigned char                                   PixelComponentType;


    typedef itk::RGBPixel< PixelComponentType >             RGBPixelType;


    typedef itk::Image< RGBPixelType, _Dimension >          RGBImageType;


    typedef itk::Statistics::ImageToHistogramGenerator<
                                RGBImageType >              HistogramGeneratorType;


    typedef HistogramGeneratorType::SizeType                SizeType;


    typedef itk::ImageFileReader< RGBImageType >  ReaderType;


    typedef HistogramGeneratorType::HistogramType  HistogramType;


    explicit HistogramGenerator(QObject *parent = 0);


    ~HistogramGenerator();


    /*!
     * \brief generateHistogram
     * \param path
     */
    void generateHistogram(const QString &path);

    /*!
     * \brief generateHistogram
     *  Oh, there is one thing Greek people say in such an occassion..
     *  Yet another path knows the good fellow..
     *  So dummy implementation, the problem was that the histogram filter was not been able to be
     *  implemented in the gnorasi..
     * \param image
     */
    void generateHistogram(VectorImageType *image);


    double* redChannelFrequency() const { return m_pRedChannelFrequency; }
    double* greenChannelFrequency() const { return m_pGreenChannelFrequency; }
    double* blueChannelFrequency() const { return m_pBlueChannelFrequency; }

    double* redChannelAmplitude() const { return m_pRedChannelAmplitude; }
    double* greenChannelAmplitude() const { return m_pGreenChannelAmplitude; }
    double* blueChannelAmplitude() const { return m_pBlueChannelAmplitude; }

    QHash<int,double> greyscaleChannelData() const { return m_greyscaleChannelData; }
    QHash<int,double> redChannelData() const { return m_redChannelData; }
    QHash<int,double> greenChannelData() const { return m_greenChannelData; }
    QHash<int,double> blueChannelData() const { return m_blueChannelData; }


    //OTB specific stuff***************************************************
    //Default 32 bit image writer
    typedef VectorImageType* 		     ImagePointer;
    typedef otb::ImageFileWriter<VectorImageType> WriterType;
    WriterType::Pointer writer;


    void setRMode(RMODE rmode) { m_rmode = rmode; }

    RMODE rmode() { return m_rmode; }

    void setCurrentGreyChannel(int c) { m_currentGreyChannel = c; }

    //
    void setCurrentRGBChannels(unsigned int red, unsigned int green, unsigned int blue) { m_redChannel = red; m_greenChannel = green; m_blueChannel = blue; }
    
signals:
    
public slots:

private:
    HistogramGeneratorType::Pointer histogramGenerator;

    ReaderType::Pointer reader;

    const HistogramType * histogram;

    /*!
     * \brief parseGreyscaleChannel
     */
    void parseGreyscaleChannel();


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


    QString constructNewName();


    void clearImageRepo();

    double *m_pRedChannelFrequency;
    double *m_pGreenChannelFrequency;
    double *m_pBlueChannelFrequency;

    double *m_pRedChannelAmplitude;
    double *m_pGreenChannelAmplitude;
    double *m_pBlueChannelAmplitude;

    QHash<int, double> m_greyscaleChannelData;
    QHash<int, double> m_redChannelData;
    QHash<int, double> m_greenChannelData;
    QHash<int, double> m_blueChannelData;

    unsigned int m_redChannel;
    unsigned int m_greenChannel;
    unsigned int m_blueChannel;

    QDir m_dir;

    RMODE m_rmode;

    int m_currentGreyChannel;

};

#endif // HISTOGRAMGENERATOR_H
