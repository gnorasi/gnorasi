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

#include "otbbinarydilatefilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBBinaryDilateFilterProcessor::loggerCat_("voreen.OTBBinaryDilateFilterProcessor");

OTBBinaryDilateFilterProcessor::OTBBinaryDilateFilterProcessor()
    : OTBImageFilterProcessor(),
    radius_("radius", "Structuring Element Radius", 1),
    foreground_("foreground", "Foregroung Value to Dilate", 255.0f, 0.0f, 1024.0f), 
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addProperty(radius_);
    addProperty(foreground_);
    addPort(inPort_);
    addPort(outPort_);
    
    filter = FilterType::New();
    structuringElement.SetRadius(radius_.get());
}

OTBBinaryDilateFilterProcessor::~OTBBinaryDilateFilterProcessor() {

}

void OTBBinaryDilateFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBBinaryDilateFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBBinaryDilateFilterProcessor::getProcessorInfo() const {
    
    return "Binary Dilation Filtering Processor";
}

void OTBBinaryDilateFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    
    //Property validation
    structuringElement.SetRadius(radius_.get());
    structuringElement.CreateStructuringElement();
    filter->SetKernel(structuringElement);
    filter->SetDilateValue(foreground_.get());
    
    try
    {
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with Binary Dilation Image Filter!");
	return;
    }
    
}


} // namespace
