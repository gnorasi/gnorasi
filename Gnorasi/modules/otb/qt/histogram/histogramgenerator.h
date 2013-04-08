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
#include <QHash>

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

#include "otbStreamingHistogramVectorImageFilter.h"
#include "otbStreamingMinMaxVectorImageFilter.h"


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


    /**
     */
    enum Bound
    {
      BOUND_LOWER = 0,
      BOUND_UPPER,
      BOUND_COUNT
    };

    /** */
    typedef
      // itk::NumericTraits< T >::FloatType and
      // itk::NumericTraits< T >::RealType do not depend on template
      // parameter T. They are always typedef, respectively, as float
      // and double.
      //
      // So, itk::NumericTraits< DefaultImageType::InternalPixelType
      // >::RealType is equivalent to itk::NumericTraits< float
      // >::RealType which is always an alias of double.
      //
      // This typedef is used for compatibility with
      // itk::Histogram<>::MeasurementType.
      itk::NumericTraits< VectorImageType::InternalPixelType >::RealType
      MeasurementType;

    /** */
    typedef itk::Statistics::Histogram< MeasurementType, 1 > Histogram;

    /** */
    typedef otb::ObjectList< Histogram > HistogramList;

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

    /** */
    inline MeasurementType Quantile( CountType band, double p ) const;

    /** */
    inline MeasurementType Quantile( CountType band, double p, Bound bound ) const;

    /*!
     * \brief redChannelIndex
     * \return
     */
    unsigned int redChannelIndex() const { return m_redChannel; }

    /*!
     * \brief greenChannelIndex
     * \return
     */
    unsigned int greenChannelIndex() const { return m_greenChannel; }

    /*!
     * \brief blueChannelIndex
     * \return
     */
    unsigned int blueChannelIndex() const { return m_blueChannel; }

    
signals:
    
public slots:

private:
    void parseHistogram();

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

    RMODE m_rmode;

    int m_currentGreyChannel;

    HistogramList::Pointer m_Histograms;
    /** */
    VectorImageType::PixelType m_MinPixel;
    /** */
    VectorImageType::PixelType m_MaxPixel;

};


/*******************************************************************************/
inline
HistogramGenerator::MeasurementType
HistogramGenerator
::Quantile( unsigned int band,
        double p ) const
{
  assert( band<m_Histograms->Size() );

  return m_Histograms->GetNthElement( band )->Quantile( 0, p );
}

/*******************************************************************************/
inline
HistogramGenerator::MeasurementType
HistogramGenerator
::Quantile( unsigned int band,
        double p,
        Bound bound ) const
{
  assert( band<m_Histograms->Size() );

  return m_Histograms->GetNthElement( band )->Quantile(
    0,
    bound==BOUND_UPPER ? 1.0 - p : p
  );
}

#endif // HISTOGRAMGENERATOR_H
