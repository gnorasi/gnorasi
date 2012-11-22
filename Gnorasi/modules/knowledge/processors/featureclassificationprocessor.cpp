#include "featureclassificationprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"


namespace voreen {

FeatureClassificationProcessor::FeatureClassificationProcessor()
    : Processor()
    , outPort_(Port::OUTPORT, "Text Data Output", 0)
    , OWLFile_("owlfile", "Input File", "Input File Name", VoreenApplication::app()->getUserDataPath())
    , newOntology_("newButton", "New Ontology")
    , saveFile_("saveButton", "Save File")
{
    // register ports and properties
    addPort(outPort_);
    saveFile_.onChange(CallMemberAction<FeatureClassificationProcessor>(this, &FeatureClassificationProcessor::writeRDF));
    newOntology_.onChange(CallMemberAction<FeatureClassificationProcessor>(this, &FeatureClassificationProcessor::newOntologyContent));
    OWLFile_.onChange(CallMemberAction<FeatureClassificationProcessor>(this, &FeatureClassificationProcessor::loadRDF));
    addProperty(newOntology_);
    addProperty(OWLFile_);
    addProperty(saveFile_);
}

Processor* FeatureClassificationProcessor::create() const {
    return new FeatureClassificationProcessor();
}

void FeatureClassificationProcessor::process() {

}

void FeatureClassificationProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
}

void FeatureClassificationProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void FeatureClassificationProcessor::loadRDF(){
    // TODO
    // Add rdf loading stuff , maybe a parser will be needed
//    OwlParser parser;
    pTextData_ = OWLFile_.get();
    FeatureClassificationProcessor::updateView();
}

void FeatureClassificationProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}

void FeatureClassificationProcessor::writeRDF(){
    // TODO
    // Add saving functionality here
    pTextData_ = "save";
    FeatureClassificationProcessor::updateView();
}

void FeatureClassificationProcessor::newOntologyContent(){
//    OntologyCreatorWidget *ontolWidget = qobject_cast<OntologyCreatorWidget*>(getProcessorWidget());

    pTextData_ = "new";
    FeatureClassificationProcessor::updateView();
}

const std::string FeatureClassificationProcessor::getFilePath() const{
    return pTextData_;
}

} // namespace
