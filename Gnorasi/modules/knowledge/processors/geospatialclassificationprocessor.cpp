#include "geospatialclassificationprocessor.h"
#include "voreen/core/voreenapplication.h"
#include "voreen/core/processors/processorwidget.h"

namespace voreen {

GeospatialClassificationProcessor::GeospatialClassificationProcessor()
    : Processor()
    , outPort_(Port::OUTPORT, "Geospatial Classification Data Output", 0)
    , inPort_(Port::INPORT, "Ontology Data Input", 1)
    , saveFile_("saveGCButton", "Save File")
    , openFile_("xmlFile", "Input xml file", "Input xml file", VoreenApplication::app()->getUserDataPath())
{
    saveFile_.onChange(CallMemberAction <GeospatialClassificationProcessor>(this, &GeospatialClassificationProcessor::writeXML));
    openFile_.onChange(CallMemberAction<GeospatialClassificationProcessor>(this, &GeospatialClassificationProcessor::openXML));
    // register ports and properties
    addPort(outPort_);
    addPort(inPort_);
    addProperty(openFile_);
    addProperty(saveFile_);
}

Processor* GeospatialClassificationProcessor::create() const {
    return new GeospatialClassificationProcessor();
}

void GeospatialClassificationProcessor::process() {
    std::cout << "processing geospatial classification function entered ..";

    inPort_.getAllData();

    updateView();
}

//!
//! Open an xml containing rules
//!
void GeospatialClassificationProcessor::openXML(){
    // TODO
    // load the xml file content
    pTextData_ = "open";
    pFilePath_ = openFile_.get();

    updateView();

    // reset value
    pTextData_ = "";
}

//!
//! save the rules data to an xml file
//!
void GeospatialClassificationProcessor::writeXML(){

    pTextData_ = "save";

    updateView();

    // reset value
    pTextData_ = "";
}

//
void GeospatialClassificationProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    Processor::initialize();
}

//
void GeospatialClassificationProcessor::deinitialize() throw (VoreenException) {
    Processor::deinitialize();
}

//
void GeospatialClassificationProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}

} // namespace voreen
