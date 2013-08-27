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

#include "otbprospectmodelprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBProspectModelProcessor::loggerCat_("voreen.OTBProspectModelProcessor");

OTBProspectModelProcessor::OTBProspectModelProcessor()
    :OTBImageFilterProcessor(),
      Cab_("Cab_", "Chlorophyll (Cab):",30.0f),
      Car_("Car_", "Carotenoid (Car):",10.0f),
      CBrown_("CBrown_", "Brown pigment content (CBrown):", 0.0f),
      Cw_("Cw_", "Water thickness EWT (Cw):", 0.015),
      Cm_("Cm_", "Dry matter content LMA (Cm):", 0.009),
      N_("N_", "Leaf structure parameter (N):", 1.2f),
      inPort_(Port::INPORT, "OTBImage.inport",0),
      outPort_(Port::OUTPORT, "OTBImage.outport",0)
{
    addProperty(enableSwitch_);

    //*INSTANTIATE TRIGGERS LINKIN ERROR* (http://i.imgur.com/E6WZ771.png)
    leafParams = LeafParametersType::New();
    prospect = ProspectType::New();
}

Processor* OTBProspectModelProcessor::create() const {
    return new OTBProspectModelProcessor();
}

OTBProspectModelProcessor::~OTBProspectModelProcessor() {

}

void OTBProspectModelProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBProspectModelProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBProspectModelProcessor::getProcessorInfo() const {
    return "Prospect Model Processor";
}

void OTBProspectModelProcessor::process() {

//    //check bypass switch
//    if (!enableSwitch_.get()) {
//        bypass(&inPort_, &outPort_);
//        return;
//    }
    LINFO("Prospect Model Processor Enabled!");

    try
    {

        double Cab = Cab_.get();
        double Car = Car_.get();
        double CBrown = CBrown_.get();
        double Cw = Cw_.get();
        double Cm = Cm_.get();
        double N = N_.get();

//        leafParams->SetCab(Cab);
//        leafParams->SetCar(Car);
//        leafParams->SetCBrown(CBrown);
//        leafParams->SetCw(Cw);
//        leafParams->SetCm(Cm);
//        leafParams->SetN(N);

//        prospect->SetInput(leafParams);
//        prospect->Update();

//        //OUTPUTS
//        response = prospect->GetReflectance();
//        prospect->GetTransmittance();

        LINFO("Prospect Model Processor Connected!");
    }
    catch (int e)
    {
        LERROR("Error in Prospect Model Processor");
        return;
    }

}

} //namespace
