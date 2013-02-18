/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/


#include "otborthorectifyprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {
const std::string OTBOrthoRectifyProcessor::loggerCat_("voreen.OTBOrthoRectifyProcessor");

OTBOrthoRectifyProcessor::OTBOrthoRectifyProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    updateButton("updateOrthoButton", "Update"),
    outPort_(Port::OUTPORT, "Vector Image Output", 0)
{
    updateButton.onChange(CallMemberAction <OTBOrthoRectifyProcessor>(this, &OTBOrthoRectifyProcessor::updateData));
    addPort(inPort_);
    addPort(outPort_);
    addProperty(updateButton);
}

Processor* OTBOrthoRectifyProcessor::create() const {
  return new OTBOrthoRectifyProcessor();
}

void OTBOrthoRectifyProcessor::updateData(){
    pTextData = "update";

    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }

    pTextData = "";
}

bool OTBOrthoRectifyProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

bool OTBOrthoRectifyProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inPort_.isConnected()) return false;

    return true;
}

OTBOrthoRectifyProcessor::~OTBOrthoRectifyProcessor() {

}

void OTBOrthoRectifyProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBOrthoRectifyProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBOrthoRectifyProcessor::getProcessorInfo() const {
    
    return "Ortho Rectify from Vector Image";
}

void OTBOrthoRectifyProcessor::process() {
  
    if (inPort_.isConnected() && getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
    
}

} // namespace
