/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.			*
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

#include "otbopticalcalibrationapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBOpticalCalibrationApplication::loggerCat_("voreen.OTBEstimateInnerProductPCAProcessor");

OTBOpticalCalibrationApplication::OTBOpticalCalibrationApplication()
    :OTBImageFilterProcessor(),
      centerData_("Enable/Disable center data", "Center Data", false),
      numberOfPrincipalComponentsReq_("Number of Principal Components required","Number of Principal Components", 1,1,3),
      inPort_(Port::INPORT, "IN MultiBand Image", 0),
      outPort_(Port::OUTPORT, "OUT MultiBand Image", 0)

{
      addProperty(enableSwitch_);
      addProperty(centerData_);
      addProperty(numberOfPrincipalComponentsReq_);
      addPort(inPort_);
      addPort(outPort_);

      ImageToLuminanceFilter = ImageToLuminanceImageFilterType::New();
      LuminanceToReflectanceFilter = LuminanceToReflectanceImageFilterType::New();
      ReflectanceToSurfaceReflectanceFilter = ReflectanceToSurfaceReflectanceImageFilterType::New();
      ScaleFilter = ScaleFilterType::New();
      ClampFilter = ClampFilterType::New();
      SurfaceAdjacencyEffectFilter = SurfaceAdjacencyEffect6SCorrectionSchemeFilterType::New();
      GroundSpacing = GroundSpacingImageType::New();
}

Processor* OTBOpticalCalibrationApplication::create() const {
    return new OTBOpticalCalibrationApplication();
}

OTBOpticalCalibrationApplication::~OTBOpticalCalibrationApplication() {

}

void OTBOpticalCalibrationApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBOpticalCalibrationApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBOpticalCalibrationApplication::getProcessorInfo() const {
    return "Optical Calibration Application";
}

void OTBOpticalCalibrationApplication::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        ///< Not implemented yet because of undefined reference errors.

        LINFO("Optical Calibration Application Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Optical Calibration Application");
        return;
    }

}

void OTBOpticalCalibrationApplication::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace
