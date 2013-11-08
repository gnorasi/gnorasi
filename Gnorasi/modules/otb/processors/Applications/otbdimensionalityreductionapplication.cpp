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

#include "otbdimensionalityreductionapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBDimensionalityReductionApplication::loggerCat_("voreen.OTBDimensionalityReductionApplication");

OTBDimensionalityReductionApplication::OTBDimensionalityReductionApplication()
    :OTBImageFilterProcessor(),
      noise_("Enable/Disable noise", "Noise", false),
      inPort_(Port::INPORT, "IN OTB Image", 0),
      outPort_(Port::OUTPORT, "OUT OTB Image", 0)

{
      addProperty(enableSwitch_);
      addProperty(noise_);
      addPort(inPort_);
      addPort(outPort_);

}

Processor* OTBDimensionalityReductionApplication::create() const {
    return new OTBDimensionalityReductionApplication();
}

OTBDimensionalityReductionApplication::~OTBDimensionalityReductionApplication() {

}

void OTBDimensionalityReductionApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBDimensionalityReductionApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBDimensionalityReductionApplication::getProcessorInfo() const {
    return "Dimensionality Reduction Application";
}

void OTBDimensionalityReductionApplication::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {

        LINFO("Dimensionality Reduction Application Connected");

    }
    catch (int e)
    {
        LERROR("Error in Dimensionality Reduction Application");
        return;
    }

}

//void OTBDimensionalityReductionApplication::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

//    outport->setData(inport->getData());
//}

}   // namespace
