#include "fuzzydefinitionprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

const std::string FuzzyDefinitionProcessor::loggerCat_("voreen.FuzzyDefinitionProcessor");
  
FuzzyDefinitionProcessor::FuzzyDefinitionProcessor()
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

FuzzyDefinitionProcessor::~FuzzyDefinitionProcessor() {

}

Processor* FuzzyDefinitionProcessor::create() const {
    return new FuzzyDefinitionProcessor();
}

bool FuzzyDefinitionProcessor::isEndProcessor() const {
    return (!outFuzzyRulePort_.isConnected());
}

bool FuzzyDefinitionProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inLblMapPort_.isConnected() || !inOntologyPort_.isConnected() ) return false;

    return true;
}

std::string FuzzyDefinitionProcessor::getProcessorInfo() const {
    return "Fuzzy Definition";
}

void FuzzyDefinitionProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
}

void FuzzyDefinitionProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void FuzzyDefinitionProcessor::process() {
    if(!isEndProcessor()){
//        outPort2_.setData(inPort2_.getData());
    }

    updateView();

//    pTextData_.clear();
}

//
void FuzzyDefinitionProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}


} // namespace
