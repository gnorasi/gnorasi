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

#include "otbdemcaracteristicsextractor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBDEMCaracteristicsExtractor::loggerCat_("voreen.OTBDEMCaracteristicsExtractor");

OTBDEMCaracteristicsExtractor::OTBDEMCaracteristicsExtractor()
    :OTBImageFilterProcessor(),
      solarAngle_("solarAngle", "Solar Angle:", 1.0f, 0.0f, 360.0f),
      solarAzimut_("solarAzimut", "Solar Azimut:", 1.0f, 0.0f, 360.0f),
      viewAngle_("viewAngle", "View Angle:", 0.0f, 0.0f, 360.0f),
      viewAzimut_("viewAzimut", "View Azimut:", 0.0f, 0.0f, 360.0f),
      inPort_(Port::INPORT, "IN OTB Image", 0),
      outPort_(Port::OUTPORT, "OUT OTB Image", 0)
{
    addProperty(enableSwitch_);
    addProperty(solarAngle_);
    addProperty(solarAzimut_);
    addProperty(viewAngle_);
    addProperty(viewAzimut_);
    addPort(inPort_);
    addPort(outPort_);

    extractor = DEMCaracteristicsExtractorFilter::New();
}

Processor* OTBDEMCaracteristicsExtractor::create() const {
    return new OTBDEMCaracteristicsExtractor();
}

OTBDEMCaracteristicsExtractor::~OTBDEMCaracteristicsExtractor() {

}

void OTBDEMCaracteristicsExtractor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBDEMCaracteristicsExtractor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBDEMCaracteristicsExtractor::getProcessorInfo() const {
    return "DEM Caracteristics Extractor";
}

void OTBDEMCaracteristicsExtractor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        extractor->SetInput(inPort_.getData());
        extractor->SetSolarAngle(solarAngle_.get());
        extractor->SetSolarAzimut(solarAzimut_.get());
        extractor->SetViewAngle(viewAngle_.get());
        extractor->SetViewAzimut(viewAzimut_.get());
        outPort_.setData(extractor->GetOutput());
        LINFO("DEM Caracteristics Extractor Connected!");

    }
    catch (int e)
    {
        LERROR("Error in DEM Caracteristics Extractor");
        return;
    }

}

}   // namespace


