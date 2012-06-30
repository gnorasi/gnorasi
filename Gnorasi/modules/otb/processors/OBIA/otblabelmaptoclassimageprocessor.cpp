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

#include "otblabelmaptoclassimageprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBLabelMapToClassImageProcessor::loggerCat_("voreen.OTBLabelMapToClassImageProcessor");

OTBLabelMapToClassImageProcessor::OTBLabelMapToClassImageProcessor()
    : Processor(),
    inPort_(Port::INPORT, "Object Map", 0),
    imageFile_("imageFile", "Output Image", "Classification Image File", VoreenApplication::app()->getDataPath(), "IMG Image file (*.img)", FileDialogProperty::SAVE_FILE),
    clearImage_("clearButton", "Clear Classification Image"),
    saveImageButton_("saveButton", "Save Classification Image")
{
    addPort(inPort_);
    clearImage_.onChange(CallMemberAction<OTBLabelMapToClassImageProcessor>(this, &OTBLabelMapToClassImageProcessor::clearImage));
    saveImageButton_.onChange(CallMemberAction<OTBLabelMapToClassImageProcessor>(this, &OTBLabelMapToClassImageProcessor::saveImage));
    addProperty(imageFile_);
    addProperty(clearImage_);
    addProperty(saveImageButton_);
        
    labelMapFilter = LabelMapFilterType::New();
    writer = WriterType::New();
}

OTBLabelMapToClassImageProcessor::~OTBLabelMapToClassImageProcessor() {

}

void OTBLabelMapToClassImageProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
    hasImage = false;
}

void OTBLabelMapToClassImageProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBLabelMapToClassImageProcessor::getProcessorInfo() const {
    
    return "Save Classification Image from Classified Object Map";
}

bool OTBLabelMapToClassImageProcessor::isReady() const {
    return (inPort_.isConnected());
}

void OTBLabelMapToClassImageProcessor::process() {
  
}

void OTBLabelMapToClassImageProcessor::saveImage() {
    
    std::string filename = imageFile_.get();
    if (!filename.empty())
    {
	hasImage = true;
    }
  
    if(this->isReady() && hasImage)
    {
        writer->SetFileName(filename.c_str());
	labelMapFilter->SetInput(inPort_.getData());
	writer->SetInput(labelMapFilter->GetOutput());
	
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
	LWARNING("Object Map inport not connected");
	return;
    }else if(!hasImage){
	LWARNING("Image Name Not Set");
	return;
    }

    LINFO("Classification image written succesfully!");
    
}

void OTBLabelMapToClassImageProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

} // namespace
