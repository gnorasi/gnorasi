#include "otbleeimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBLeeImageFilterProcessor::loggerCat_("voreen.OTBLeeImageFilterProcessor");


OTBLeeImageFilterProcessor::OTBLeeImageFilterProcessor()
    : OTBImageFilterProcessor(),
    radius_("radiusWindowValue", "Radius Window X*X", 3, 3, 15),
    numberOfLooks_("NumberOfLooks", "Number of Looks", 1.0f, 0.0f, 4.0f),
    inPort_(Port::INPORT, "OTBImage.inport", 0),
    outPort_(Port::OUTPORT, "OTBImage.outport", 0)
    {
    addProperty(enableSwitch_);
    addProperty(radius_);
    addProperty(numberOfLooks_);

    addPort(inPort_);
    addPort(outPort_);

    filter = FilterType::New();
    }

Processor* OTBLeeImageFilterProcessor::create() const {
return new OTBLeeImageFilterProcessor();
}

OTBLeeImageFilterProcessor::~OTBLeeImageFilterProcessor() {

}

void OTBLeeImageFilterProcessor::initialize() throw (tgt::Exception) {

Processor::initialize();
}

void OTBLeeImageFilterProcessor::deinitialize() throw (tgt::Exception) {

Processor::deinitialize();
}

std::string OTBLeeImageFilterProcessor::getProcessorInfo() const {

return "Edge-preserving Speckle Filtering Processor";
}

void OTBLeeImageFilterProcessor::process() {
    //check bypass switch
    if (!enableSwitch_.get()){
        bypass(&inPort_, &outPort_);
        return;
    }


    // The method \code{SetRadius()} defines the size of the window to
    // be used for the computation of the local statistics. The method
    // \code{SetNbLooks()} sets the number of looks of the input
    // image.
    try
    {
    filter->SetInput(inPort_.getData());

    FilterType::SizeType Radius;
    Radius[0] = radius_.get();
    Radius[1] = radius_.get();
    filter->SetRadius(Radius);
    filter->SetNbLooks(numberOfLooks_.get());

    outPort_.setData(filter->GetOutput());
    }
    catch (int e)
    {
        LERROR("Problem with Edge Perserving Speckle Image Filter!");
    return;
    }
}




} //namespace




