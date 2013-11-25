#include "ontologyclassificationprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

const std::string OntologyClassificationProcessor::loggerCat_("voreen.OntologyClassificationProcessor");
  
OntologyClassificationProcessor::OntologyClassificationProcessor()
    : Processor(),
    inLblMapPort_(Port::INPORT, "Input Object Map Port",0),
    outTextPort_(Port::OUTPORT, "CSV Text Port"),
    outXmlPort(Port::OUTPORT,"XML Port"),
    XMLFile_("xmlfile", "Output File", "Output File Name", VoreenApplication::app()->getUserDataPath(), "XML Ontology Classification file (*.xml)", FileDialogProperty::SAVE_FILE)
{
    outTextPort_.setDescription("1");
    outXmlPort.setDescription("2");

    // register ports and properties
    addPort(inLblMapPort_);
    addPort(outTextPort_);
    addPort(outXmlPort);

    addProperty(XMLFile_);

}

OntologyClassificationProcessor::~OntologyClassificationProcessor() {

}

Processor* OntologyClassificationProcessor::create() const {
    return new OntologyClassificationProcessor();
}

//bool OntologyClassificationProcessor::isEndProcessor() const {
//    return (!outTextPort_.isConnected());
//    return false;
//}

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

    if(!inLblMapPort_.isConnected())
    {
        LWARNING("Object map is not connected");
        return;
    }

    try{
        updateView();

        LINFO("Ontology Classificaction Processor Processed");
        return;
    }
    catch (int e)
    {
        LERROR("Problem with processing the ontology classification data");
        return;
    }
}

//
void OntologyClassificationProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}

std::string OntologyClassificationProcessor::getFilePath(){

    return XMLFile_.get();
}


} // namespace
