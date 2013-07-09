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

#include "otbbayesianclassifierimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBBayesianClassifierImageFilterProcessor::loggerCat_("voreen.OTBBayesianClassifierImageFilterProcessor");

OTBBayesianClassifierImageFilterProcessor::OTBBayesianClassifierImageFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    outPort_(Port::OUTPORT, "Image Output", 0),
    numberOfSmoothingIterations_("numberOfSmoothingIterations_", "Number of Smoothing Iterations:", 1, 1, 100)
{
    addPort(inPort_);
    addPort(outPort_);

    addProperty(numberOfSmoothingIterations_);

    filter = ClassifierFilterType::New();
    smoother = SmoothingFilterType::New();
    rescaler = RescalerType::New();

    charToPortCaster = CharToPortCastingFilterType::New();
    vectorCaster = VectorCastingFilterType::New();
}

Processor* OTBBayesianClassifierImageFilterProcessor::create() const {
  return new OTBBayesianClassifierImageFilterProcessor();
}

OTBBayesianClassifierImageFilterProcessor::~OTBBayesianClassifierImageFilterProcessor() {

}

void OTBBayesianClassifierImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBBayesianClassifierImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBBayesianClassifierImageFilterProcessor::getProcessorInfo() const {

    return "Bayesian Classifier Image Filter";
}

void OTBBayesianClassifierImageFilterProcessor::process() {

    try
    {
        ///< Cast Vector Image (inPort to Input)

        vectorCaster->SetInput(inPort_.getData());
        filter->SetInput(vectorCaster->GetOutput());

        ///< Optional Parameter

        filter->SetNumberOfSmoothingIterations(numberOfSmoothingIterations_.get());

        ///< Smoothing Filter Parameters (hardcoded)

        smoother->SetNumberOfIterations(1);
        smoother->SetTimeStep(0.125);
        smoother->SetConductanceParameter(3);
        filter->SetSmoothingFilter(smoother);

        filter->Update();

        ///< Rescaler Parameters (hardcoded to defaults)

        rescaler->SetInput(filter->GetOutput());
        rescaler->SetOutputMinimum(0);
        rescaler->SetOutputMaximum(255);

        ///< Cast Image (Output to outPort)

        charToPortCaster->SetInput(rescaler->GetOutput());
        outPort_.setData(charToPortCaster->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with Bayesian Classifier Image Filter!");
    return;
    }

}

} // namespace
