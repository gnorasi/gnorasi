/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
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

#include "otbleeimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBLeeImageFilterProcessor::loggerCat_("voreen.OTBLeeImageFilterProcessor");


OTBLeeImageFilterProcessor::OTBLeeImageFilterProcessor()
    : OTBImageFilterProcessor(),
    radius_("radiusWindowValue", "Filter Radius", 3, 3, 15),
    numberOfLooks_("NumberOfLooks", "Number of Looks", 1.0f, 0.0f, 4.0f),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
    {
    addProperty(enableSwitch_);
    addProperty(radius_);
    addProperty(numberOfLooks_);

    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
    }

Processor* OTBLeeImageFilterProcessor::create() const {
return new OTBLeeImageFilterProcessor();
}

OTBLeeImageFilterProcessor::~OTBLeeImageFilterProcessor() {

}

void OTBLeeImageFilterProcessor::initialize() throw (tgt::Exception) {

Processor::initialize();
}

void OTBLeeImageFilterProcessor::deinitialize() throw (tgt::Exception) {

Processor::deinitialize();
}

std::string OTBLeeImageFilterProcessor::getProcessorInfo() const {

return "Edge-preserving Speckle Filtering Processor";
}

void OTBLeeImageFilterProcessor::process() {
    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }


    // The method \code{SetRadius()} defines the size of the window to
    // be used for the computation of the local statistics. The method
    // \code{SetNbLooks()} sets the number of looks of the input
    // image.
    try
    {
    filter->SetInput(inPort_.getData());

    FilterType::SizeType Radius;
    Radius[0] = radius_.get();
    Radius[1] = radius_.get();
    filter->SetRadius(Radius);
    filter->SetNbLooks(numberOfLooks_.get());

    outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
        LERROR("Problem with Edge Perserving Speckle Image Filter!");
    return;
    }
}




} //namespace




