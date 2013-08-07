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

#include "otbrgbtoluminanceimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBRGBToLuminanceImageFilterProcessor::loggerCat_("voreen.OTBRGBToLuminanceImageFilterProcessor");

OTBRGBToLuminanceImageFilterProcessor::OTBRGBToLuminanceImageFilterProcessor()
    :OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "IN MultiBand Image", 0),
    outPort_(Port::OUTPORT, "OUT MultiBand Image", 0)
{
//    addProperty(enableSwitch_);
    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
    rgbCaster = RGBCasterType::New();
    scalarCaster = ScalarCasterType::New();
}

Processor* OTBRGBToLuminanceImageFilterProcessor::create() const {
    return new OTBRGBToLuminanceImageFilterProcessor();
}

OTBRGBToLuminanceImageFilterProcessor::~OTBRGBToLuminanceImageFilterProcessor() {

}

void OTBRGBToLuminanceImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBRGBToLuminanceImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBRGBToLuminanceImageFilterProcessor::getProcessorInfo() const {
    return "RGB To Luminance Image Filter Processor";
}

void OTBRGBToLuminanceImageFilterProcessor::process() {

    //check bypass switch
//    if (!enableSwitch_.get()) {
//        bypass(&inPort_, &outPort_);
//        return;
//    }
//    LINFO("RGB To Luminance Image Filter Enabled!");


    try
    {
        rgbCaster->SetInput(inPort_.getData());
        filter->SetInput(rgbCaster->GetOutput());

        filter->Update();

        scalarCaster->SetInput(filter->GetOutput());
        outPort_.setData(scalarCaster->GetOutput());
        LINFO("RGB To Luminance Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in RGB To Luminance Image Filter");
        return;
    }

}

//void OTBRGBToLuminanceImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

//    outport->setData(inport->getData());
//}

}// namespace


