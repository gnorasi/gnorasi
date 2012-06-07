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
    CSVFile_("csvfile", "Output File", "Properties Output File", VoreenApplication::app()->getDataPath(), "CSV Image file (*.csv)", FileDialogProperty::SAVE_FILE),
    clearFile_("clearButton", "Clear File"),
    saveButton_("saveButton", "Save Properties")
{
    // register ports and properties
    addPort(inPort_);
    addPort(outPort_);
    clearFile_.onChange(CallMemberAction<OTBLabelMapStatisticsWriterProcessor>(this, &OTBLabelMapStatisticsWriterProcessor::clearCSV));
    saveButton_.onChange(CallMemberAction<OTBLabelMapStatisticsWriterProcessor>(this, &OTBLabelMapStatisticsWriterProcessor::saveCSV));
    addProperty(CSVFile_);
    addProperty(clearFile_);
    addProperty(saveButton_);
    
    //OTB initialization
    labelmap = 0;
}

Processor* OTBLabelMapStatisticsWriterProcessor::create() const {
    return new OTBLabelMapStatisticsWriterProcessor();
}


bool OTBLabelMapStatisticsWriterProcessor::isReady() const {
    return (inPort_.isConnected());
}

std::string OTBLabelMapStatisticsWriterProcessor::getProcessorInfo() const {
    return "Saves Object Properties";
}

void OTBLabelMapStatisticsWriterProcessor::process() {

}

void OTBLabelMapStatisticsWriterProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
    hasFileName = false;
    
}

void OTBLabelMapStatisticsWriterProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBLabelMapStatisticsWriterProcessor::saveCSV() {
    
    std::string filename = CSVFile_.get();
    if (!filename.empty())
    {
	hasFileName = true;
    }
  
    if(this->isReady() && hasFileName)
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
	    
	    //we need to read the titles of the Properties in the Attribute Table
	    //in this case we read the first LabelObject from the std::map container.
	    typename LabelMapType::LabelObjectContainerType::const_iterator it=
		labelmap->GetLabelObjectContainer().begin();
	    
	    
	    //we now go in the LabelObject type and read the Attributes Name List
	    std::vector<std::string> attrvector = it->second->GetAvailableAttributes();
	    
	    for(int i=0;i<attrvector.size();i++)
	    {
		if (i!=0) pTextDataOut_ << ";";
		pTextDataOut_ << attrvector[i];
	    }
	    pTextDataOut_ << std::endl;
	    
	    //Write the csv header to file
            std::ofstream outfile;
            outfile.open(CSVFile_.get().c_str());
	    outfile << pTextDataOut_.rdbuf();
	    outfile.close();
	    
	    
	    //Output the csv string to the outport
	    setOutPortData();
	}
	catch(itk::ExceptionObject& err)
	{
	    LWARNING("ExceptionObject caught !");
	    return;
	}
    }else if(!this->isReady()){
	LWARNING("Input Object Map is not connected");
	return;
    }else if(!hasFileName){
	LWARNING("Properties File Name Not Set");
	return;
    }

    LINFO("Properties written succesfully!");
    
}

void OTBLabelMapStatisticsWriterProcessor::clearCSV() {

    if (hasFileName) {
        hasFileName = false;
    }
    CSVFile_.set("");
}

void OTBLabelMapStatisticsWriterProcessor::setOutPortData(){

    outPort_.setData(pTextDataOut_.str());
}

} // namespace
