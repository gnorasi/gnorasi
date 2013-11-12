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

#include "otbattributesmapopeninglabelmapfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBAttributesMapOpeningLabelMapFilterProcessor::loggerCat_("voreen.OTBAttributesMapOpeningLabelMapFilterProcessor");

OTBAttributesMapOpeningLabelMapFilterProcessor::OTBAttributesMapOpeningLabelMapFilterProcessor()
    :OTBImageFilterProcessor(),
      attribute_("attribute_", "Attribute Name: ", "STATS::Band1::Mean"),
      thresh_("thresh_", "Thresholding Lambda:", 0.5f, 0.5f, 0.999f),
      lowerThan_("lowerThan_", "Reverse Ordering:", false),
      inPort_(Port::INPORT, "Object Map inport",0),
      outPort_(Port::OUTPORT, "Object Map outport",0)
{
    addProperty(enableSwitch_);
    addProperty(attribute_);
    addProperty(thresh_);
    addProperty(lowerThan_);

    addPort(inPort_);
    addPort(outPort_);

    filter = OpeningLabelMapFilterType::New();
}

Processor* OTBAttributesMapOpeningLabelMapFilterProcessor::create() const {
    return new OTBAttributesMapOpeningLabelMapFilterProcessor();
}

OTBAttributesMapOpeningLabelMapFilterProcessor::~OTBAttributesMapOpeningLabelMapFilterProcessor() {

}

void OTBAttributesMapOpeningLabelMapFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBAttributesMapOpeningLabelMapFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBAttributesMapOpeningLabelMapFilterProcessor::getProcessorInfo() const {
    return "Attributes Map Opening Label Map Filter Processor";
}

void OTBAttributesMapOpeningLabelMapFilterProcessor::process() {

    //LINFO("Attributes Map Opening Label Map Filter Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        std::string attr = attribute_.get();
        const char *cattr = attr.c_str();

        filter->SetInput(inPort_.getData());
        filter->SetAttributeName(cattr);
        filter->SetLambda(thresh_.get());
        filter->SetReverseOrdering(lowerThan_.get());
        filter->Update();
        outPort_.setData(filter->GetOutput());

        LINFO("Attributes Map Opening Label Map Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Attributes Map Opening Label Map Filter");
        return;
    }

}
void OTBAttributesMapOpeningLabelMapFilterProcessor::bypass(OTBLabelMapPort *inport, OTBLabelMapPort *outport) {

    outport->setData(inport->getData());
}

}   // namespace


