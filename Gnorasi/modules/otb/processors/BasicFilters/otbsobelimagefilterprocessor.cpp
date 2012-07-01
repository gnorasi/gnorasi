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

#include "otbsobelimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBSobelImageFilterProcessor::loggerCat_("voreen.OTBSobelImageFilterProcessor");

OTBSobelImageFilterProcessor::OTBSobelImageFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
{
    addProperty(enableSwitch_);
    addPort(inPort_);
    addPort(outPort_);
    
    filter = FilterType::New();
}

Processor* OTBSobelImageFilterProcessor::create() const {
  return new OTBSobelImageFilterProcessor(); 
}

OTBSobelImageFilterProcessor::~OTBSobelImageFilterProcessor() {

}

void OTBSobelImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBSobelImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBSobelImageFilterProcessor::getProcessorInfo() const {
    
    return "Sobel Edge Detection Filtering Processor";
}

void OTBSobelImageFilterProcessor::process() {

    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }
    
    try
    {
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with Sobel Image Filter!");
	return;
    }
    
}


} // namespace
