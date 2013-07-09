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

#ifndef OTBGEODESICACTIVECONTOURLEVELSETIMAGEFILTERPROCESSOR_H
#define OTBGEODESICACTIVECONTOURLEVELSETIMAGEFILTERPROCESSOR_H

#include "modules/otb/processors/BasicFilters/otbimagefilterprocessor.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "../../ports/otbimageport.h"
#include "otbImage.h"
#include "itkCastImageFilter.h"
#include "itkGeodesicActiveContourLevelSetImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSigmoidImageFilter.h"
#include "itkFastMarchingImageFilter.h"

namespace voreen {

class OTBGeodesicActiveContourLevelSetImageFilterProcessor : public OTBImageFilterProcessor
{
public:

    OTBGeodesicActiveContourLevelSetImageFilterProcessor();

    virtual ~OTBGeodesicActiveContourLevelSetImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Segmentation"; }
    virtual std::string getClassName() const { return "Geodesic Active Contour Level Set Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int                           Dimension = 2;
    typedef itk::Image<unsigned char,2>                 CharImageType;
    typedef float                                       InternalPixelType;
    typedef itk::Image<InternalPixelType, Dimension>    InternalImageType;

    //PIPELINE FILTERS

    typedef itk::CurvatureAnisotropicDiffusionImageFilter<ImageType, ImageType> SmoothingFilterType;
    SmoothingFilterType::Pointer smoothing;

    typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<ImageType, ImageType> GradientFilterType;
    GradientFilterType::Pointer gradientMagnitude;

    typedef itk::SigmoidImageFilter<CharImageType, CharImageType> SigmoidFilterType;
    SigmoidFilterType::Pointer sigmoid;

    typedef itk::FastMarchingImageFilter<ImageType, ImageType> FastMarchingFilterType;
    FastMarchingFilterType::Pointer fastMarching;

    typedef FastMarchingFilterType::NodeContainer NodeContainer;
    typedef FastMarchingFilterType::NodeType NodeType;

    NodeContainer::Pointer seeds;
    ImageType::IndexType seedPosition;

    typedef itk::GeodesicActiveContourLevelSetImageFilter<ImageType, InternalImageType> GeodesicActiveContourFilterType;
    GeodesicActiveContourFilterType::Pointer geodesicActiveContour;

    typedef itk::BinaryThresholdImageFilter<ImageType, ImageType> ThresholdingFilterType;
    ThresholdingFilterType::Pointer thresholder;

    //CASTERS

    typedef itk::CastImageFilter<InternalImageType, ImageType> InternalToPortCastingFilterType;
    InternalToPortCastingFilterType::Pointer internToPortCaster1;

    typedef itk::CastImageFilter<ImageType, CharImageType> PortToCharCastingFilterType;
    PortToCharCastingFilterType::Pointer portToCharCaster;

    typedef itk::CastImageFilter<CharImageType, InternalImageType> CharToInternalCastingFilterType;
    CharToInternalCastingFilterType::Pointer charToInternalCaster;

    typedef itk::CastImageFilter<CharImageType, ImageType> CharToPortCastingFilterType;
    CharToPortCastingFilterType::Pointer charToPortCaster;

protected:

    virtual void setDescriptions() {
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

    OTBImagePort inPort_;
    OTBImagePort outPort_;
    OTBImagePort testOutPort1_;
    OTBImagePort testOutPort2_;
    OTBImagePort testOutPort3_;
    OTBImagePort testOutPort4_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBGEODESICACTIVECONTOURLEVELSETIMAGEFILTERPROCESSOR_H
