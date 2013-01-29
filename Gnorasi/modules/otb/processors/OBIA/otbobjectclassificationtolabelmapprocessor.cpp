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

#include "otbobjectclassificationtolabelmapprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBObjectClassificationToLabelMapProcessor::loggerCat_("voreen.OTBObjectClassificationToLabelMapProcessor");
  
OTBObjectClassificationToLabelMapProcessor::OTBObjectClassificationToLabelMapProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Object Map Input", 0),
    inTextPort_(Port::INPORT, "Input Classification Text", 0),
    outPort_(Port::OUTPORT, "Object Map Output", 0),
    updateButton_("updateButton", "Update Object Map")
{
    // register ports and properties
    addPort(inPort_);
    addPort(inTextPort_);
    addPort(outPort_);
    updateButton_.onChange(CallMemberAction<OTBObjectClassificationToLabelMapProcessor>(this, &OTBObjectClassificationToLabelMapProcessor::update));
    addProperty(updateButton_);
    
    //OTB initialization
    labelmap = 0;
}

Processor* OTBObjectClassificationToLabelMapProcessor::create() const {
    return new OTBObjectClassificationToLabelMapProcessor();
}


bool OTBObjectClassificationToLabelMapProcessor::isReady() const {
    return (inPort_.isConnected() && inTextPort_.isConnected());
}

std::string OTBObjectClassificationToLabelMapProcessor::getProcessorInfo() const {
    return "Saves Object Classification in Attribute Map";
}

void OTBObjectClassificationToLabelMapProcessor::process() {

}

void OTBObjectClassificationToLabelMapProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
}

void OTBObjectClassificationToLabelMapProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBObjectClassificationToLabelMapProcessor::update() {
    
    if(this->isReady())
    {
	try
	{
	    //Connect with data inport
	    labelmap = inPort_.getData();
	    
	    //Check if Object Map has data in it
	    if(labelmap->GetLabelObjectContainer().empty()){
		LWARNING("Object Map is empty. Please Update previous processors");
		return;
	    }
	    
	    std::stringstream ss(inTextPort_.getData());
	    
	    //For each input text line (CSV)
	    std::string line;
	    while(std::getline(ss,line))
	    {
		std::stringstream  lineStream(line);
		std::string        cell;
		int i = 1;
		long unsigned int id;
		short unsigned int classid;
		while(std::getline(lineStream,cell,';'))
		{
		    std::stringstream tmp(cell);
		    if (i == 1)	tmp >> id;
		    if (i == 2) tmp >> classid;
		    i++;
		}
		
		LabelMapType::LabelObjectContainerType::iterator it=
		labelmap->GetLabelObjectContainer().find(id);
		if (it != labelmap->GetLabelObjectContainer().end())
		{
		    it->second->SetClassLabel(classid);
		}
		
	    }
	    
        outPort_.setData(labelmap);
	    
	}
	catch(itk::ExceptionObject& err)
	{
	    LWARNING("ExceptionObject caught !");
	    return;
	}
    }else if(!this->isReady()){
	LWARNING("Input Object Map or Input Classification Text Data is not connected");
	return;
    }

    LINFO("Object Map updated with classification succesfully!");
    
}


} // namespace
