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

#include "otbcsvreaderprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBCSVReaderProcessor::loggerCat_("voreen.OTBCSVReaderProcessor");
  
OTBCSVReaderProcessor::OTBCSVReaderProcessor()
    : Processor(),
    outPort_(Port::OUTPORT, "Text Data Output", 0),
    CSVFile_("csvfile", "Input File", "Input File Name", VoreenApplication::app()->getDataPath()),
    clearFile_("clearButton", "Clear File")
{
    // register ports and properties
    addPort(outPort_);
    clearFile_.onChange(CallMemberAction<OTBCSVReaderProcessor>(this, &OTBCSVReaderProcessor::clearCSV));
    CSVFile_.onChange(CallMemberAction<OTBCSVReaderProcessor>(this, &OTBCSVReaderProcessor::loadCSV));
    addProperty(CSVFile_);
    addProperty(clearFile_);
    outPort_.setData("");
    hasFileName = false;
}

Processor* OTBCSVReaderProcessor::create() const {
    return new OTBCSVReaderProcessor();
}


bool OTBCSVReaderProcessor::isReady() const {
    return (outPort_.isConnected());
}

std::string OTBCSVReaderProcessor::getProcessorInfo() const {
    return "Loads CSV Data";
}

void OTBCSVReaderProcessor::process() {

}

void OTBCSVReaderProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
    
}

void OTBCSVReaderProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBCSVReaderProcessor::loadCSV() {
    
    std::string filename = CSVFile_.get();
    if (!filename.empty())
    {
	hasFileName = true;
    }
  
    if(hasFileName)
    {
	//Read csv from file
        std::ifstream infile(CSVFile_.get().c_str());
	std::stringstream buffer;
	buffer << infile.rdbuf();
	infile.close();
	
	outPort_.setData(buffer.str());
	
	
    }else if(!hasFileName){
	LWARNING("CSV File Name Not Set");
	return;
    }

    LINFO("CSV data loaded");
    
}

void OTBCSVReaderProcessor::clearCSV() {

    if (hasFileName) {
        hasFileName = false;
	CSVFile_.set("");
	outPort_.setData("");
    }
}

} // namespace
