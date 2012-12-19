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

#include "otbsimpleviewerprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBSimpleViewerProcessor::loggerCat_("voreen.OTBSimpleViewerProcessor");
  
OTBSimpleViewerProcessor::OTBSimpleViewerProcessor()
    : Processor(),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0),
    showImageButton_("showButton", "Show Image")
{
    // register ports and properties
    addPort(inPort_);
    addPort(outPort_);
    showImageButton_.onChange(CallMemberAction<OTBSimpleViewerProcessor>(this, &OTBSimpleViewerProcessor::showImage));
    addProperty(showImageButton_);
    
    //OTB initialization

    
}

OTBSimpleViewerProcessor::~OTBSimpleViewerProcessor() {
    //inPort_.setData(0);
    //outPort_.setData(0);

}

Processor* OTBSimpleViewerProcessor::create() const {
    return new OTBSimpleViewerProcessor();
}

bool OTBSimpleViewerProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

bool OTBSimpleViewerProcessor::isReady() const {
    return (inPort_.isConnected());
}

std::string OTBSimpleViewerProcessor::getProcessorInfo() const {
    return "Displays Image Data";
}

void OTBSimpleViewerProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
}

void OTBSimpleViewerProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBSimpleViewerProcessor::process() {
    if(!isEndProcessor()){
	outPort_.setData(inPort_.getData());
    }
}

void OTBSimpleViewerProcessor::showImage() {
    
    if(this->isReady())
    {
        viewer = ViewerType::New();
	viewer->SetImage(inPort_.getData());
	viewer->SetLabel("Simple Image Viewer");
	viewer->Update();
	//Fl::check();
	Fl::run();
        Fl::flush();
	Fl::release();
        viewer->Delete();
	return;
    }else{
	LWARNING("Image Inport not connected");
	return;
    }
}

} // namespace
