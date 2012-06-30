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

#include "otbndviprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBNDVIProcessor::loggerCat_("voreen.OTBNDVIProcessor");

OTBNDVIProcessor::OTBNDVIProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    outPort_(Port::OUTPORT, "NDVI Image Output", 0),
    redBand_("red", "Red Band Index", 3),
    nirBand_("nir", "NIR Band Index", 4)
{
    addPort(inPort_);
    addPort(outPort_);
    redBand_.onChange(CallMemberAction<OTBNDVIProcessor>(this, 
					&OTBNDVIProcessor::setRedBandIndex));
    addProperty(redBand_);
    nirBand_.onChange(CallMemberAction<OTBNDVIProcessor>(this, 
					&OTBNDVIProcessor::setNirBandIndex));
    addProperty(nirBand_);
    
    filter = NDVIImageFilterType::New();
}

OTBNDVIProcessor::~OTBNDVIProcessor() {

}

void OTBNDVIProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBNDVIProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBNDVIProcessor::getProcessorInfo() const {
    
    return "Calculate NDVI from Vector Image";
}

void OTBNDVIProcessor::process() {
  
    try
    {
	filter->SetRedIndex(redBand_.get());
	filter->SetNIRIndex(nirBand_.get());
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with NDVI calculation!");
	return;
    }
    
}

void OTBNDVIProcessor::setRedBandIndex() {
    filter->SetRedIndex(redBand_.get());
}

void OTBNDVIProcessor::setNirBandIndex() {
    filter->SetNIRIndex(nirBand_.get());
}

} // namespace
