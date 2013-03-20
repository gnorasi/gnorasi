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

const unsigned int _Dimension                           = 2;

/*!
 * \brief The HistogramGenerator class encapsulates the HistogramGenerator object from the ITK library.
 *  The ITK HistogramGenerator class can generate Histogram objects for a given image. For more information
 *  please visit the ITK web page at \link http://www.itk.org/Doxygen314/html/classitk_1_1Statistics_1_1ListSampleToHistogramGenerator.html \endlink
 *  All it does is to create a histogram for an image set and given a channel set or a greyscale channel value.
 */
class HistogramGenerator : public QObject
{
    Q_OBJECT
public:

    typedef unsigned char                               PixelComponentType;
    typedef itk::RGBPixel< PixelComponentType >         RGBPixelType;
    typedef itk::Image< RGBPixelType, _Dimension >      RGBImageType;
    typedef itk::Statistics::ImageToHistogramGenerator<
                                RGBImageType >          HistogramGeneratorType;
    typedef HistogramGeneratorType::SizeType            SizeType;
    typedef itk::ImageFileReader< RGBImageType >        ReaderType;
    typedef HistogramGeneratorType::HistogramType       HistogramType;

    /*!
     * \brief The RMODE enum can be either RMODE_GREYSCALE or RMODE_RGB enumeration value
     */
    enum RMODE{
        RMODE_GREYSCALE = 0,
        RMODE_RGB       = 1
    };

    /*!
     * \brief HistogramGenerator , ctor
     * \param parent
     */
    explicit HistogramGenerator(QObject *parent = 0);

    /*!
     *  \brief destructor
     */
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

    //! getters
    double* redChannelFrequency() const { return m_pRedChannelFrequency; }
    double* greenChannelFrequency() const { return m_pGreenChannelFrequency; }
    double* blueChannelFrequency() const { return m_pBlueChannelFrequency; }

    //! getters
    double* redChannelAmplitude() const { return m_pRedChannelAmplitude; }
    double* greenChannelAmplitude() const { return m_pGreenChannelAmplitude; }
    double* blueChannelAmplitude() const { return m_pBlueChannelAmplitude; }

    //! getters
    QHash<int,double> greyscaleChannelData() const { return m_greyscaleChannelData; }
    QHash<int,double> redChannelData() const { return m_redChannelData; }
    QHash<int,double> greenChannelData() const { return m_greenChannelData; }
    QHash<int,double> blueChannelData() const { return m_blueChannelData; }


    //OTB specific stuff***************************************************
    //Default 32 bit image writer
    typedef VectorImageType* 		     ImagePointer;
    typedef otb::ImageFileWriter<VectorImageType> WriterType;
    WriterType::Pointer writer;

    /*!
     * \brief setRMode
     * \param rmode
     */
    void setRMode(RMODE rmode) { m_rmode = rmode; }

    //! getter
    RMODE rmode() { return m_rmode; }

    //! setter
    void setCurrentGreyChannel(int c) { m_currentGreyChannel = c; }

    //! setter
    void setCurrentRGBChannels(unsigned int red, unsigned int green, unsigned int blue) { m_redChannel = red; m_greenChannel = green; m_blueChannel = blue; }
    
signals:
    
public slots:

private:
    /*!
     * \brief histogramGenerator
     */
    HistogramGeneratorType::Pointer histogramGenerator;

    /*!
     * \brief reader, a helper instance
     */
    ReaderType::Pointer reader;

    /*!
     * \brief histogram instance
     */
    const HistogramType * histogram;

    /*!
     * \brief parseGreyscaleChannel
     */
    void parseGreyscaleChannel();


    /*!
     * \brief parseRedChannel, this is where the parsing for the red channel is sodne
     */
    void parseRedChannel();

    /*!
     * \brief parseGreenChannel, this is where the parsing for the green channel is sodne
     */
    void parseGreenChannel();

    /*!
     * \brief parseblueChannel, this is where the parsing for the blue channel is sodne
     */
    void parseBlueChannel();

    // this is ta helper funciton for creating new paths on the helper folder
    QString constructNewName();

    /// clear all files on the helper folder
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
