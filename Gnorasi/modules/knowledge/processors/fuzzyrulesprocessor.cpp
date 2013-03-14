#include "fuzzyrulesprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

const std::string FuzzyRulesProcessor::loggerCat_("voreen.FuzzyRulesProcessor");
  
FuzzyRulesProcessor::FuzzyRulesProcessor()
    : Processor(),
    inLblMapPort_(Port::INPORT, "Input Label Map Port",0),
    inOntologyPort_(Port::INPORT,"Input Ontology Port", 0),
    outFuzzyRulePort_(Port::OUTPORT, "Output Fuzzy Rule Port")
{
    // register ports and properties
    addPort(inLblMapPort_);
    addPort(inOntologyPort_);
    addPort(outFuzzyRulePort_);
    //OTB initialization
    
}

FuzzyRulesProcessor::~FuzzyRulesProcessor() {

}

Processor* FuzzyRulesProcessor::create() const {
    return new FuzzyRulesProcessor();
}

bool FuzzyRulesProcessor::isEndProcessor() const {
    return (!outFuzzyRulePort_.isConnected());
}

bool FuzzyRulesProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inLblMapPort_.isConnected() || !inOntologyPort_.isConnected() ) return false;

    return true;
}

std::string FuzzyRulesProcessor::getProcessorInfo() const {
    return "Fuzzy Rules";
}

void FuzzyRulesProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
}

void FuzzyRulesProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void FuzzyRulesProcessor::process() {
    if(!isEndProcessor()){
//        outPort2_.setData(inPort2_.getData());
    }

    updateView();

//    pTextData_.clear();
}

//
void FuzzyRulesProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}


} // namespace
