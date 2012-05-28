/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#include "otbimagefilterprocessor.h"
#include "voreen/core/voreenapplication.h"


namespace voreen {
const std::string OTBImageFilterProcessor::loggerCat_("voreen.OTBImageFilterProcessor");

OTBImageFilterProcessor::OTBImageFilterProcessor()
    : Processor(),
    enableSwitch_("enable", "Enable", true)
{
  
}

OTBImageFilterProcessor::~OTBImageFilterProcessor() {

}

void OTBImageFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBImageFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBImageFilterProcessor::getProcessorInfo() const {
    
    return "Base Filtering Processor";
}

void OTBImageFilterProcessor::bypass(OTBImagePort *inport, OTBImagePort *outport) {
   
    outport->setData(inport->getData());
}

} // namespace

