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

#include "otblabelmapstatisticwriterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBLabelMapStatisticsWriterProcessor::loggerCat_("voreen.OTBLabelMapStatisticsWriterProcessor");
  
OTBLabelMapStatisticsWriterProcessor::OTBLabelMapStatisticsWriterProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Object Map Input", 0),
    outPort_(Port::OUTPORT, "Object Properties Output", 0),
    update_("updateButton", "Update Text Data")
{
    // register ports and properties
    addPort(inPort_);
    addPort(outPort_);
    update_.onChange(CallMemberAction<OTBLabelMapStatisticsWriterProcessor>(this, &OTBLabelMapStatisticsWriterProcessor::process));
    addProperty(update_);
    
    //OTB initialization
    labelmap = 0;
}

Processor* OTBLabelMapStatisticsWriterProcessor::create() const {
    return new OTBLabelMapStatisticsWriterProcessor();
}


bool OTBLabelMapStatisticsWriterProcessor::isReady() const {
    return (inPort_.isConnected() && outPort_.isConnected());
}

std::string OTBLabelMapStatisticsWriterProcessor::getProcessorInfo() const {
    return "Saves Object Properties To Text Data";
}

void OTBLabelMapStatisticsWriterProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
}

void OTBLabelMapStatisticsWriterProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBLabelMapStatisticsWriterProcessor::process() {
    
    if(this->isReady())
    {
	try
	{
	    //Connect with data inport
	    labelmap = inPort_.getData();
	    
	    //test code
	    //std::stringstream tmp1;
	    //tmp1 << "ID" << ";" << "CLASS" << std::endl;
	    
	    //Check if Object Map has data in it
	    if(labelmap->GetLabelObjectContainer().empty()){
		LWARNING("Object Map is empty. Please Update previous processors");
		return;
	    }
	    
	    //we need to read the titles of the Properties in the Attribute Table
	    //in this case we read the first LabelObject from the std::map container.
	    typename LabelMapType::LabelObjectContainerType::const_iterator it=
		labelmap->GetLabelObjectContainer().begin();
	    
	    //we now go in the LabelObject type and read the Attributes Name List
	    std::vector<std::string> attrvector = it->second->GetAvailableAttributes();
	    
	    pTextDataOut_ << "ID" << ";";
	    
            for(unsigned int i=0;i<attrvector.size();i++)
	    {
		(i!=attrvector.size()-1) ? pTextDataOut_ << attrvector[i] << ";" 
		    : pTextDataOut_ << attrvector[i];
		
	    }
	    pTextDataOut_ << std::endl;
	    
	    //Iterate through all LabelMap Objects
	    it = labelmap->GetLabelObjectContainer().begin();
            //int k =0;
	    while(it != labelmap->GetLabelObjectContainer().end())
	    {
		//Export Label ID
		pTextDataOut_ << it->second->GetLabel() << ";";
		//test code
		//tmp1 << it->second->GetLabel() << ";";
		//Iterate through the attributes
                for(unsigned int i=0;i<attrvector.size();i++)
		{
		    (i!=attrvector.size()-1) ?  
			pTextDataOut_ << it->second->GetAttribute(attrvector[i].c_str()) << ";" :
                        pTextDataOut_ << it->second->GetAttribute(attrvector[i].c_str());
		}
		
		//test code
                //(k%5==0)? tmp1 << "1" << std::endl : tmp1 << "2" << std::endl;
		//Move to next MapObject
		pTextDataOut_ << std::endl;
		++it;
                //k++;
	    }
	    
	    //test code
	    //std::ofstream outfile2;
	    //std::string koko = CSVFile_.get() + ".class";
            //outfile2.open(koko.c_str());
	    //outfile2 << tmp1.rdbuf();
	    //outfile2.close();
	    
	    //Output the csv string to the outport
	    setOutPortData();
	}
	catch(itk::ExceptionObject& err)
	{
	    LWARNING("ExceptionObject caught !");
	    return;
	}
    }else if(!this->isReady()){
	LWARNING("Ports not connected");
	return;
    }

    LINFO("Text Data ready!");
    
}


void OTBLabelMapStatisticsWriterProcessor::setOutPortData(){

    outPort_.setData(pTextDataOut_.str());
}

} // namespace
