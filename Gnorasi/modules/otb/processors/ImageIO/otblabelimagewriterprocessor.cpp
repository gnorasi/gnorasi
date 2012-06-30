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

#include "otblabelimagewriterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBLabelImageWriterProcessor::loggerCat_("voreen.OTBLabelImageWriterProcessor");
  
OTBLabelImageWriterProcessor::OTBLabelImageWriterProcessor()
    : Processor(),
    inport_(Port::INPORT, "Label Image", 0),
    imageFile_("imageFile", "Output Image", "Image File", VoreenApplication::app()->getDataPath(), "IMG Image file (*.img)", FileDialogProperty::SAVE_FILE),
    clearImage_("clearButton", "Clear Image"),
    saveImageButton_("saveButton", "Save Image")
{
    // register ports and properties
    addPort(inport_);
    clearImage_.onChange(CallMemberAction<OTBLabelImageWriterProcessor>(this, &OTBLabelImageWriterProcessor::clearImage));
    saveImageButton_.onChange(CallMemberAction<OTBLabelImageWriterProcessor>(this, &OTBLabelImageWriterProcessor::saveImage));
    addProperty(imageFile_);
    addProperty(clearImage_);
    addProperty(saveImageButton_);
    
    //OTB initialization
    writer = WriterType::New();
    
}

Processor* OTBLabelImageWriterProcessor::create() const {
    return new OTBLabelImageWriterProcessor();
}

bool OTBLabelImageWriterProcessor::isEndProcessor() const {
    return true;
}

bool OTBLabelImageWriterProcessor::isReady() const {
    return (inport_.isConnected());
}

std::string OTBLabelImageWriterProcessor::getProcessorInfo() const {
    return "Saves Label Image Data after Performing Segmentation";
}

void OTBLabelImageWriterProcessor::process() {

}

void OTBLabelImageWriterProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
    hasImage = false;
    //getProcessorWidget()->updateFromProcessor();
}

void OTBLabelImageWriterProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBLabelImageWriterProcessor::saveImage() {
    
    std::string filename = imageFile_.get();
    if (!filename.empty())
    {
	hasImage = true;
    }
  
    if(this->isReady() && hasImage)
    {
        writer->SetFileName(filename.c_str());
	writer->SetInput(inport_.getData());
	try
	{
	    writer->Update();
	}
	catch (itk::ExceptionObject& err)
	{
	    LWARNING("ExceptionObject caught !");
	    return;
	}
    }else if(!this->isReady()){
	LWARNING("Writer Inport not connected");
	return;
    }else if(!hasImage){
	LWARNING("Image Name Not Set");
	return;
    }

    LINFO("Image written succesfully!");
    
}

void OTBLabelImageWriterProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

} // namespace
