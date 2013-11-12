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

#include "otbbinarythinningimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBBinaryThinningImageFilterProcessor::loggerCat_("voreen.OTBBinaryThinningImageFilterProcessor");

OTBBinaryThinningImageFilterProcessor::OTBBinaryThinningImageFilterProcessor()
    :OTBImageFilterProcessor(),
      inPort_(Port::INPORT, "OTBImage.Inport",0),
      outPort_(Port::OUTPORT, "OTBImage.Outport",0)
{
    addProperty(enableSwitch_);
    addPort(inPort_);
    addPort(outPort_);

    filter = BinaryThinningImageFilterType::New();
    rescaler = RescaleIntensityImageFilterType::New();

    portToCharCaster = PortToCharCastingFilterType::New();
}

Processor* OTBBinaryThinningImageFilterProcessor::create() const {
    return new OTBBinaryThinningImageFilterProcessor();
}

OTBBinaryThinningImageFilterProcessor::~OTBBinaryThinningImageFilterProcessor() {

}

void OTBBinaryThinningImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBBinaryThinningImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBBinaryThinningImageFilterProcessor::getProcessorInfo() const {
    return "Binary Thinning Image Filter Processor";
}

void OTBBinaryThinningImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    LINFO("Binary Thinning Image Filter Enabled!");

    try
    {
        portToCharCaster->SetInput(inPort_.getData());

        filter->SetInput(portToCharCaster->GetOutput());

        rescaler->SetInput(filter->GetOutput());
        rescaler->SetOutputMinimum(0);
        rescaler->SetOutputMaximum(255);
        rescaler->Update();

        outPort_.setData(rescaler->GetOutput());

        LINFO("Binary Thinning Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Binary Thinning Image Filter!");
        return;
    }
}

}   // namespace

