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

#include "otbgrayscaleconnectedclosingimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBGrayscaleConnectedClosingImageFilterProcessor::loggerCat_("voreen.OTBGrayscaleConnectedClosingImageFilterProcessor");

OTBGrayscaleConnectedClosingImageFilterProcessor::OTBGrayscaleConnectedClosingImageFilterProcessor()
    :OTBImageFilterProcessor(),
      //seed_("seed", "Seed:", 1.0f, 1.0f, 100.0f),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      outPort_(Port::OUTPORT, "OTBImage.outport",0)
{
    addProperty(enableSwitch_);
    //addProperty(seed_);
    addPort(inPort_);
    addPort(outPort_);

    filter = GrayscaleConnectedClosingType::New();
}

Processor* OTBGrayscaleConnectedClosingImageFilterProcessor::create() const {
    return new OTBGrayscaleConnectedClosingImageFilterProcessor();
}

OTBGrayscaleConnectedClosingImageFilterProcessor::~OTBGrayscaleConnectedClosingImageFilterProcessor() {

}

void OTBGrayscaleConnectedClosingImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBGrayscaleConnectedClosingImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBGrayscaleConnectedClosingImageFilterProcessor::getProcessorInfo() const {
    return "Grayscale Connected Closing Image Filter Processor";
}

void OTBGrayscaleConnectedClosingImageFilterProcessor::process() {

    //LINFO("Grayscale Connected Closing Image Filter Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        filter->SetInput(inPort_.getData());
        //filter->SetSeed(seed_.getData());
        outPort_.setData(filter->GetOutput());
        LINFO("Grayscale Connected Closing Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Grayscale Connected Closing Image Filter ");
        return;
    }

}

}   // namespace


