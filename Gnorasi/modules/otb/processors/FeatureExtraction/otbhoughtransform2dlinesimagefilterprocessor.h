/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
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

#ifndef OTBHOUGHTRANSFORM2DLINESIMAGEFILTERPROCESSOR_H
#define OTBHOUGHTRANSFORM2DLINESIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "itkHoughTransform2DLinesImageFilter.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"

#include "itkImageRegionIterator.h"
#include "itkThresholdImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkCastImageFilter.h"

namespace voreen {

class OTBHoughTransform2DLinesImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBHoughTransform2DLinesImageFilterProcessor();

    virtual ~OTBHoughTransform2DLinesImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "Hough Transform 2D Lines Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int Dimension = 2;

    typedef unsigned char CharPixelType;
    typedef float AccumulatorPixelType;
    typedef unsigned char OutputPixelType;

    typedef otb::Image<CharPixelType, Dimension> CharImageType;
    typedef otb::Image<double, 2> DoubleImageType;
    typedef otb::Image<AccumulatorPixelType, Dimension> AccumulatorImageType;
    typedef otb::Image< OutputPixelType, Dimension> OutputImageType;

    CharImageType::Pointer localInputImage;
    AccumulatorImageType::Pointer localAccumulator;
    OutputImageType::Pointer localOutputImage;

    ImageType::IndexType localIndex;
    OutputImageType::RegionType region;
    OutputImageType::RegionType outputRegion;

    typedef itk::CastImageFilter<CharImageType, AccumulatorImageType> CastingFilterType;
    CastingFilterType::Pointer caster;

    typedef itk::CastImageFilter<DoubleImageType, CharImageType> PortToCharCastingFilterType;
    PortToCharCastingFilterType::Pointer portToCharCaster;

    typedef itk::CastImageFilter<CharImageType, DoubleImageType> CharToPortCastingFilterType;
    CharToPortCastingFilterType::Pointer charToPortCaster;

    typedef itk::CastImageFilter<AccumulatorImageType, DoubleImageType> AccToDoubleCastingFilterType;
    AccToDoubleCastingFilterType::Pointer accToDoubleCaster;

    typedef itk::GradientMagnitudeImageFilter<AccumulatorImageType, AccumulatorImageType> GradientFilterType;
    GradientFilterType::Pointer gradFilter;

    typedef itk::ThresholdImageFilter<AccumulatorImageType> ThresholdFilterType;
    ThresholdFilterType::Pointer threshFilter;

    typedef itk::HoughTransform2DLinesImageFilter<PixelType, AccumulatorPixelType> HoughTransformFilterType;
    HoughTransformFilterType::Pointer houghFilter;
    HoughTransformFilterType::LinesListType lines;

    typedef HoughTransformFilterType::LinesListType::const_iterator linesIterator;
    typedef HoughTransformFilterType::LineType::PointListType PointListType;


protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    FloatProperty discRadius_;
    FloatProperty variance_;
    IntProperty numberOfLines_;


    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBHOUGHTRANSFORM2DLINESIMAGEFILTERPROCESSOR_H
