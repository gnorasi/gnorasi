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
    imageFile_("imageFile", "Image File", "Image File", VoreenApplication::app()->getDataPath(), "TIFF Image file (*.tif)", FileDialogProperty::SAVE_FILE),
    clearImage_("clearButton", "Clear Image"),
    saveImageButton_("saveButton", "Save Image")
{
    // register ports and properties
    addPort(inport_);
    clearImage_.onChange(CallMemberAction<ImageWriterProcessor>(this, &ImageWriterProcessor::clearImage));
    saveImageButton_.onChange(CallMemberAction<ImageWriterProcessor>(this, &ImageWriterProcessor::saveImage));
    addProperty(imageFile_);
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
    loadImage(imageFile_.get());
    //getProcessorWidget()->updateFromProcessor();
}

void ImageWriterProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void ImageWriterProcessor::loadImage(const std::string& fname) {

    // necessary since the passed string reference might be changed during clearImage/invalidate,
    std::string filename = fname;

    // clear image and check for empty filename
    if (hasImage) {
        clearImage();
    }
    if (filename.empty())
        return;

    // load image
    hasImage = true;
    
    writer->SetFileName(filename.c_str());
    int_writer->SetFileName(filename.c_str());
    float_writer->SetFileName(filename.c_str());
    byte_writer->SetFileName(filename.c_str());
    
    if (hasImage) {
        imageFile_.set(filename);
    }
    else {
        LWARNING("Failed to load image: " << filename);
        imageFile_.set("");
    }

    //invalidate();
}

void ImageWriterProcessor::saveImage() {
    
    if(this->isReady() && hasImage)
    {
        writer->SetInput(inport_.getData());
        writer->Update();
	LWARNING("Success!");
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
