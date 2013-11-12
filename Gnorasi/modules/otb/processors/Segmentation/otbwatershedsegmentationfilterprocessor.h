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

#ifndef OTBWATERSHEDSEGMENTATIONFILTERPROCESSOR_H
#define OTBWATERSHEDSEGMENTATIONFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include <itkRGBPixel.h>
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkVectorGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "otbWatershedSegmentationFilter.h"
#include "itkCastImageFilter.h"
#include "itkRGBToVectorImageAdaptor.h"
#include "../../ports/otbvectorimageport.h"
#include "../../ports/otblabelimageport.h"
#include "otbWatershedSegmentationFilter.h"
#include "../../ports/otblabelmapport.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"

namespace voreen {

class OTBWatershedSegmentationFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBWatershedSegmentationFilterProcessor();

    virtual ~OTBWatershedSegmentationFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Segmentation"; }
    virtual std::string getClassName() const { return "Watershed Segmentation"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;
    virtual bool isReady() const;

    typedef unsigned long                  LabelType;
    typedef otb::Image<LabelType, 2>       otbLabelImageType;
    typedef itk::Vector<float, 3>          VectorPixelType;
    typedef itk::Image<VectorPixelType, 2> VectorImageType;
    typedef otb::VectorImage<double, 2>    portVectorImageType;
    typedef itk::Image<float, 2>           ScalarImageType;
    typedef itk::RGBPixel<unsigned char>   RGBPixelType;
    typedef otb::Image<RGBPixelType, 2>    RGBImageType;

    //PIPELINE FILTERS
    typedef itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType,VectorImageType> DiffusionFilterType;
    DiffusionFilterType::Pointer diffusion;

    typedef itk::VectorGradientMagnitudeImageFilter<VectorImageType> GradientMagnitudeFilterType;
    GradientMagnitudeFilterType::Pointer gradient;

    typedef itk::WatershedImageFilter<ScalarImageType> WatershedFilterType;
    WatershedFilterType::Pointer watershedFilter;
    typedef WatershedFilterType::OutputImageType watershedOutput;

    //CASTERS
    typedef itk::VectorCastImageFilter<portVectorImageType, VectorImageType> VectorCastFilterType;
    VectorCastFilterType::Pointer vectorCaster;

    typedef itk::CastImageFilter<watershedOutput, otbLabelImageType> LabelImageCastFilterType;
    LabelImageCastFilterType::Pointer labelCaster;

    typedef itk::CastImageFilter<watershedOutput, ImageType> ImageCastFilterType;
    ImageCastFilterType::Pointer otbCaster;

    //CONVERTERS
    typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long> ColorMapFunctor;
    typedef itk::UnaryFunctorImageFilter<watershedOutput, RGBImageType, ColorMapFunctor> ColorMapFilterType;
    ColorMapFilterType::Pointer colormapper;

    typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, 2, double, unsigned short> LabelObjectType;
    typedef itk::LabelMap<LabelObjectType> LabelMapType;
    typedef itk::LabelImageToLabelMapFilter<watershedOutput, LabelMapType> LabelMapFilterType;
    LabelMapFilterType::Pointer labelMapFilter;


protected:
    virtual void setDescriptions()
    {
        setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

private:

    //Level controls watershed depth
    FloatProperty waterShedLevel_;
    //controls the lower thresholding of the input
    FloatProperty waterShedThreshold_;
    IntProperty numberOfIterations_;
    FloatProperty timeStep_;
    FloatProperty conductance_;
    BoolProperty gradientMode_;

    OTBVectorImagePort vectorInPort_;
    OTBLabelImagePort labelOutPort_;
    OTBLabelMapPort mapOutPort_;
    OTBImagePort    outPort_;
    OTBVectorImagePort rgbOutPort_;

    static const std::string loggerCat_; ///< category used in logging

};

} //namespace


#endif // OTBWATERSHEDSEGMENTATIONFILTERPROCESSOR_H
