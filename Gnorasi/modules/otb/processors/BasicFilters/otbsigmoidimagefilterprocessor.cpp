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

#include "otbsigmoidimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBSigmoidImageFilterProcessor::loggerCat_("voreen.OTBSigmoidImageFilterProcessor");

OTBSigmoidImageFilterProcessor::OTBSigmoidImageFilterProcessor():OTBImageFilterProcessor(),
      sigmoidAlpha_("sigmoidAlpha", "Sigmoid Alpha:", 5.f, -100.0f, 255.0f),
      sigmoidBeta_("sigmoidBeta", "Sigmoid Beta:", 10.0f, -100.0f, 255.0f),
      min_("min_", "Output Minimum: ", 0.0f, -1000, 1000),
      max_("max_", "Output Maximum: ", 255.0f, -1000, 1000),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      outPort_(Port::OUTPORT, "OTBImage.outport",0)
{
    addProperty(enableSwitch_);
    addProperty(sigmoidAlpha_);
    addProperty(sigmoidBeta_);
    addProperty(min_);
    addProperty(max_);

    addPort(inPort_);
    addPort(outPort_);

    sigmoid = SigmoidFilterType::New();

}

Processor* OTBSigmoidImageFilterProcessor::create() const {
    return new OTBSigmoidImageFilterProcessor();
}

OTBSigmoidImageFilterProcessor::~OTBSigmoidImageFilterProcessor() {

}

void OTBSigmoidImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBSigmoidImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBSigmoidImageFilterProcessor::getProcessorInfo() const {
    return "Otsu Multiple Threshold Image Segmentation Processor";
}

void OTBSigmoidImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    LINFO("Sigmoid Image Filter Enabled!");

    try
    {
        //Sigmoid
        sigmoid->SetInput(inPort_.getData());

        sigmoid->SetOutputMinimum(min_.get());
        sigmoid->SetOutputMaximum(max_.get());
        sigmoid->SetAlpha(sigmoidAlpha_.get());
        sigmoid->SetBeta(sigmoidBeta_.get());
        sigmoid->Update();

        outPort_.setData(sigmoid->GetOutput());
        LINFO("Sigmoid Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Sigmoid Image Filter");
        return;
    }

}

} //namespace
