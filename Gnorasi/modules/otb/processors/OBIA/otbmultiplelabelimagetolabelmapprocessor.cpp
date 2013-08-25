#include "otbmultiplelabelimagetolabelmapprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OtbMultipleLabelImageToLabelMapProcessor::loggerCat_("voreen.OtbMultipleLabelImageToLabelMapProcessor");

OtbMultipleLabelImageToLabelMapProcessor::OtbMultipleLabelImageToLabelMapProcessor()
    : Processor(),
    inPort_(Port::INPORT,"Label Image",1),
    outPort_(Port::OUTPORT,"Multiple Object Map",0)
{
    addPort(inPort_);
    addPort(outPort_);

    labelMapFilter = LabelMapFilterType::New();
}

Processor* OtbMultipleLabelImageToLabelMapProcessor::create() const {
  return new OtbMultipleLabelImageToLabelMapProcessor();
}

OtbMultipleLabelImageToLabelMapProcessor::~OtbMultipleLabelImageToLabelMapProcessor() {

}

void OtbMultipleLabelImageToLabelMapProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OtbMultipleLabelImageToLabelMapProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OtbMultipleLabelImageToLabelMapProcessor::getProcessorInfo() const {

    return "Transformation of Multiple Label Images to Object Map List ";
}

void OtbMultipleLabelImageToLabelMapProcessor::process() {

    std::vector<const OTBLabelImagePort*> list = inPort_.getConnected();
    std::vector<const OTBLabelImagePort*>::const_iterator i;
    std::vector<LabelMapType*> helperList;
    for(i = list.begin(); i != list.end(); i++){
        const OTBLabelImagePort *port = *i;

        try
        {
        labelMapFilter->SetInput(port->getData());
        labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::min());
        labelMapFilter->Update();
        OtbMultipleLabelImageToLabelMapProcessor::LabelMapType *p = labelMapFilter->GetOutput();
        helperList.push_back(p);
        LINFO("Label Image to Objects Processed");
        }
        catch (int e)
        {
        LERROR("Problem with Label Image to Object Map process!");
        return;
        }
    }

    outPort_.setData(helperList);

}

bool OtbMultipleLabelImageToLabelMapProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

bool OtbMultipleLabelImageToLabelMapProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inPort_.isConnected() ) return false;

    return true;
}


} // namespace
