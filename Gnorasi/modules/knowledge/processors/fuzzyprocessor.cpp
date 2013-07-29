#include "fuzzyprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

const std::string FuzzyProcessor::loggerCat_("voreen.FuzzyProcessor");
  
FuzzyProcessor::FuzzyProcessor()
    : Processor(),
    inLblMapPort_(Port::INPORT, "Input Label Map Port",0),
    outFuzzyRulePort_(Port::OUTPORT, "Output Fuzzy Rule Port"),
    outTextPort_(Port::OUTPORT, "Output Text Port"),
    inOntologyPort_(Port::INPORT,"Input Ontology Port", 0)
{
    // register ports and properties
    addPort(inLblMapPort_);
    addPort(outFuzzyRulePort_);
    addPort(outTextPort_);
    addPort(inOntologyPort_);
    //OTB initialization

    
}

FuzzyProcessor::~FuzzyProcessor() {

}

Processor* FuzzyProcessor::create() const {
    return new FuzzyProcessor();
}

bool FuzzyProcessor::isEndProcessor() const {
    return (!outFuzzyRulePort_.isConnected());
}

bool FuzzyProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inLblMapPort_.isConnected() ) return false;

    return true;
}

std::string FuzzyProcessor::getProcessorInfo() const {
    return "Fuzzy Processor";
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

    updateView();
}

//
void FuzzyProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}


} // namespace
