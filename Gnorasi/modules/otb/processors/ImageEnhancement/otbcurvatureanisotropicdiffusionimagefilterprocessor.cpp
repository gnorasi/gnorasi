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

#include "otbcurvatureanisotropicdiffusionimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBCurvatureAnisotropicDiffusionImageFilterProcessor::loggerCat_("voreen.OTBCurvatureAnisotropicDiffusionImageFilterProcessor");

OTBCurvatureAnisotropicDiffusionImageFilterProcessor::OTBCurvatureAnisotropicDiffusionImageFilterProcessor()
    :OTBImageFilterProcessor(),
      useImageSpacing_("useImageSpacing", "Use Image Spacing",false),
      numberOfIterations_("numberOfIterations", "Number of Iterations:", 0, 0, 10),
      timeStep_("timeStep", "Time Step:", 1.0f, 1.0f, 100.0f),
      conductance_("conductance", "Conductance:", 1.0f, 1.0f, 100.0f),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      outPort_(Port::OUTPORT, "OTBImage.outport",0)
{
    addProperty(enableSwitch_);
    addProperty(useImageSpacing_);
    addProperty(numberOfIterations_);
    addProperty(timeStep_);
    addProperty(conductance_);
    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
}

Processor* OTBCurvatureAnisotropicDiffusionImageFilterProcessor::create() const {
    return new OTBCurvatureAnisotropicDiffusionImageFilterProcessor();
}

OTBCurvatureAnisotropicDiffusionImageFilterProcessor::~OTBCurvatureAnisotropicDiffusionImageFilterProcessor() {

}

void OTBCurvatureAnisotropicDiffusionImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBCurvatureAnisotropicDiffusionImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBCurvatureAnisotropicDiffusionImageFilterProcessor::getProcessorInfo() const {
    return "Curvature Anisotropic Diffusion Image Filter Processor";
}

void OTBCurvatureAnisotropicDiffusionImageFilterProcessor::process() {

    //LINFO("Curvature Anisotropic Diffusion Image Filter Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {

        filter->SetInput(inPort_.getData());

        filter->SetNumberOfIterations(numberOfIterations_.get());
        filter->SetTimeStep(timeStep_.get());
        filter->SetConductanceParameter(conductance_.get());
        if (useImageSpacing_.get()) {
            filter->UseImageSpacingOn();
        }
        filter->Update();
        outPort_.setData(filter->GetOutput());
        LINFO("Curvature Anisotropic Diffusion Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Curvature Anisotropic Diffusion Image Filter ");
        return;
    }

}

}   // namespace


