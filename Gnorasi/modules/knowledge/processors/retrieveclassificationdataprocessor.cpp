/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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

#include "retrieveclassificationdataprocessor.h"

#include "voreen/core/processors/processorwidget.h"
#include "voreen/core/processors/processorwidgetfactory.h"


namespace voreen {
const std::string RetrieveClassificationDataProcessor::loggerCat_("voreen.RetrieveClassificationDataProcessor");

RetrieveClassificationDataProcessor::RetrieveClassificationDataProcessor()
    : Processor()
    , inPort_(Port::INPORT, "inport", 0)
    , outPort_(Port::OUTPORT, "outport", 0)
    , serverURLquery_("serverURLquery_", "URL for issuing queries", "http://localhost:2020/gnorasi")
    , pTextData_("")
    , pTextDataOut_("")
{
    // register ports and properties
    addPort(inPort_);
    addPort(outPort_);
    addProperty(serverURLquery_);


}

RetrieveClassificationDataProcessor::~RetrieveClassificationDataProcessor() {}

Processor* RetrieveClassificationDataProcessor::create() const {
    return new RetrieveClassificationDataProcessor();
}

void RetrieveClassificationDataProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();

    if (inPort_.hasData()) {
        readData();
        //calculate();
    }
    setOutPortData();

}

void RetrieveClassificationDataProcessor::deinitialize() throw (tgt::Exception) {

    //outPort_.setData("");

    pTextData_ = "";
    pTextDataOut_ = "";


    Processor::deinitialize();
}

bool RetrieveClassificationDataProcessor::isReady() const {
    return true;
}

bool RetrieveClassificationDataProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

void RetrieveClassificationDataProcessor::setOutPortData(){
    //if (outPort_.isConnected()){
        outPort_.setData(pTextDataOut_);
    //}
}

void RetrieveClassificationDataProcessor::setTextDataOut(std::string outTextData) {
    pTextDataOut_ = outTextData;
    RetrieveClassificationDataProcessor::setOutPortData();
}

const std::string RetrieveClassificationDataProcessor::getTextData() const {
    return pTextData_;
}


std::string RetrieveClassificationDataProcessor::getProcessorInfo() const {
    return "Retrieve classification data from a knowledge web service";
}

void RetrieveClassificationDataProcessor::readData() {
    //if (dynamic_cast<const std::string>(inPort_.getData())) {
        //pTextData_ = dynamic_cast<const std::string>(inPort_.getData());
    pTextData_ = inPort_.getData();
    RetrieveClassificationDataProcessor::updateView();
    //}
    /*
    if (!inPort_.hasData())
        pTextData_ = "";
    else {
        pTextData_ = "";
        LWARNING("Data on the Text-Port is not a TextData-Type");
    }
    */
}

void RetrieveClassificationDataProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}

void RetrieveClassificationDataProcessor::process() {
    tgtAssert(isInitialized(), "not initialized");

    if (inPort_.hasData() && inPort_.hasChanged() ) {
        readData();
        //calculate();
    }

    else if (!inPort_.hasData() ){
        pTextData_ = "";

        outPort_.setData(pTextDataOut_);
        if (getProcessorWidget()){
            getProcessorWidget()->updateFromProcessor();
        }
    }

    else if (inPort_.hasData() && !inPort_.hasChanged() ) {
        //calculate();
    }

}


} // namespace

