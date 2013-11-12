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

#include "otbgrayscalegeodesicdilateimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBGrayscaleGeodesicDilateImageFilterProcessor::loggerCat_("voreen.OTBGrayscaleGeodesicDilateImageFilterProcessor");

OTBGrayscaleGeodesicDilateImageFilterProcessor::OTBGrayscaleGeodesicDilateImageFilterProcessor()
    :OTBImageFilterProcessor(),
      runOneIteration_("Run one iteration or until convergence.", "Set One Iteration:", true),
      inPort_(Port::INPORT, "IN OTB Image Port",0),
      markerPort_(Port::INPORT, "Marker OTB Image Port",0),
      maskPort_(Port::INPORT, "Mask OTB Image Port",0),
      outPort_(Port::OUTPORT, "OUT OTB Image Port",0)
{
    addProperty(enableSwitch_);
    addProperty(runOneIteration_);
    addPort(inPort_);
    addPort(markerPort_);
    addPort(maskPort_);
    addPort(outPort_);

    filter = GrayscaleGeodesicDilateType::New();
}

Processor* OTBGrayscaleGeodesicDilateImageFilterProcessor::create() const {
    return new OTBGrayscaleGeodesicDilateImageFilterProcessor();
}

OTBGrayscaleGeodesicDilateImageFilterProcessor::~OTBGrayscaleGeodesicDilateImageFilterProcessor() {

}

void OTBGrayscaleGeodesicDilateImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBGrayscaleGeodesicDilateImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBGrayscaleGeodesicDilateImageFilterProcessor::getProcessorInfo() const {
    return "Grayscale Connected Closing Image Filter Processor";
}

void OTBGrayscaleGeodesicDilateImageFilterProcessor::process() {

    //LINFO("Grayscale Geodesic Dilate Image Filter Enabled!");
    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        filter->SetRunOneIteration(runOneIteration_.get());
        filter->SetInput(inPort_.getData());
        filter->SetMarkerImage(markerPort_.getData());
        filter->SetMaskImage(maskPort_.getData());
        outPort_.setData(filter->GetOutput());
        LINFO("Grayscale Geodesic Dilate Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Grayscale Geodesic Dilate Image Filter ");
        return;
    }

}

}   // namespace


