/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#include "imagereaderprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string ImageReaderProcessor::loggerCat_("voreen.ClassifierWSProcessor");

ImageReaderProcessor::ImageReaderProcessor()
    : Processor(),
    outPort_(Port::OUTPORT, "outport", 0),
    imageFile_("imageFile", "Image File", "Image File", VoreenApplication::app()->getDataPath()),
    clearImage_("clearButton", "Clear Image"),
    pTextDataOut_("")
{
    // register ports and properties
    addPort(outPort_);
    clearImage_.onChange(CallMemberAction<ImageReaderProcessor>(this, &ImageReaderProcessor::clearImage));
    addProperty(imageFile_);
    addProperty(clearImage_);
}

ImageReaderProcessor::~ImageReaderProcessor() {}

Processor* ImageReaderProcessor::create() const {
    return new ImageReaderProcessor();
}

void ImageReaderProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
    setOutPortData();
    
    loadImage(imageFile_.get());
}

void ImageReaderProcessor::deinitialize() throw (tgt::Exception) {

    //outPort_.setData("");
    pTextDataOut_ = "";
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
        outPort_.setData(pTextDataOut_);
    //}
}

void ImageReaderProcessor::setTextDataOut(std::string outTextData) {
    pTextDataOut_ = outTextData;
    ImageReaderProcessor::setOutPortData();
}


std::string ImageReaderProcessor::getProcessorInfo() const {
    return "Open Image Data to Perforf Processing";
}

void ImageReaderProcessor::readData() {

    ImageReaderProcessor::updateView();
    
}

void ImageReaderProcessor::updateView() {
    
}

void ImageReaderProcessor::process() {
    
}

void ImageReaderProcessor::loadImage(const std::string& fname) {

    // necessary since the passed string reference might be changed during clearImage/invalidate,
    std::string filename = fname;

    // clear image and check for empty filename
    if (hasImage) {
        clearImage();
    }
    if (filename.empty())
        return;

    // load image as texture
    hasImage = true;
    /*texture_ = TexMgr.load(filename, tgt::Texture::LINEAR,
        false, false, true, false);*/
    
    if (hasImage) {
        imageFile_.set(filename);
    }
    else {
        LWARNING("Failed to load image: " << filename);
        imageFile_.set("");
    }

    //invalidate();
}

void ImageReaderProcessor::clearImage() {

    if (hasImage) {
        hasImage = false
    }
    imageFile_.set("");
}


} // namespace

