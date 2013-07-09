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

#ifndef OTBHOUGHTRANSFORM2DCIRCLESIMAGEFILTERPROCESSOR_H
#define OTBHOUGHTRANSFORM2DCIRCLESIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "itkHoughTransform2DCirclesImageFilter.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"
#include "itkCastImageFilter.h"

namespace voreen {

class OTBHoughTransform2DCirclesImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBHoughTransform2DCirclesImageFilterProcessor();

    virtual ~OTBHoughTransform2DCirclesImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Feature Extraction"; }
    virtual std::string getClassName() const { return "Hough Transform 2D Circles Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int Dimension = 2;

    typedef unsigned char CharPixelType;
    typedef float AccumulatorPixelType;
    typedef unsigned char OutputPixelType;

    typedef otb::Image<CharPixelType, Dimension> CharImageType;
    typedef otb::Image<AccumulatorPixelType, Dimension> AccumulatorImageType;
    typedef otb::Image<double,2> DoubleImageType;
    typedef otb::Image< OutputPixelType, Dimension> OutputImageType;

    CharImageType::Pointer localInputImage;
    AccumulatorImageType::Pointer localAccumulator;
    OutputImageType::Pointer localOutputImage;

    ImageType::IndexType localIndex;
    OutputImageType::RegionType region;
    OutputImageType::RegionType outputRegion;

    typedef itk::CastImageFilter<DoubleImageType, CharImageType> PortToCharCastingFilterType;
    PortToCharCastingFilterType::Pointer portToCharCaster;

    typedef itk::CastImageFilter<CharImageType, DoubleImageType> CharToPortCastingFilterType;
    CharToPortCastingFilterType::Pointer charToPortCaster;

    typedef itk::HoughTransform2DCirclesImageFilter<PixelType, AccumulatorPixelType> HoughTransformFilterType;
    HoughTransformFilterType::Pointer houghFilter;
    HoughTransformFilterType::CirclesListType circles;

    typedef HoughTransformFilterType::CirclesListType CirclesListType;
    CirclesListType::const_iterator itCircles;

protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    FloatProperty maxRadius_;
    FloatProperty minRadius_;
    FloatProperty sweepAngle_;
    IntProperty sigmaGradient_;
    FloatProperty variance_;
    FloatProperty radiusToRemove_;
    IntProperty numberOfCircles_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBHOUGHTRANSFORM2DCIRCLESIMAGEFILTERPROCESSOR_H
