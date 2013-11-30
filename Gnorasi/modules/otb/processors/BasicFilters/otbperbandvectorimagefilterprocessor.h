/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#ifndef OTBPERBANDVECTORIMAGEFILTERPROCESSOR_H
#define OTBPERBANDVECTORIMAGEFILTERPROCESSOR_H

#include "otbimagefilterprocessor.h"
#include "otbVectorImage.h"
#include "modules/otb/ports/otbvectorimageport.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/optionproperty.h"

#include "otbPerBandVectorImageFilter.h"

//Included filters
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkMeanImageFilter.h"

namespace voreen {

class OTBPerBandVectorImageFilterProcessor : public OTBImageFilterProcessor
{
public:
    OTBPerBandVectorImageFilterProcessor();

    virtual ~OTBPerBandVectorImageFilterProcessor();

    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Basic Filters"; }
    virtual std::string getClassName() const { return "Per Band Vector Image Filter"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING;}//STABLE, TESTING, EXPERIMENTAL

    virtual std::string getProcessorInfo() const;

    typedef otb::VectorImage<double, 2> VectorImageType;

    // CANNY EDGE DETECTION IMAGE FILTER PROCESSOR DECLARATION

    typedef itk::CannyEdgeDetectionImageFilter<OTBImageFilterProcessor::ImageType,
                                OTBImageFilterProcessor::ImageType> CannyEdgeDetectionFilterType;
    CannyEdgeDetectionFilterType::Pointer canny;

    typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, CannyEdgeDetectionFilterType> CannyPerBandFilterType;
    CannyPerBandFilterType::Pointer cannyPerBand;

    // GRADIENT ANISOTROPIC DIFFUSION IMAGE FILTER PROCESSOR DECLARATION

    typedef itk::GradientAnisotropicDiffusionImageFilter<OTBImageFilterProcessor::ImageType,
                          OTBImageFilterProcessor::ImageType> GradientAnisotropicDiffusionFilterType;
    GradientAnisotropicDiffusionFilterType::Pointer gradient;

    typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, GradientAnisotropicDiffusionFilterType> GradientAnisotropicPerBandFilterType;
    GradientAnisotropicPerBandFilterType::Pointer gradientAnisotropicPerBand;

    // MEAN IMAGE FILTER PROCESSOR DECLARATION

    typedef itk::MeanImageFilter<OTBImageFilterProcessor::ImageType,OTBImageFilterProcessor::ImageType> MeanFilterType;
    MeanFilterType::Pointer mean;

    typedef otb::PerBandVectorImageFilter<VectorImageType, VectorImageType, MeanFilterType> MeanPerBandFilterType;
    MeanPerBandFilterType::Pointer meanPerBand;


protected:

    virtual void setDescriptions() {
    setDescription("processor.");
    }
    void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual void bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport); ///< Passes the image from inport to outport without changes.

    virtual void updateFilterSelection();

private:

    OTBVectorImagePort inPort_;
    OTBVectorImagePort outPort_;
    StringOptionProperty selectFilter_; ///< Select which filter to use

    // CANNY EDGE DETECTION IMAGE FILTER PROCESSOR VARIABLES

    FloatProperty variance_;
    FloatProperty upper_;
    FloatProperty lower_;

    // GRADIENT ANISOTROPIC DIFFUSION IMAGE FILTER PROCESSOR VARIABLES

    IntProperty numberOfIterations_;
    FloatProperty timeStep_;
    FloatProperty conductance_;

    // MEAN IMAGE FILTER PROCESSOR VARIABLES

    IntProperty filterSize_;

    static const std::string loggerCat_; ///< category used in logging
};

} // namespace

#endif // OTBPERBANDVECTORIMAGEFILTERPROCESSOR_H
