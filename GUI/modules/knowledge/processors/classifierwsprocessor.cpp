#include "classifierwsprocessor.h"

#include "voreen/core/processors/processorwidget.h"
#include "voreen/core/processors/processorwidgetfactory.h"


namespace voreen {
const std::string ClassifierWSProcessor::loggerCat_("voreen.ClassifierWSProcessor");

ClassifierWSProcessor::ClassifierWSProcessor()
    : Processor()
    , inPort_(Port::INPORT, "inport", 0)
    , outPort_(Port::OUTPORT, "outport", 0)
    , serverURLupdate_("serverURLupdate_", "URL for sending data to ontology", "http://160.40.50.66:2020/update/gnorasi")
    , serverURLquery_("serverURLquery_", "URL for issuing queries", "http://160.40.50.66:2020/gnorasi")
    , pTextData_("")
    , pTextDataOut_("")
{
    // register ports and properties
    addPort(inPort_);
    addPort(outPort_);
    addProperty(serverURLupdate_);
    addProperty(serverURLquery_);


}

ClassifierWSProcessor::~ClassifierWSProcessor() {}

Processor* ClassifierWSProcessor::create() const {
    return new ClassifierWSProcessor();
}

void ClassifierWSProcessor::initialize() throw (tgt::Exception) {
    Processor::initialize();

    if (inPort_.hasData()) {
        readData();
        //calculate();
    }
    setOutPortData();

}

void ClassifierWSProcessor::deinitialize() throw (tgt::Exception) {

    //outPort_.setData("");

    pTextData_ = "";
    pTextDataOut_ = "";


    Processor::deinitialize();
}

bool ClassifierWSProcessor::isReady() const {
    return true;
}

bool ClassifierWSProcessor::isEndProcessor() const {
    return (!outPort_.isConnected());
}

void ClassifierWSProcessor::setOutPortData(){
    //if (outPort_.isConnected()){
        outPort_.setData(pTextDataOut_);
    //}
}

void ClassifierWSProcessor::setTextDataOut(std::string outTextData) {
    pTextDataOut_ = outTextData;
    ClassifierWSProcessor::setOutPortData();
}

const std::string ClassifierWSProcessor::getTextData() const {
    return pTextData_;
}


std::string ClassifierWSProcessor::getProcessorInfo() const {
    return "Perform knowledge classification using a knowledge web service";
}

void ClassifierWSProcessor::readData() {
    //if (dynamic_cast<const std::string>(inPort_.getData())) {
        //pTextData_ = dynamic_cast<const std::string>(inPort_.getData());
    pTextData_ = inPort_.getData();
    ClassifierWSProcessor::updateView();
    //}
    /*
    if (!inPort_.hasData())
        pTextData_ = "";
    else {
        pTextData_ = "";
        LWARNING("Data on the Text-Port is not a TextData-Type");
    }
    */
}

void ClassifierWSProcessor::updateView() {
    if (getProcessorWidget()){
        getProcessorWidget()->updateFromProcessor();
    }
}

void ClassifierWSProcessor::process() {
    tgtAssert(isInitialized(), "not initialized");

    if (inPort_.hasData() && inPort_.hasChanged() ) {
        readData();
        //calculate();
    }

    else if (!inPort_.hasData() ){
        pTextData_ = "";

        outPort_.setData(pTextDataOut_);
        if (getProcessorWidget()){
            getProcessorWidget()->updateFromProcessor();
        }
    }

    else if (inPort_.hasData() && !inPort_.hasChanged() ) {
        //calculate();
    }

}


} // namespace

