#include "fuzzypreprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

const std::string FuzzyPreProcessor::loggerCat_("voreen.FuzzyPreProcessor");
  
FuzzyPreProcessor::FuzzyPreProcessor()
    : Processor(),
    inLblMapPort_(Port::INPORT, "Input Label Map Port",0),
    outLblMapPort_(Port::OUTPORT, "Output Label Map Port",0),
    outTextPort_(Port::OUTPORT, "Output Text Port")
{
    // register ports and properties
    addPort(inLblMapPort_);
    addPort(outLblMapPort_);
    addPort(outTextPort_);
    //OTB initialization

    
}

FuzzyPreProcessor::~FuzzyPreProcessor() {

}

Processor* FuzzyPreProcessor::create() const {
    return new FuzzyPreProcessor();
}

bool FuzzyPreProcessor::isEndProcessor() const {
    return (!outLblMapPort_.isConnected());
}

bool FuzzyPreProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inLblMapPort_.isConnected() ) return false;

    return true;
}

std::string FuzzyPreProcessor::getProcessorInfo() const {
    return "Fuzzy Calculation";
}

void FuzzyPreProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
}

void FuzzyPreProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void FuzzyPreProcessor::process() {
    if(!isEndProcessor()){
//        outPort2_.setData(inPort2_.getData());
    }

//    if(inLblMapPort_.isConnected() ){
//        std::vector<const OTBLabelMapPort*> list = inPort_.getConnected();
//        if(!list.empty())
//        {
//            const OTBLabelMapPort *iPort  = list.at(0);

////            inLblMapPort_.setImagePath(iPort->imagePath());
//        }else {
//            std::vector<const OTBVectorImagePort*> list = inPort2_.getConnected();
//            if(!list.empty())
//            {
//                const OTBVectorImagePort *vPort  = list.at(0);

//                inPort2_.setImagePath(vPort->imagePath());
//            }
//        }
//    }

    updateView();

//    pTextData_.clear();
}

//
void FuzzyPreProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}


} // namespace
