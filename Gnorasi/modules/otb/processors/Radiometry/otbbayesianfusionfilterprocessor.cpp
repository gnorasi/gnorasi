/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.				*
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

#include "otbbayesianfusionfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBBayesianFusionFilterProcessor::loggerCat_("voreen.OTBBayesianFusionFilterProcessor");

OTBBayesianFusionFilterProcessor::OTBBayesianFusionFilterProcessor()
    : Processor(),
      inMultiSpecPort_(Port::INPORT, "Vector Multi Spec Image Input", 0),
      inMultiSpecInterpPort_(Port::INPORT, "Vector Multi Spec Interp. Image Input", 0),
      inPanchroPort_(Port::INPORT, "Vector Panchro Image Input", 0),
      outPort_(Port::OUTPORT, "Bayesian Fusion Filter Image Output", 0),

      //tune the fusion toward either a high color consistency or sharp details
      lambda_("Lambda", "Lambda coefficient", 0.9999)
{
    addPort(inMultiSpecPort_);
    addPort(inMultiSpecInterpPort_);
    addPort(inPanchroPort_);
    addPort(outPort_);

    lambda_.onChange(CallMemberAction<OTBBayesianFusionFilterProcessor>(this,
                    &OTBBayesianFusionFilterProcessor::setLambda));
    addProperty(lambda_);

    filter = BayesianFusionFilterType::New();

} //Namespace


Processor* OTBBayesianFusionFilterProcessor::create() const {
  return new OTBBayesianFusionFilterProcessor();
}

OTBBayesianFusionFilterProcessor::~OTBBayesianFusionFilterProcessor() {

}

void OTBBayesianFusionFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBBayesianFusionFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBBayesianFusionFilterProcessor::getProcessorInfo() const {

    return "Calculate Bayesian Fused Image from Multisprectral and Panchro Images";
}

void OTBBayesianFusionFilterProcessor::process() {

    try
    {
        filter->SetMultiSpect(inMultiSpecPort_.getData());
        filter->SetMultiSpectInterp(inMultiSpecInterpPort_.getData());
        filter->SetPanchro(inPanchroPort_.getData());

        filter->SetLambda(lambda_.get());

        outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
    LERROR("Problem with Bayesian Fuison Filter calculation!");
    return;
    }

}

void OTBBayesianFusionFilterProcessor::setLambda() {
    filter->SetLambda(lambda_.get());
}


} // namespace
