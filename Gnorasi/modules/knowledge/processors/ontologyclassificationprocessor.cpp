#include "ontologyclassificationprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

const std::string OntologyClassificationProcessor::loggerCat_("voreen.OntologyClassificationProcessor");
  
OntologyClassificationProcessor::OntologyClassificationProcessor()
    : Processor(),
    inLblMapPort_(Port::INPORT, "Input Object Map Port",1),
//    outFuzzyRulePort_(Port::OUTPORT, "Output Fuzzy Rule Port"),
    outTextPort_(Port::OUTPORT, "Output Text Port")
//    inOntologyPort_(Port::INPORT,"Input Ontology Port", 0)
{
    // register ports and properties
    addPort(inLblMapPort_);
//    addPort(outFuzzyRulePort_);
    addPort(outTextPort_);
//    addPort(inOntologyPort_);
    //OTB initialization
}

OntologyClassificationProcessor::~OntologyClassificationProcessor() {

}

Processor* OntologyClassificationProcessor::create() const {
    return new OntologyClassificationProcessor();
}

bool OntologyClassificationProcessor::isEndProcessor() const {
    return (!outTextPort_.isConnected());
    return false;
}

bool OntologyClassificationProcessor::isReady() const {
    if (!isInitialized())
        return false;

    if(!inLblMapPort_.isConnected() ) return false;

    return true;
}

std::string OntologyClassificationProcessor::getProcessorInfo() const {
    return "OntologyClassification Processor";
}

void OntologyClassificationProcessor::initialize() throw (VoreenException) {
    Processor::initialize();
}

void OntologyClassificationProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OntologyClassificationProcessor::process() {
    if(!isEndProcessor()){
//        outPort2_.setData(inPort2_.getData());
    }

    updateView();
}

//
void OntologyClassificationProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}


} // namespace
