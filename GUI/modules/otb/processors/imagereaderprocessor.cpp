/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#include "imagereaderprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string ImageReaderProcessor::loggerCat_("voreen.OTBImageReaderProcessor");

ImageReaderProcessor::ImageReaderProcessor()
    : Processor(),
    outPort_(Port::OUTPORT, "outport", 0),
    imageFile_("imageFile", "Image File", "Image File", VoreenApplication::app()->getDataPath()),
    clearImage_("clearButton", "Clear Image")
{
    // register ports and properties
    addPort(outPort_);
    imageFile_.onChange(CallMemberAction<ImageReaderProcessor>(this, &ImageReaderProcessor::loadImage));
    clearImage_.onChange(CallMemberAction<ImageReaderProcessor>(this, &ImageReaderProcessor::clearImage));
    addProperty(imageFile_);
    addProperty(clearImage_);
    
    //OTB stuff
    reader = ReaderType::New();
}

ImageReaderProcessor::~ImageReaderProcessor() {

}

Processor* ImageReaderProcessor::create() const {
    return new ImageReaderProcessor();
}

void ImageReaderProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
    setOutPortData();
    hasImage = false;
}

void ImageReaderProcessor::deinitialize() throw (tgt::Exception) {

    //outPort_.setData("");
    pDataOut_ = 0;
    clearImage();

    Processor::deinitialize();
}

bool ImageReaderProcessor::isReady() const {
    return true;
}

bool ImageReaderProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

void ImageReaderProcessor::setOutPortData(){
    //if (outPort_.isConnected()){
      pDataOut_ = reader->GetOutput();
      outPort_.setData(pDataOut_);
    //}
}

std::string ImageReaderProcessor::getProcessorInfo() const {
    return "Open Image Data to Perform Processing";
}

void ImageReaderProcessor::readData() {

    ImageReaderProcessor::updateView();
    
}

void ImageReaderProcessor::updateView() {
    
}

void ImageReaderProcessor::process() {
    
}

void ImageReaderProcessor::loadImage() {

    // necessary since the passed string reference might be changed during clearImage/invalidate,
    std::string filename = imageFile_.get();

    // check for empty filename
    
    if (filename.empty())
        return;

    // load image as texture
    hasImage = true;
    /*texture_ = TexMgr.load(filename, tgt::Texture::LINEAR,
        false, false, true, false);*/
    reader->SetFileName(filename.c_str());
    
    if (!hasImage) {
        LWARNING("Failed to load image: " << filename);
        imageFile_.set("");
    }

    setOutPortData();
    //invalidate();
}

void ImageReaderProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

ImageReaderProcessor::ImagePointer const ImageReaderProcessor::getImage() const {
    return pDataOut_;
}


} // namespace

