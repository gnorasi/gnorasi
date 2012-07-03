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

#include "otbndwiprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBNDWIProcessor::loggerCat_("voreen.OTBNDWIProcessor");

OTBNDWIProcessor::OTBNDWIProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    outPort_(Port::OUTPORT, "NDWI Image Output", 0),
    nirBand_("nir", "NIR Band Index", 4),
    mirBand_("mir", "MIR Band Index", 5)
{
    addPort(inPort_);
    addPort(outPort_);
    nirBand_.onChange(CallMemberAction<OTBNDWIProcessor>(this, 
					&OTBNDWIProcessor::setNirBandIndex));
    addProperty(nirBand_);
    mirBand_.onChange(CallMemberAction<OTBNDWIProcessor>(this, 
					&OTBNDWIProcessor::setMirBandIndex));
    addProperty(mirBand_);
    
    filter = NDWIImageFilterType::New();
}

Processor* OTBNDWIProcessor::create() const {
  return new OTBNDWIProcessor(); 
}

OTBNDWIProcessor::~OTBNDWIProcessor() {

}

void OTBNDWIProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBNDWIProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBNDWIProcessor::getProcessorInfo() const {
    
    return "Calculate NDWI from Vector Image";
}

void OTBNDWIProcessor::process() {
  
    try
    {
	filter->SetIndex(otb::BandName::NIR , nirBand_.get());
        filter->SetIndex(otb::BandName::MIR , mirBand_.get());
	filter->SetInput(inPort_.getData());
	outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with NDWI calculation!");
	return;
    }
    
}

void OTBNDWIProcessor::setNirBandIndex() {
    filter->SetIndex(otb::BandName::NIR , nirBand_.get());
}

void OTBNDWIProcessor::setMirBandIndex() {
    filter->SetIndex(otb::BandName::MIR , mirBand_.get());
}

} // namespace
