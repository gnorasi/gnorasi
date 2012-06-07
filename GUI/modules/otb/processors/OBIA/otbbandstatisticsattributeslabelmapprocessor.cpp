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

#include "otbbandstatisticsattributeslabelmapprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBBandStatisticsAttributesLabelMapProcessor::loggerCat_("voreen.OTBBandStatisticsAttributesLabelMapProcessor");

OTBBandStatisticsAttributesLabelMapProcessor::OTBBandStatisticsAttributesLabelMapProcessor()
    : Processor(),
    reducedProperties_("reduced", "Only Basic Properties", true),
    update_("updateButton", "Update"), 
    inPort_(Port::INPORT, "Input Object Map", 0),
    inVImage_(Port::INPORT, "Input MultiBand Image", 0),
    outPort_(Port::OUTPORT, "Output Object Map", 0),
    outPort2_(Port::OUTPORT, "Unchanged Input Object Map", 0),
    outVImage_(Port::OUTPORT, "Unchanged Input MultiBand Image", 0)
{
    addProperty(reducedProperties_);
    update_.onChange(CallMemberAction<OTBBandStatisticsAttributesLabelMapProcessor>(this, &OTBBandStatisticsAttributesLabelMapProcessor::update));
    addProperty(update_);
    addPort(inPort_);
    addPort(inVImage_);
    addPort(outPort_);
    addPort(outPort2_);
    addPort(outVImage_);
        
    statisticsLabelMapFilter = StatisticsLabelMapFilterType::New();
}

OTBBandStatisticsAttributesLabelMapProcessor::~OTBBandStatisticsAttributesLabelMapProcessor() {

}

void OTBBandStatisticsAttributesLabelMapProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBBandStatisticsAttributesLabelMapProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBBandStatisticsAttributesLabelMapProcessor::getProcessorInfo() const {
    
    return "Calculation of Band Statistics Properties on Object Map";
}

void OTBBandStatisticsAttributesLabelMapProcessor::process() {

    try
    {
	statisticsLabelMapFilter->SetInput(inPort_.getData());
	statisticsLabelMapFilter->SetFeatureImage(inVImage_.getData());
	(reducedProperties_.get()) ? statisticsLabelMapFilter->ReducedAttributeSetOn() : 
				     statisticsLabelMapFilter->ReducedAttributeSetOff();
	outPort_.setData(statisticsLabelMapFilter->GetOutput());
	outVImage_.setData(inVImage_.getData());
	outPort2_.setData(inPort_.getData());
	LINFO("Here");
    }
    catch (int e)
    {
	LERROR("Problem with Statistics Calculation on Object Map!");
	return;
    }
    
}

void OTBBandStatisticsAttributesLabelMapProcessor::update() {
    process();
    statisticsLabelMapFilter->Update();
}

bool OTBBandStatisticsAttributesLabelMapProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inPort_.isConnected()) return false;
    
    if(!outPort_.isConnected() && !outPort2_.isConnected() && !outVImage_.isConnected()) return false;
    
    return true;
}


} // namespace
