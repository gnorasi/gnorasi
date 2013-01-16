#include "otbimageviewerprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

const std::string OTBImageViewerProcessor::loggerCat_("voreen.OTBImageViewerProcessor");
  
OTBImageViewerProcessor::OTBImageViewerProcessor()
    : Processor(),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0),
    inPort2_(Port::INPORT, "IN Multi Band Image", 0),
    outPort2_(Port::OUTPORT, "OUT Multi Band Image",0),
    showImageButton_("showButton", "Show Image"),
    loadImageFile_("imageFile", "Load Image", "Load Image", VoreenApplication::app()->getUserDataPath())
{
    // register ports and properties
    addPort(inPort_);
    addPort(outPort_);
    addPort(inPort2_);
    addPort(outPort2_);
    showImageButton_.onChange(CallMemberAction<OTBImageViewerProcessor>(this, &OTBImageViewerProcessor::showImage));
    loadImageFile_.onChange(CallMemberAction<OTBImageViewerProcessor>(this, &OTBImageViewerProcessor::loadImage));
    addProperty(showImageButton_);
    addProperty(loadImageFile_);
    //OTB initialization

    
}

OTBImageViewerProcessor::~OTBImageViewerProcessor() {
    //inPort_.setData(0);
    //outPort_.setData(0);

}

Processor* OTBImageViewerProcessor::create() const {
    return new OTBImageViewerProcessor();
}

bool OTBImageViewerProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

bool OTBImageViewerProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inPort_.isConnected() && !inPort2_.isConnected()) return false;

    return true;
}

std::string OTBImageViewerProcessor::getProcessorInfo() const {
    return "Displays Image Data";
}

void OTBImageViewerProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
}

void OTBImageViewerProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OTBImageViewerProcessor::process() {
    if(!isEndProcessor()){
        outPort_.setData(inPort_.getData());
        outPort2_.setData(inPort2_.getData());
    }

    if(inPort_.isConnected() || inPort2_.isConnected()){
        std::vector<const OTBImagePort*> list = inPort_.getConnected();
        if(!list.empty())
        {
            const OTBImagePort *iPort  = list.at(0);

            inPort_.setImagePath(iPort->imagePath());
        }else {
            std::vector<const OTBVectorImagePort*> list = inPort2_.getConnected();
            if(!list.empty())
            {
                const OTBVectorImagePort *vPort  = list.at(0);

                inPort2_.setImagePath(vPort->imagePath());
            }
        }
    }

    updateView();

    pTextData_.clear();
}

//
void OTBImageViewerProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}

void OTBImageViewerProcessor::showImage() {
    
    if(this->isReady())
    {
//        viewer = ViewerType::New();
//        viewer->SetImage(inPort_.getData());
//        viewer->SetLabel("Simple Image Viewer");
//        viewer->Update();
    }
}

void OTBImageViewerProcessor::loadImage(){
    pTextData_.clear();
    if(inPort_.isConnected())
        return;

    pTextData_ = loadImageFile_.get();

    updateView();
}

} // namespace
