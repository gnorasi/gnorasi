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

#include "otbderivativeimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBDerivativeImageFilterProcessor::loggerCat_("voreen.OTBDerivativeImageFilterProcessor");

OTBDerivativeImageFilterProcessor::OTBDerivativeImageFilterProcessor()
    :OTBImageFilterProcessor(),
      order_("order_", "Set Order: ", 1, 0, 1),
      direction_("direction_", "Set Direction: ", 0, 0, 1),
      useImageSpacing_("useImageSpacing_", "Use image spacing: ", true),
      inPort_(Port::INPORT, "OTBImage.Inport",0),
      outPort_(Port::OUTPORT, "OTBImage.Outport",0)
{
    addProperty(enableSwitch_);
    addProperty(order_);
    addProperty(direction_);
    addProperty(useImageSpacing_);
    addPort(inPort_);
    addPort(outPort_);

    filter = DerivativeImageFilterType::New();

    portToCharCaster = PortToCharCastingFilterType::New();
    internToPortCaster = InternToPortCastingFilterType::New();
}

Processor* OTBDerivativeImageFilterProcessor::create() const {
    return new OTBDerivativeImageFilterProcessor();
}

OTBDerivativeImageFilterProcessor::~OTBDerivativeImageFilterProcessor() {

}

void OTBDerivativeImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBDerivativeImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBDerivativeImageFilterProcessor::getProcessorInfo() const {
    return "Derivative Image Filter Processor";
}

void OTBDerivativeImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    LINFO("Derivative Image Filter Enabled!");

    try
    {
        portToCharCaster->SetInput(inPort_.getData());
        filter->SetInput(portToCharCaster->GetOutput());

        filter->SetOrder(order_.get());
        filter->SetDirection(direction_.get());
        filter->SetUseImageSpacing(useImageSpacing_.get());
        filter->Update();
//        filter->UpdateLargestPossibleRegion();

        internToPortCaster->SetInput(filter->GetOutput());
        outPort_.setData(internToPortCaster->GetOutput());

        LINFO("Derivative Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in DerivativeImage Filter!");
        return;
    }
}

}   // namespace

