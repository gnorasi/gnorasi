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
#ifndef OTBBAYESIANCLASSIFIERIMAGEFILTERPROCESSOR_H
#define OTBBAYESIANCLASSIFIERIMAGEFILTERPROCESSOR_H

#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "../BasicFilters/otbimagefilterprocessor.h"
#include "../../ports/otbvectorimageport.h"
#include "itkBayesianClassifierImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

namespace voreen {

class OTBBayesianClassifierImageFilterProcessor : public OTBImageFilterProcessor {
public:
    OTBBayesianClassifierImageFilterProcessor();
    virtual ~OTBBayesianClassifierImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Classification"; }
    virtual std::string getClassName() const { return "Bayesian Classifier Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_EXPERIMENTAL; }//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    static const unsigned int                                           Dimension = 2;
    typedef float                                                       InputPixelType;
    typedef itk::VectorImage<InputPixelType,Dimension>                  InputImageType;

    typedef itk::Image<unsigned char,Dimension>                         OutputImageType;

    typedef otb::Image<double, 2> DoubleImageType;
    typedef itk::VectorImage<double, 2> VectorDoubleImageType;

    typedef itk::Image<double, 2>                                       PortImageType;


    typedef unsigned char                                               LabelType;
    typedef float                                                       PriorType;
    typedef float                                                       PosteriorType;

    typedef itk::BayesianClassifierImageFilter<InputImageType,LabelType,PosteriorType,PriorType> ClassifierFilterType;
    ClassifierFilterType::Pointer filter;

    typedef ClassifierFilterType::ExtractedComponentImageType ExtractedComponentImageType;
    typedef itk::GradientAnisotropicDiffusionImageFilter<ExtractedComponentImageType, ExtractedComponentImageType> SmoothingFilterType;
    SmoothingFilterType::Pointer smoother;

    typedef ClassifierFilterType::OutputImageType ClassifierOutputImageType;

    typedef itk::RescaleIntensityImageFilter<ClassifierOutputImageType, OutputImageType> RescalerType;
    RescalerType::Pointer rescaler;

    typedef itk::CastImageFilter<OutputImageType, DoubleImageType> CharToPortCastingFilterType;
    CharToPortCastingFilterType::Pointer charToPortCaster;

    typedef itk::CastImageFilter<VectorDoubleImageType, InputImageType> VectorCastingFilterType;
    VectorCastingFilterType::Pointer vectorCaster;

protected:
    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();

    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    void update();

private:

    OTBVectorImagePort inPort_;
    OTBImagePort outPort_;

    IntProperty         numberOfSmoothingIterations_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBBAYESIANCLASSIFIERIMAGEFILTERPROCESSOR_H
