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

#include "otbvectorimagewriterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBVectorImageWriterProcessor::loggerCat_("voreen.OTBVectorImageWriterProcessor");
  
OTBVectorImageWriterProcessor::OTBVectorImageWriterProcessor()
    : Processor(),
    inport_(Port::INPORT, "MultiBand Image Input", 0),
    imageFile_("imageFile", "Output Image", "Image File", VoreenApplication::app()->getDataPath(), "IMG Image file (*.img)", FileDialogProperty::SAVE_FILE),
    clearImage_("clearButton", "Clear Image"),
    imageType_("outputImageType", "Output Image Type"),
    clip_("clipValue","Clip Value", 0.01f, 0.0f, 1.0f),
    min_("minValue","Minimum Output Value", 0.0f, 0.0f, 10000.0f),
    max_("maxValue","Maximum Output Value", 255.0f, 0.0f, 10000.0f),
    saveImageButton_("saveButton", "Save Image")
{
    imageType_.addOption("double", "64-bit double");
    imageType_.addOption("float", "32-bit float");
    imageType_.addOption("int", "16-bit integer");
    imageType_.addOption("char", "8-bit integer");
      
    // register ports and properties
    addPort(inport_);
    clearImage_.onChange(CallMemberAction<OTBVectorImageWriterProcessor>(this, &OTBVectorImageWriterProcessor::clearImage));
    saveImageButton_.onChange(CallMemberAction<OTBVectorImageWriterProcessor>(this, &OTBVectorImageWriterProcessor::saveImage));
    addProperty(imageFile_);
    addProperty(clearImage_);
    addProperty(imageType_);
    addProperty(clip_);
    addProperty(min_);
    addProperty(max_);
    addProperty(saveImageButton_);
    
    //OTB initialization
    writer = WriterType::New();
    int_writer = IntegerWriterType::New();
    intrescaler = IntegerRescalerFilterType::New();
    float_writer = FloatWriterType::New();
    floatrescaler = FloatRescalerFilterType::New();
    byte_writer = ByteWriterType::New();
    byterescaler = ByteRescalerFilterType::New();

}

Processor* OTBVectorImageWriterProcessor::create() const {
    return new OTBVectorImageWriterProcessor();
}

bool OTBVectorImageWriterProcessor::isEndProcessor() const {
    return true;
}

bool OTBVectorImageWriterProcessor::isReady() const {
    return (inport_.isConnected());
}

std::string OTBVectorImageWriterProcessor::getProcessorInfo() const {
    return "Saves MultiBand Image Data after Performing Processing";
}

void OTBVectorImageWriterProcessor::process() {

}

void OTBVectorImageWriterProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
    hasImage = false;
    //getProcessorWidget()->updateFromProcessor();
}

void OTBVectorImageWriterProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBVectorImageWriterProcessor::saveImage() {
    
    std::string filename = imageFile_.get();
    if (!filename.empty())
    {
	hasImage = true;
    }
  
    if(this->isReady() && hasImage)
    {
        if (imageType_.get() == "double") {
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
	}else if (imageType_.get() == "float"){
	    try
            {
		FloatImageType::PixelType minimum, maximum;
		minimum.SetSize(inport_.getData()->GetNumberOfComponentsPerPixel());
		maximum.SetSize(inport_.getData()->GetNumberOfComponentsPerPixel());
		minimum.Fill(min_.get());
                maximum.Fill(max_.get());
		floatrescaler->SetInput(inport_.getData());
		floatrescaler->SetOutputMinimum(minimum);
		floatrescaler->SetOutputMaximum(maximum);
		floatrescaler->SetClampThreshold(clip_.get());
		float_writer->SetFileName(filename.c_str());  
		float_writer->SetInput(floatrescaler->GetOutput());
		float_writer->Update();
	    }
	    catch (itk::ExceptionObject& err)
            {
        	LWARNING("ExceptionObject caught !");
        	return;
            }
	}else if (imageType_.get() == "int"){
	    try
            {
		IntegerImageType::PixelType minimum, maximum;
		minimum.SetSize(inport_.getData()->GetNumberOfComponentsPerPixel());
		maximum.SetSize(inport_.getData()->GetNumberOfComponentsPerPixel());
		minimum.Fill(min_.get());
                maximum.Fill(max_.get());
		intrescaler->SetInput(inport_.getData());
		intrescaler->SetOutputMinimum(minimum);
		intrescaler->SetOutputMaximum(maximum);
		intrescaler->SetClampThreshold(clip_.get());
		int_writer->SetFileName(filename.c_str());  
		int_writer->SetInput(intrescaler->GetOutput());
		int_writer->Update();
	    }
	    catch (itk::ExceptionObject& err)
            {
        	LWARNING("ExceptionObject caught !");
        	return;
            }
	}else if (imageType_.get() == "char"){
	    try
            {
		ByteImageType::PixelType minimum, maximum;
		minimum.SetSize(inport_.getData()->GetNumberOfComponentsPerPixel());
		maximum.SetSize(inport_.getData()->GetNumberOfComponentsPerPixel());
		minimum.Fill(min_.get());
                maximum.Fill(max_.get());
		byterescaler->SetInput(inport_.getData());
		byterescaler->SetOutputMinimum(minimum);
		byterescaler->SetOutputMaximum(maximum);
		byterescaler->SetClampThreshold(clip_.get());
		byte_writer->SetFileName(filename.c_str());  
		byte_writer->SetInput(byterescaler->GetOutput());
		byte_writer->Update();
	    }
	    catch (itk::ExceptionObject& err)
            {
        	LWARNING("ExceptionObject caught !");
        	return;
            }
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

void OTBVectorImageWriterProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

} // namespace
