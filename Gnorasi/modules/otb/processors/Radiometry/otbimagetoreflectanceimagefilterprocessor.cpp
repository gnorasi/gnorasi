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

#include "otbimagetoreflectanceimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBImageToReflectanceImageFilterProcessor::loggerCat_("voreen.OTBImageToReflectanceImageFilterProcessor");

OTBImageToReflectanceImageFilterProcessor::OTBImageToReflectanceImageFilterProcessor()
    :OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "IN MultiBand Image", 0),
    outPort_(Port::OUTPORT, "OUT MultiBand Image", 0)
{
    addProperty(enableSwitch_);
    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
}

Processor* OTBImageToReflectanceImageFilterProcessor::create() const {
    return new OTBImageToReflectanceImageFilterProcessor();
}

OTBImageToReflectanceImageFilterProcessor::~OTBImageToReflectanceImageFilterProcessor() {

}

void OTBImageToReflectanceImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBImageToReflectanceImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBImageToReflectanceImageFilterProcessor::getProcessorInfo() const {
    return "Image To Reflectance Image Filter Processor";
}

void OTBImageToReflectanceImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    LINFO("Image To Reflectance Image Filter Enabled!");


    try
    {
        filter->SetInput(inPort_.getData());

        filter->Update();

        outPort_.setData(filter->GetOutput());
        LINFO("Image To Reflectance Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Image To Reflectance Image Filter");
        return;
    }

}

void OTBImageToReflectanceImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}// namespace


