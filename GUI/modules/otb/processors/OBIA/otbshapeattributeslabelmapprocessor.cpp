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

#include "otbshapeattributeslabelmapprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBShapeAttributesLabelMapProcessor::loggerCat_("voreen.OTBShapeAttributesLabelMapProcessor");

OTBShapeAttributesLabelMapProcessor::OTBShapeAttributesLabelMapProcessor()
    : Processor(),
    reducedProperties_("reduced", "Only Basic Properties", true),
    inPort_(Port::INPORT, "Input Object Map", 0),
    outPort_(Port::OUTPORT, "Output Object Map", 0)
{
    addProperty(reducedProperties_);
    addPort(inPort_);
    addPort(outPort_);
    
    shapeLabelMapFilter = ShapeLabelMapFilterType::New();
}

OTBShapeAttributesLabelMapProcessor::~OTBShapeAttributesLabelMapProcessor() {

}

void OTBShapeAttributesLabelMapProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBShapeAttributesLabelMapProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBShapeAttributesLabelMapProcessor::getProcessorInfo() const {
    
    return "Calculation of Shape Properties on Object Map";
}

void OTBShapeAttributesLabelMapProcessor::process() {
    
    LINFO("ShapeAttributes Processed");
    try
    {
	shapeLabelMapFilter->SetInput(inPort_.getData());
	(reducedProperties_.get()) ? shapeLabelMapFilter->ReducedAttributeSetOn() : 
				     shapeLabelMapFilter->ReducedAttributeSetOff();
	outPort_.setData(shapeLabelMapFilter->GetOutput());
    }
    catch (int e)
    {
	LERROR("Problem with Label Image to Object Map process!");
	return;
    }
    
}


} // namespace
