#include "ontologycreatorprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

OntologyCreatorProcessor::OntologyCreatorProcessor()
    : Processor()
    , outPort_(Port::OUTPORT, "Text Data Output", 0)
    , OWLFile_("owlfile", "Input File", "Input File Name", VoreenApplication::app()->getUserDataPath())
    , newOntology_("newButton", "New Ontology")
    , saveFile_("saveButton", "Save File")
{
    // register ports and properties
    addPort(outPort_);
    saveFile_.onChange(CallMemberAction<OntologyCreatorProcessor>(this, &OntologyCreatorProcessor::writeRDF));
    newOntology_.onChange(CallMemberAction<OntologyCreatorProcessor>(this, &OntologyCreatorProcessor::newOntologyContent));
    OWLFile_.onChange(CallMemberAction<OntologyCreatorProcessor>(this, &OntologyCreatorProcessor::loadRDF));
    addProperty(newOntology_);
    addProperty(OWLFile_);
    addProperty(saveFile_);
}

Processor* OntologyCreatorProcessor::create() const {
    return new OntologyCreatorProcessor();
}

void OntologyCreatorProcessor::process() {
    std::vector<const TextPort*> l = outPort_.getConnected();
    if(l.empty())
        return;

    const TextPort *p = l.at(0);

    Processor *processor_ = p->getProcessor();
    if(processor_ && processor_->getProcessorWidget())
        processor_->getProcessorWidget()->updateFromProcessor();


//    p->setData(outPort_.getData());

//    QString text = QString:fromStdString(p->getAllData());



//    TextPort *port = dynamic_cast<TextPort*>
}

void OntologyCreatorProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
}

void OntologyCreatorProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

void OntologyCreatorProcessor::loadRDF(){
    // TODO
    // Add rdf loading stuff , maybe a parser will be needed
//    OwlParser parser;
    pTextData_ = OWLFile_.get();
    OntologyCreatorProcessor::updateView();
}

void OntologyCreatorProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}

void OntologyCreatorProcessor::writeRDF(){
    // TODO
    // Add saving functionality here
    pTextData_ = "save";
    OntologyCreatorProcessor::updateView();
}

void OntologyCreatorProcessor::newOntologyContent(){
//    OntologyCreatorWidget *ontolWidget = qobject_cast<OntologyCreatorWidget*>(getProcessorWidget());

    pTextData_ = "new";
    OntologyCreatorProcessor::updateView();
}

const std::string OntologyCreatorProcessor::getFilePath() const{
    return pTextData_;
}

} // namespace
