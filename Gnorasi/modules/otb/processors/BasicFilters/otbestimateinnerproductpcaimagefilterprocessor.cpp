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

#include "otbestimateinnerproductpcaimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBEstimateInnerProductPCAImageFilterProcessor::loggerCat_("voreen.OTBEstimateInnerProductPCAProcessor");

OTBEstimateInnerProductPCAImageFilterProcessor::OTBEstimateInnerProductPCAImageFilterProcessor()
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

      filter = FilterType::New();
}

Processor* OTBEstimateInnerProductPCAImageFilterProcessor::create() const {
    return new OTBEstimateInnerProductPCAImageFilterProcessor();
}

OTBEstimateInnerProductPCAImageFilterProcessor::~OTBEstimateInnerProductPCAImageFilterProcessor() {

}

void OTBEstimateInnerProductPCAImageFilterProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBEstimateInnerProductPCAImageFilterProcessor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBEstimateInnerProductPCAImageFilterProcessor::getProcessorInfo() const {
    return "Estimate Inner Product PCA Image Filter";
}

void OTBEstimateInnerProductPCAImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()) {
        bypass(&inPort_, &outPort_);
        return;
    }

    try
    {
        filter->SetInput(inPort_.getData());
        filter->Update();
        filter->SetNumberOfPrincipalComponentsRequired(numberOfPrincipalComponentsReq_.get());
        filter->SetCenterData(centerData_.get());
        outPort_.setData(filter->GetOutput());
        LINFO("Estimate Inner Product PCA Image Filter Connected!");

    }
    catch (int e)
    {
        LERROR("Error in Estimate Inner Product PCA Image Filter");
        return;
    }

}

void OTBEstimateInnerProductPCAImageFilterProcessor::bypass(OTBVectorImagePort *inport, OTBVectorImagePort *outport) {

    outport->setData(inport->getData());
}

}   // namespace
