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

#include "otbtextwriterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBTextWriterProcessor::loggerCat_("voreen.OTBTextWriterProcessor");
  
OTBTextWriterProcessor::OTBTextWriterProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Text Data Input", 0),
    CSVFile_("csvfile", "Output File", "Output File Name", VoreenApplication::app()->getUserDataPath(), "CSV Image file (*.csv)", FileDialogProperty::SAVE_FILE),
    clearFile_("clearButton", "Clear File"),
    saveButton_("saveButton", "Save CSV")
{
    // register ports and properties
    addPort(inPort_);
    clearFile_.onChange(CallMemberAction<OTBTextWriterProcessor>(this, &OTBTextWriterProcessor::clearCSV));
    saveButton_.onChange(CallMemberAction<OTBTextWriterProcessor>(this, &OTBTextWriterProcessor::saveCSV));
    addProperty(CSVFile_);
    addProperty(clearFile_);
    addProperty(saveButton_);
    
}

Processor* OTBTextWriterProcessor::create() const {
    return new OTBTextWriterProcessor();
}


bool OTBTextWriterProcessor::isReady() const {
    return (inPort_.isConnected());
}

std::string OTBTextWriterProcessor::getProcessorInfo() const {
    return "Saves Text Data";
}

void OTBTextWriterProcessor::process() {

}

void OTBTextWriterProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
    hasFileName = false;
    
}

void OTBTextWriterProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBTextWriterProcessor::saveCSV() {
    
    std::string filename = CSVFile_.get();
    if (!filename.empty())
    {
	hasFileName = true;
    }
  
    if(this->isReady() && hasFileName)
    {
	//Connect with data inport
	inData = inPort_.getData();
	    
	//Write the csv header to file
        std::ofstream outfile;
        outfile.open(CSVFile_.get().c_str());
	outfile << inData;
	outfile.close();
	
	
    }else if(!this->isReady()){
	LWARNING("Input Data not connected");
	return;
    }else if(!hasFileName){
	LWARNING("CSV File Name Not Set");
	return;
    }

    LINFO("CSV written succesfully!");
    
}

void OTBTextWriterProcessor::clearCSV() {

    if (hasFileName) {
        hasFileName = false;
    }
    CSVFile_.set("");
}

} // namespace
