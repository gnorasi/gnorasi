/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.					*
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
 
#include "otbgradientmagnitudeimagefilterprocessor.h"

namespace voreen {
const std::string OTBGradientMagnitudeImageFilterProcessor::loggerCat_("voreen.OTBGradientMagnitudeImageFilterProcessor");


OTBGradientMagnitudeImageFilterProcessor::OTBGradientMagnitudeImageFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
    {
    addProperty(enableSwitch_);

    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
    }

Processor* OTBGradientMagnitudeImageFilterProcessor::create() const {
return new OTBGradientMagnitudeImageFilterProcessor();
}

OTBGradientMagnitudeImageFilterProcessor::~OTBGradientMagnitudeImageFilterProcessor() {

}

void OTBGradientMagnitudeImageFilterProcessor::initialize() throw (tgt::Exception) {

Processor::initialize();
}

void OTBGradientMagnitudeImageFilterProcessor::deinitialize() throw (tgt::Exception) {

Processor::deinitialize();
}

std::string OTBGradientMagnitudeImageFilterProcessor::getProcessorInfo() const {

return "Gradient Magnitude Filtering Processor";
}

void OTBGradientMagnitudeImageFilterProcessor::process() {
    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
    filter->SetInput(inPort_.getData());

    filter->Update();

    outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
        LERROR("Problem with Gradient Magnitute Image Filter!");
    return;
    }
}

} //namespace


