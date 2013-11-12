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

#include "otbsarradiometriccalibrationapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBSARRadiometricCalibrationApplication::loggerCat_("voreen.OTBSARRadiometricCalibrationApplication");

OTBSARRadiometricCalibrationApplication::OTBSARRadiometricCalibrationApplication()
    :OTBImageFilterProcessor(),
      noise_("Enable/Disable noise", "Noise", false),
      inPort_(Port::INPORT, "IN OTB Image", 0),
      outPort_(Port::OUTPORT, "OUT OTB Image", 0)

{
      addProperty(enableSwitch_);
      addProperty(noise_);
      addPort(inPort_);
      addPort(outPort_);

      filter = SarRadiometricCalibrationToImageFilterType::New();

}

Processor* OTBSARRadiometricCalibrationApplication::create() const {
    return new OTBSARRadiometricCalibrationApplication();
}

OTBSARRadiometricCalibrationApplication::~OTBSARRadiometricCalibrationApplication() {

}

void OTBSARRadiometricCalibrationApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBSARRadiometricCalibrationApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBSARRadiometricCalibrationApplication::getProcessorInfo() const {
    return "SAR Radiometric Calibration Application";
}

void OTBSARRadiometricCalibrationApplication::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        filter->SetInput(inPort_.getData());
        filter->SetEnableNoise(noise_.get());
        filter->Update();
        outPort_.setData(filter->GetOutput());
        LINFO("SAR Radiometric Calibration Application Connected");

    }
    catch (int e)
    {
        LERROR("Error in SAR Radiometric Calibration Application");
        return;
    }

}

//void OTBSARRadiometricCalibrationApplication::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

//    outport->setData(inport->getData());
//}

}   // namespace
