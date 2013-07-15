/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
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

#ifndef OTBSHAPEDETECTIONLEVELSETIMAGEFILTERPROCESSOR_H
#define OTBSHAPEDETECTIONLEVELSETIMAGEFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/floatproperty.h"
#include "../../ports/otbimageport.h"
#include "itkShapeDetectionLevelSetImageFilter.h"
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSigmoidImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkCastImageFilter.h"

namespace voreen {

class OTBShapeDetectionLevelSetImageFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBShapeDetectionLevelSetImageFilterProcessor();

    virtual ~OTBShapeDetectionLevelSetImageFilterProcessor();
    virtual Processor* create() const;
    virtual std::string getProcessorInfo() const;


    virtual std::string getCategory() const { return "Image Segmentation"; }
    virtual std::string getClassName() const { return "Shape Detection Level Set Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; } // STABLE, TESTING, EXPERIMENTAL

    static const unsigned int Dimension = 2;

    typedef float       InternalPixelType;
    typedef itk::Image<InternalPixelType, Dimension> InternalImageType;

    typedef unsigned char   OutputPixelType;
    typedef itk::Image<OutputPixelType, Dimension>    OutputImageType;

    typedef itk::BinaryThresholdImageFilter<InternalImageType, OutputImageType> ThresholdingFilterType;
    ThresholdingFilterType::Pointer thresholder;

    typedef itk::CurvatureAnisotropicDiffusionImageFilter<ImageType, ImageType> SmoothingFilterType;
    SmoothingFilterType::Pointer smoothing;

    typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<ImageType, ImageType> GradientFilterType;
    GradientFilterType::Pointer gradientMagnitude;

    typedef itk::SigmoidImageFilter<InternalImageType, InternalImageType> SigmoidFilterType;
    SigmoidFilterType::Pointer sigmoid;

    typedef itk::FastMarchingImageFilter<ImageType, ImageType> FastMarchingFilterType;
    FastMarchingFilterType::Pointer fastMarching;

    typedef FastMarchingFilterType::NodeContainer NodeContainer;
    typedef FastMarchingFilterType::NodeType NodeType;

    NodeContainer::Pointer seeds;
    ImageType::IndexType seedPosition;

    typedef itk::ShapeDetectionLevelSetImageFilter<ImageType, ImageType> ShapeDetectionFilterType;
    ShapeDetectionFilterType::Pointer shapeDetection;

    //CASTERS

    typedef itk::CastImageFilter<ImageType, InternalImageType> PortToFloatCastingFilterType;
    PortToFloatCastingFilterType::Pointer portToFloatCaster;

    typedef itk::CastImageFilter<InternalImageType, ImageType> FloatToPortCastingFilterType;
    FloatToPortCastingFilterType::Pointer floatToPortCaster;
    FloatToPortCastingFilterType::Pointer floatToPortCaster1;

    typedef itk::CastImageFilter<OutputImageType, ImageType> CharToPortCastingFilterType;
    CharToPortCastingFilterType::Pointer charToPortCaster;

protected:

    virtual void setDescriptions()
    {
        setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    FloatProperty initialDistance_;
    FloatProperty sigma_;
    IntProperty seedX_;
    IntProperty seedY_;
    FloatProperty sigmoidAlpha_;
    FloatProperty sigmoidBeta_;
    FloatProperty propagationScaling_;
    FloatProperty curvatureScaling_;

    OTBImagePort inPort_;
    OTBImagePort outPort_;

    OTBImagePort testOutPort1_;
    OTBImagePort testOutPort2_;
    OTBImagePort testOutPort3_;
    OTBImagePort testOutPort4_;

    static const std::string loggerCat_; ///< category used in logging

    };
}
//namespace

#endif // OTBSHAPEDETECTIONLEVELSETIMAGEFILTERPROCESSOR_H
