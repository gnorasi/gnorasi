/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#include "otbimagereaderprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBImageReaderProcessor::loggerCat_("voreen.OTBImageReaderProcessor");

OTBImageReaderProcessor::OTBImageReaderProcessor()
    : Processor(),
    outPort_(OTBImagePort::OUTPORT, "outport", 0),
    imageFile_("imageFile", "Image File", "Image File", VoreenApplication::app()->getDataPath()),
    clearImage_("clearButton", "Clear Image")
{
    // register ports and properties
    addPort(outPort_);
    imageFile_.onChange(CallMemberAction<OTBImageReaderProcessor>(this, &OTBImageReaderProcessor::loadImage));
    clearImage_.onChange(CallMemberAction<OTBImageReaderProcessor>(this, &OTBImageReaderProcessor::clearImage));
    addProperty(imageFile_);
    addProperty(clearImage_);
    
    //OTB stuff
    reader = ReaderType::New();
}

OTBImageReaderProcessor::~OTBImageReaderProcessor() {

}

Processor* OTBImageReaderProcessor::create() const {
    return new OTBImageReaderProcessor();
}

void OTBImageReaderProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();
    setOutPortData();
    hasImage = false;
}

void OTBImageReaderProcessor::deinitialize() throw (tgt::Exception) {

    //outPort_.setData("");
    pDataOut_ = 0;
    clearImage();

    Processor::deinitialize();
}

bool OTBImageReaderProcessor::isReady() const {
    return true;
}

bool OTBImageReaderProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

void OTBImageReaderProcessor::setOutPortData(){
    //if (outPort_.isConnected()){
      pDataOut_ = reader->GetOutput();
      outPort_.setData(pDataOut_);
    //}
}

std::string OTBImageReaderProcessor::getProcessorInfo() const {
    return "Open Image Data to Perform Processing";
}

void OTBImageReaderProcessor::readData() {

    OTBImageReaderProcessor::updateView();
    
}

void OTBImageReaderProcessor::updateView() {
    
}

void OTBImageReaderProcessor::process() {
    
}

void OTBImageReaderProcessor::loadImage() {

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

void OTBImageReaderProcessor::clearImage() {

    if (hasImage) {
        hasImage = false;
    }
    imageFile_.set("");
}

OTBImageReaderProcessor::ImagePointer const OTBImageReaderProcessor::getImage() const {
    return pDataOut_;
}


} // namespace

