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

#include "otbimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBImageFilterProcessor::loggerCat_("voreen.OTBImageFilterProcessor");

OTBImageFilterProcessor::OTBImageFilterProcessor()
    : Processor(),
    enableSwitch_("enable", "Enable", true)
{
  
}

OTBImageFilterProcessor::~OTBImageFilterProcessor() {

}

void OTBImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBImageFilterProcessor::getProcessorInfo() const {
    
    return "Base Filtering Processor";
}

void OTBImageFilterProcessor::bypass(OTBImagePort *inport, OTBImagePort *outport) {
   
    outport->setData(inport->getData());
}

} // namespace

