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

#include "otbstatisticsattributeslabelmapprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBStatisticsAttributesLabelMapProcessor::loggerCat_("voreen.OTBStatisticsAttributesLabelMapProcessor");

OTBStatisticsAttributesLabelMapProcessor::OTBStatisticsAttributesLabelMapProcessor()
    : Processor(),
    reducedProperties_("reduced", "Only Basic Properties", true),
    featureName_("featurename","Feature Name", "feature1"),
    update_("updateButton", "Update"), 
    automaticUpdate("automaticUpdate", "Automaic Update", false),
    inPort_(Port::INPORT, "Input Object Map", 0),
    inImage_(Port::INPORT, "Input Feature Image", 0),
    outPort_(Port::OUTPORT, "Output Object Map", 0),
    outImage_(Port::OUTPORT, "Unchanged Feature Image", 0)
{
    addProperty(automaticUpdate);
    addProperty(reducedProperties_);
    featureName_.onChange(CallMemberAction<OTBStatisticsAttributesLabelMapProcessor>(this, &OTBStatisticsAttributesLabelMapProcessor::setFeatureName));
    addProperty(featureName_);
    update_.onChange(CallMemberAction<OTBStatisticsAttributesLabelMapProcessor>(this, &OTBStatisticsAttributesLabelMapProcessor::update));
    addProperty(update_);
    addPort(inPort_);
    addPort(inImage_);
    addPort(outPort_);
    addPort(outImage_);
        
    statisticsLabelMapFilter = StatisticsLabelMapFilterType::New();
    featureName = "feature1";
}

Processor* OTBStatisticsAttributesLabelMapProcessor::create() const {
  return new OTBStatisticsAttributesLabelMapProcessor(); 
}

OTBStatisticsAttributesLabelMapProcessor::~OTBStatisticsAttributesLabelMapProcessor() {

}

void OTBStatisticsAttributesLabelMapProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBStatisticsAttributesLabelMapProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBStatisticsAttributesLabelMapProcessor::getProcessorInfo() const {
    
    return "Adding a Feature on Object Map";
}

void OTBStatisticsAttributesLabelMapProcessor::process() {

    LINFO("Statistics Calculation Processor Processed.");
    try
    {
    statisticsLabelMapFilter->SetInput(inPort_.getData());
    statisticsLabelMapFilter->SetFeatureImage(inImage_.getData());
    statisticsLabelMapFilter->SetFeatureName(featureName);
    (reducedProperties_.get()) ? statisticsLabelMapFilter->ReducedAttributeSetOn() :
                     statisticsLabelMapFilter->ReducedAttributeSetOff();

    if(automaticUpdate.get())
        statisticsLabelMapFilter->Update();

    outPort_.setData(statisticsLabelMapFilter->GetOutput());
    outImage_.setData(inImage_.getData());
    }
    catch (int e)
    {
	LERROR("Problem with Statistics Calculation on Object Map!");
	return;
    }
    
}

void OTBStatisticsAttributesLabelMapProcessor::update() {
    process();
    statisticsLabelMapFilter->Update();
}

void OTBStatisticsAttributesLabelMapProcessor::setFeatureName() {
    featureName = featureName_.get();
}

bool OTBStatisticsAttributesLabelMapProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inPort_.isConnected() && !inImage_.isConnected()) return false;
    
    if(!outPort_.isConnected() && !outImage_.isConnected()) return false;
    
    return true;
}

} // namespace
