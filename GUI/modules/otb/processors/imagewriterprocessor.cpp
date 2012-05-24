/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#include "imagewriterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string ImageWriterProcessor::loggerCat_("voreen.OTBImageWriterProcessor");
  
ImageWriterProcessor::ImageWriterProcessor()
    : Processor(),
    inport_(Port::INPORT, "inport", 0),
    imageFile_("imageFile", "Output Image", "Image File", VoreenApplication::app()->getDataPath(), "TIFF Image file (*.tif)", FileDialogProperty::SAVE_FILE),
    imageType_("outputImageType", "Output Image Type"),
    clearImage_("clearButton", "Clear Image"),
    saveImageButton_("saveButton", "Save Image")
{
    imageType_.addOption("double", "64-bit double");
    imageType_.addOption("float", "32-bit float");
    imageType_.addOption("int", "16-bit integer");
    imageType_.addOption("char", "8-bit integer");
      
    // register ports and properties
    addPort(inport_);
    clearImage_.onChange(CallMemberAction<ImageWriterProcessor>(this, &ImageWriterProcessor::clearImage));
    saveImageButton_.onChange(CallMemberAction<ImageWriterProcessor>(this, &ImageWriterProcessor::saveImage));
    addProperty(imageFile_);
    addProperty(imageType_);
    addProperty(clearImage_);
    addProperty(saveImageButton_);
    
    //OTB initialization
    writer = WriterType::New();
    int_writer = IntegerWriterType::New();
    float_writer = FloatWriterType::New();
    byte_writer = ByteWriterType::New();

}

Processor* ImageWriterProcessor::create() const {
    return new ImageWriterProcessor();
}

bool ImageWriterProcessor::isEndProcessor() const {
    return true;
}

bool ImageWriterProcessor::isReady() const {
    return (inport_.isConnected());
}

std::string ImageWriterProcessor::getProcessorInfo() const {
    return "Saves Image Data after Performing Processing";
}

void ImageWriterProcessor::process() {

}

void ImageWriterProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
    hasImage = false;
    //getProcessorWidget()->updateFromProcessor();
}

void ImageWriterProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void ImageWriterProcessor::saveImage() {
    
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
	    writer->Update();
	    LWARNING("Success!");
	}else if (imageType_.get() == "float"){
	    //float_writer->SetFileName(filename.c_str());
            //float_writer->SetInput(inport_.getData());
            //float_writer->Update();
	    LWARNING("Success!");
	}else if (imageType_.get() == "int"){
	    //int_writer->SetFileName(filename.c_str());
	    //int_writer->SetInput(inport_.getData());
            //int_writer->Update();
	    LWARNING("Success!");
	}else if (imageType_.get() == "char"){
	    //byte_writer->SetFileName(filename.c_str());  
	    //byte_writer->SetInput(inport_.getData());
            //byte_writer->Update();
	    LWARNING("Success!");
	}
    }else if(!this->isReady()){
	LWARNING("Writer Inport not connected");
	return;
    }else if(!hasImage){
	LWARNING("Image Name Not Set");
	return;
    }
    
}

void ImageWriterProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

} // namespace
