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

#include "otbgrayscaleconnectedopeningimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBGrayscaleConnectedOpeningImageFilterProcessor::loggerCat_("voreen.OTBGrayscaleConnectedOpeningImageFilterProcessor");

OTBGrayscaleConnectedOpeningImageFilterProcessor::OTBGrayscaleConnectedOpeningImageFilterProcessor()
    :OTBImageFilterProcessor(),
      //seed_("seed", "Seed:", 1.0f, 1.0f, 100.0f),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      outPort_(Port::OUTPORT, "OTBImage.outport",0)
{
    addProperty(enableSwitch_);
    //addProperty(seed_);
    addPort(inPort_);
    addPort(outPort_);

    filter = GrayscaleConnectedOpeningType::New();
}

Processor* OTBGrayscaleConnectedOpeningImageFilterProcessor::create() const {
    return new OTBGrayscaleConnectedOpeningImageFilterProcessor();
}

OTBGrayscaleConnectedOpeningImageFilterProcessor::~OTBGrayscaleConnectedOpeningImageFilterProcessor() {

}

void OTBGrayscaleConnectedOpeningImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBGrayscaleConnectedOpeningImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBGrayscaleConnectedOpeningImageFilterProcessor::getProcessorInfo() const {
    return "Grayscale Connected Opening Image Filter Processor";
}

void OTBGrayscaleConnectedOpeningImageFilterProcessor::process() {

    //LINFO("Grayscale Connected Opening Image Filter Enabled!");
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
        LINFO("Grayscale Connected Opening Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Grayscale Connected Opening Image Filter ");
        return;
    }

}

}   // namespace


