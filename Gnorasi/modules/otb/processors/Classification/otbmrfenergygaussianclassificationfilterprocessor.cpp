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

#include "otbmrfenergygaussianclassificationfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBMRFEnergyGaussianClassificationFilterProcessor::loggerCat_("voreen.OTBMRFEnergyGaussianClassificationFilterProcessor");

OTBMRFEnergyGaussianClassificationFilterProcessor::OTBMRFEnergyGaussianClassificationFilterProcessor()
    : OTBImageFilterProcessor(),
      inPort_(Port::INPORT, "OTB.Image inport", 0),
      outPort_(Port::OUTPORT, "OTB.Image output", 0),
      numberOfIterations_("numberOfIterations_", "Number of  Iterations:", 1, 1, 100),
      lambda_("lambda_", "Lambda: ", 0.99f),
      optimizerTemperature_("optimizerTemperature_", "Optimizer Temperature:", 0.0f, 0.0f, 200.0f),
      useRandomValue_("useRandomValue_", "Use Random Value:", false)
{
    addProperty(enableSwitch_);

    addProperty(lambda_);
    addProperty(numberOfIterations_);
    addProperty(optimizerTemperature_);
    addProperty(useRandomValue_);

    addPort(inPort_);
    addPort(outPort_);

    markovFilter = MarkovRandomFieldFilterType::New();
    sampler = SamplerType::New();
    optimizer = OptimizerType::New();
    energyRegularization = EnergyRegularizationType::New();
    energyFidelity = EnergyFidelityType::New();

    rescaler = RescalerType::New();
    labelToPortCaster = LabelToPortCastingFilterType::New();
}

Processor* OTBMRFEnergyGaussianClassificationFilterProcessor::create() const {
  return new OTBMRFEnergyGaussianClassificationFilterProcessor();
}

OTBMRFEnergyGaussianClassificationFilterProcessor::~OTBMRFEnergyGaussianClassificationFilterProcessor() {

}

void OTBMRFEnergyGaussianClassificationFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBMRFEnergyGaussianClassificationFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBMRFEnergyGaussianClassificationFilterProcessor::getProcessorInfo() const {

    return "Markov Classification Filter";
}

void OTBMRFEnergyGaussianClassificationFilterProcessor::process() {

    try
    {
        //check bypass switch
        if (!enableSwitch_.get()) {
            bypass(&inPort_, &outPort_);
            LINFO("Markov Classification Filter Disabled!");
            return;
        }
        LINFO("Markov Classification Filter Enabled!");

        if (useRandomValue_.get() == true) {
            sampler->InitializeSeed(0);
            optimizer->InitializeSeed(1);
            markovFilter->InitializeSeed(2);
            LINFO("Overpass random calculation (test mode)!");
        }
        EnergyFidelityType::ParametersType parameters;

        markovFilter->SetInput(inPort_.getData());

        unsigned int nClass = 4;
        energyFidelity->SetNumberOfParameters(2*nClass);
        parameters.SetSize((energyFidelity->GetNumberOfParameters()));
        parameters[0] = 10.0;   //Class 0 mean
        parameters[1] = 10.0;   //Class 0 stdev
        parameters[2] = 80.0;   //Class 1 mean
        parameters[3] = 10.0;   //Class 1 stdev
        parameters[4] = 150.0;  //Class 2 mean
        parameters[5] = 10.0;   //Class 2 stdev
        parameters[6] = 220.0;  //Class 3 mean
        parameters[7] = 10.0;   //Class 3 stdev
        energyFidelity->SetParameters(parameters);

        OptimizerType::ParametersType param(1);
        param.Fill(optimizerTemperature_.get());
        optimizer->SetParameters(param);
        markovFilter->SetNumberOfClasses(nClass);
        markovFilter->SetMaximumNumberOfIterations(numberOfIterations_.get());
        markovFilter->SetErrorTolerance(0.0);
        markovFilter->SetLambda(lambda_.get());
        markovFilter->SetNeighborhoodRadius(1);
        markovFilter->SetEnergyRegularization(energyRegularization);
        markovFilter->SetEnergyFidelity(energyFidelity);
        markovFilter->SetOptimizer(optimizer);
        markovFilter->SetSampler(sampler);

        rescaler->SetInput(markovFilter->GetOutput());
        rescaler->SetOutputMinimum(0);
        rescaler->SetOutputMaximum(255);
        rescaler->Update();

        labelToPortCaster->SetInput(rescaler->GetOutput());
        outPort_.setData(labelToPortCaster->GetOutput());
        LINFO("Markov Classification Filter Connected Successfully!");

    }
    catch (int e)
    {
    LERROR("Problem with Markov Classification Filter!");
    return;
    }

}

} // namespace
