/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) ****** - All rights reserved.                                  *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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

#include "otbrescaleintensityimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBRescaleIntensityImageFilterProcessor::loggerCat_("voreen.OTBRescaleIntensityImageFilterProcessor");

OTBRescaleIntensityImageFilterProcessor::OTBRescaleIntensityImageFilterProcessor()
    :OTBImageFilterProcessor(),
      OutputMin_("outputmin", "Output Minimum:", 0, -1000, 1000),
      OutputMax_("outputmax", "Output Maximum:", 255, -1000, 1000),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      outPort_(Port::OUTPORT, "OTBImage.outport",0)
{
    addProperty(enableSwitch_);
    addProperty(OutputMin_);
    addProperty(OutputMax_);
    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
}

Processor* OTBRescaleIntensityImageFilterProcessor::create() const {
    return new OTBRescaleIntensityImageFilterProcessor();
}

OTBRescaleIntensityImageFilterProcessor::~OTBRescaleIntensityImageFilterProcessor() {

}

void OTBRescaleIntensityImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBRescaleIntensityImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBRescaleIntensityImageFilterProcessor::getProcessorInfo() const {
    return "Rescale Intensity Image Filter Processor";
}

void OTBRescaleIntensityImageFilterProcessor::process() {

    //LINFO("Rescale Intensity Image Filter Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        filter->SetInput(inPort_.getData());
        filter->SetOutputMinimum(OutputMin_.get());
        filter->SetOutputMaximum(OutputMax_.get());
        filter->Update();
        outPort_.setData(filter->GetOutput());
        LINFO("Rescale Intensity Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Rescale Intensity Image Filter");
        return;
    }

}



}   // namespace


