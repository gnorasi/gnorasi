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

#include "otbgeodesicmorphologylevelingfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBGeodesicMorphologyLevelingFilterProcessor::loggerCat_("voreen.OTBGeodesicMorphologyLevelingFilterProcessor");

OTBGeodesicMorphologyLevelingFilterProcessor::OTBGeodesicMorphologyLevelingFilterProcessor()
    :OTBImageFilterProcessor(),
      inPort_(Port::INPORT, "OTBImage.Inport",0),
      convexMapPort_(Port::INPORT, "Convex Map Inport",0),
      concaveMapPort_(Port::INPORT, "Concave Map Inport",0),
      outPort_(Port::OUTPORT, "OTBImage.Outport",0)
{
    addProperty(enableSwitch_);

    addPort(inPort_);
    addPort(convexMapPort_);
    addPort(concaveMapPort_);
    addPort(outPort_);

    filter = GeodesicMorphologyLevelingFilterType::New();
}

Processor* OTBGeodesicMorphologyLevelingFilterProcessor::create() const {
    return new OTBGeodesicMorphologyLevelingFilterProcessor();
}

OTBGeodesicMorphologyLevelingFilterProcessor::~OTBGeodesicMorphologyLevelingFilterProcessor() {

}

void OTBGeodesicMorphologyLevelingFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBGeodesicMorphologyLevelingFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBGeodesicMorphologyLevelingFilterProcessor::getProcessorInfo() const {
    return "Geodesic Morphology Leveling Image Filter Processor";
}

void OTBGeodesicMorphologyLevelingFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }
    LINFO("Geodesic Morphology Leveling Image Filter Enabled!");

    try
    {
        filter->SetInput(inPort_.getData());

        filter->SetInputConvexMap(convexMapPort_.getData());
        filter->SetInputConcaveMap(concaveMapPort_.getData());
        filter->Update();

        outPort_.setData(filter->GetOutput());

        LINFO("Geodesic Morphology Leveling Image Filter Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Geodesic Morphology Leveling Image Filter!");
        return;
    }
}

}   // namespace

