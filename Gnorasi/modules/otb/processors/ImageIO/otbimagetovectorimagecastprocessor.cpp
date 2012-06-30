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

#include "otbimagetovectorimagecastprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBImageToVectorImageCastProcessor::loggerCat_("voreen.OTBImageToVectorImageCastProcessor");

OTBImageToVectorImageCastProcessor::OTBImageToVectorImageCastProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Image Input", 0),
    outPort_(Port::OUTPORT, "Vector Image Output", 0)
{
    addPort(inPort_);
    addPort(outPort_);
    
    filter = ImageToVectorImageCastFilterType::New();
}

OTBImageToVectorImageCastProcessor::~OTBImageToVectorImageCastProcessor() {

}

void OTBImageToVectorImageCastProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBImageToVectorImageCastProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBImageToVectorImageCastProcessor::getProcessorInfo() const {
    
    return "Convert Image to Vector Image with Single Band";
}

void OTBImageToVectorImageCastProcessor::process() {

    try
    {
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with Image to Vector Image Filter!");
	return;
    }
    
}


} // namespace
