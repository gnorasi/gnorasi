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

#include "otbbinarythresholdfilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBBinaryThresholdFilterProcessor::loggerCat_("voreen.OTBBinaryThresholdFilterProcessor");

OTBBinaryThresholdFilterProcessor::OTBBinaryThresholdFilterProcessor()
    : OTBImageFilterProcessor(),
    insideValue_("insideValue", "Inside Value", 255.0f, 0.0f, 1024.0f),
    outsideValue_("outsideValue", "Outside Value", 0.0f, 0.0f, 1024.0f),
    lowerThreshold_("lowerThreshold", "Lower Threshold", 0.1f, 0.0f, 1024.0f),
    upperThreshold_("upperThreshold", "Upper Threshold", 1.0f, 0.0f, 1024.0f),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(insideValue_);
    addProperty(outsideValue_);
    addProperty(lowerThreshold_);
    addProperty(upperThreshold_);
    addPort(inPort_);
    addPort(outPort_);
    
    filter = FilterType::New();
}

Processor* OTBBinaryThresholdFilterProcessor::create() const {
  return new OTBBinaryThresholdFilterProcessor(); 
}

OTBBinaryThresholdFilterProcessor::~OTBBinaryThresholdFilterProcessor() {

}

void OTBBinaryThresholdFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBBinaryThresholdFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBBinaryThresholdFilterProcessor::getProcessorInfo() const {
    
    return "Binary Threshold Filtering Processor";
}

void OTBBinaryThresholdFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    
    //Property validation
    filter->SetOutsideValue(outsideValue_.get());
    filter->SetInsideValue(insideValue_.get());
    filter->SetLowerThreshold(lowerThreshold_.get());
    filter->SetUpperThreshold(upperThreshold_.get());
    
    try
    {
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with Binary Threshold Image Filter!");
	return;
    }
    
}


} // namespace
