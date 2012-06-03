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

#include "otbkmeansimageclassificationprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBKMeansImageClassificationProcessor::loggerCat_("voreen.OTBKMeansImageClassificationProcessor");
  
OTBKMeansImageClassificationProcessor::OTBKMeansImageClassificationProcessor()
    : Processor(),
    inPort_(Port::INPORT, "MultiBand Image Input", 0),
    imageFile_("imageFile", "Output Image", "Image File", VoreenApplication::app()->getDataPath(), "IMG Image file (*.img)", FileDialogProperty::SAVE_FILE),
    clearImage_("clearButton", "Clear Image"),
    numClasses_("numClasses", "Number of Classes", 6.0f, 0.0f, 256.0f),
    saveImageButton_("saveButton", "Save Image")
{
    // register ports and properties
    addPort(inPort_);
    clearImage_.onChange(CallMemberAction<OTBKMeansImageClassificationProcessor>(this, &OTBKMeansImageClassificationProcessor::clearImage));
    saveImageButton_.onChange(CallMemberAction<OTBKMeansImageClassificationProcessor>(this, &OTBKMeansImageClassificationProcessor::saveImage));
    addProperty(imageFile_);
    addProperty(clearImage_);
    addProperty(numClasses_);
    addProperty(saveImageButton_);
    
    //OTB initialization
    writer = WriterType::New();
    filter = ClassificationFilterType::New();
    
}

Processor* OTBKMeansImageClassificationProcessor::create() const {
    return new OTBKMeansImageClassificationProcessor();
}

bool OTBKMeansImageClassificationProcessor::isEndProcessor() const {
    return true;
}

bool OTBKMeansImageClassificationProcessor::isReady() const {
    return (inPort_.isConnected());
}

std::string OTBKMeansImageClassificationProcessor::getProcessorInfo() const {
    return "Classifies a MultiBand Image";
}

void OTBKMeansImageClassificationProcessor::process() {

}

void OTBKMeansImageClassificationProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
    hasImage = false;
    //getProcessorWidget()->updateFromProcessor();
}

void OTBKMeansImageClassificationProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBKMeansImageClassificationProcessor::saveImage() {
    
    std::string filename = imageFile_.get();
    if (!filename.empty())
    {
	hasImage = true;
    }
  
    if(this->isReady() && hasImage)
    {
        try
        {
	    writer->SetFileName(imageFile_.get());
	    
	    const unsigned int sampleSize =
		  ClassificationFilterType::MaxSampleDimension;
	    const unsigned int parameterSize = numClasses_.get() * sampleSize;
	    KMeansParametersType parameters;
	    parameters.SetSize(parameterSize);
	    parameters.Fill(0);
	    
	    for (unsigned int i = 0; i < numClasses_.get(); ++i)
	    {
		for (unsigned int j = 0; j <
		    inPort_.getData()->GetNumberOfComponentsPerPixel(); ++j)
		{
		    parameters[i * sampleSize + j] = atof(argv[4 + i *
		     inPort_.getData()->GetNumberOfComponentsPerPixel() + j]);
                  
		}
	    }
	    
	    filter->SetCentroids(parameters);
	    filter->SetInput(inPort_.getData());
	    writer->SetInput(filter->GetOutput());
	    writer->Update();
	    
	}
	catch(itk::ExceptionObject& err)
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

    LINFO("Classiciaction written succesfully!");
    
}

void OTBKMeansImageClassificationProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

} // namespace
