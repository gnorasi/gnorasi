/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * 										*
 *                                                                    		*
 * This file is part of the GNORASI software package. GNORASI is free  		*
 * software: you can redistribute it and/or modify it under the terms 		*
 * of the GNU General Public License version 2 as published by the    		*
 * Free Software Foundation.                                          		*
 *                                                                    		*
 * GNORASI is distributed in the hope that it will be useful,          		*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     		*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       		*
 * GNU General Public License for more details.                       		*
 *                                                                    		*
 * You should have received a copy of the GNU General Public License  		*
 * in the file "LICENSE.txt" along with this program.                 		*
 * If not, see <http://www.gnu.org/licenses/>.                        		*
 *                                                                    		*
 ********************************************************************************/

#include "otbgradientanisotropicdiffusionfilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBGradientAnisotropicDiffusionFilterProcessor::loggerCat_("voreen.OTBGradientAnisotropicDiffusionFilterProcessor");

OTBGradientAnisotropicDiffusionFilterProcessor::OTBGradientAnisotropicDiffusionFilterProcessor()
    : OTBImageFilterProcessor(),
    numberOfIterations_("numberOfIterations", "Iterations", 5),
    timeStep_("timeStep", "Time Step", 0.125f, 0.0f, 10.0f),
    conductance_("conductance", "Conductance", 3.0f, 0.0f, 100.0f),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(numberOfIterations_);
    addProperty(timeStep_);
    addProperty(conductance_);
    addPort(inPort_);
    addPort(outPort_);
    
    filter = FilterType::New();
}

Processor* OTBGradientAnisotropicDiffusionFilterProcessor::create() const {
  return new OTBGradientAnisotropicDiffusionFilterProcessor(); 
}

OTBGradientAnisotropicDiffusionFilterProcessor::~OTBGradientAnisotropicDiffusionFilterProcessor() {

}

void OTBGradientAnisotropicDiffusionFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBGradientAnisotropicDiffusionFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBGradientAnisotropicDiffusionFilterProcessor::getProcessorInfo() const {
    
    return "Gradient Anisotropic Diffusion Filtering Processor";
}

void OTBGradientAnisotropicDiffusionFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    
    //Property validation
    
    filter->SetNumberOfIterations(numberOfIterations_.get());
    filter->SetTimeStep(timeStep_.get());
    filter->SetConductanceParameter(conductance_.get());
    
    try
    {
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Error in Gradient Anisotropic Diffusion!");
	return;
    }
    
}


} // namespace
