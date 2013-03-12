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

#include "otbimagetosurfkeypointsetfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBImageToSURFKeyPointSetFilterProcessor::loggerCat_("voreen.OTBImageToSURFKeyPointSetFilterProcessor");

OTBImageToSURFKeyPointSetFilterProcessor::OTBImageToSURFKeyPointSetFilterProcessor()
    : OTBImageFilterProcessor(),

    octavesNumber_("octavesNumber", "octavesNumber Value", 255, 0, 3),
    scalesNumber_("scalesNumber", "scalesNumber Value", 255, 0, 3),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
    {
    addProperty(enableSwitch_);
    addProperty(octavesNumber_);
    addProperty(scalesNumber_);

    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
    }

Processor* OTBImageToSURFKeyPointSetFilterProcessor::create() const {
return new OTBImageToSURFKeyPointSetFilterProcessor();
}

OTBImageToSURFKeyPointSetFilterProcessor::~OTBImageToSURFKeyPointSetFilterProcessor() {
}

void OTBImageToSURFKeyPointSetFilterProcessor::initialize() throw (tgt::Exception) {

Processor::initialize();
}

void OTBImageToSURFKeyPointSetFilterProcessor::deinitialize() throw (tgt::Exception) {

Processor::deinitialize();
}

std::string OTBImageToSURFKeyPointSetFilterProcessor::getProcessorInfo() const {

return "SURF Detector Processor";
}

void OTBImageToSURFKeyPointSetFilterProcessor::process() {
    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
    filter->SetInput(inPort_.getData());

    filter->SetOctavesNumber(octavesNumber_.get());
    filter->SetScalesNumber(scalesNumber_.get());

    //The filter is executed by invoking the \code{Update()} method.
    filter->Update();

    //outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
        LERROR("Problem with SURF DETECTOR Processor!");
    return;
    }
}


} //namespace



