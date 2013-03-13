#include "fuzzyprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

const std::string FuzzyProcessor::loggerCat_("voreen.FuzzyProcessor");
  
FuzzyProcessor::FuzzyProcessor()
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

FuzzyProcessor::~FuzzyProcessor() {

}

Processor* FuzzyProcessor::create() const {
    return new FuzzyProcessor();
}

bool FuzzyProcessor::isEndProcessor() const {
    return (!outLblMapPort_.isConnected());
}

bool FuzzyProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inLblMapPort_.isConnected() ) return false;

    return true;
}

std::string FuzzyProcessor::getProcessorInfo() const {
    return "Fuzzy Calculation";
}

void FuzzyProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
}

void FuzzyProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void FuzzyProcessor::process() {
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
void FuzzyProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}


} // namespace
