/**********************************************************************
 * GNORASI                                                            *
 *                                                                    *
 **********************************************************************/

#include "dummysegmentationprocessor.h"

namespace voreen {

DummySegmentationProcessor::DummySegmentationProcessor()
    : RenderProcessor()
    , inport_(Port::INPORT, "inport")
    , outport_(Port::OUTPORT, "text", 0)
{
    // register ports and properties
    addPort(inport_);
    addPort(outport_);
}

Processor* DummySegmentationProcessor::create() const {
    return new DummySegmentationProcessor();
}

std::string DummySegmentationProcessor::getProcessorInfo() const {
    return "Dummy Segmentation, segments an image and outputs region IDs and features";
}

void DummySegmentationProcessor::process() {
    std::cout << "DUMMY PROCESS HERE";
    const std::string dummyXML = "<regions>" \
                     "<region id=\"1\"><msavi>1.23</msavi></region>"\
                     "<region id=\"2\"><msavi>2.33</msavi></region>"\
                     "<region id=\"3\"><msavi>3.33</msavi></region>"\
                     "<region id=\"4\"><msavi>4.33</msavi></region>"\
                     "<region id=\"5\"><msavi>5.33</msavi></region>"\
                     "<region id=\"6\"><msavi>6.33</msavi></region>"\
                     "<region id=\"7\"><msavi>7.33</msavi></region>"\
                     "<region id=\"8\"><msavi>8.33</msavi></region>"\
                     "<region id=\"9\"><msavi>9.33</msavi></region>"\
                     "<region id=\"10\"><msavi>10.33</msavi></region>"\
                     "</regions>";

    outport_.setData(dummyXML);

    // check for OpenGL errors
    //LGL_ERROR;
}

void DummySegmentationProcessor::initialize() throw (VoreenException) {
    // call superclass function first
    RenderProcessor::initialize();
    //getProcessorWidget()->updateFromProcessor();
}

void DummySegmentationProcessor::deinitialize() throw (VoreenException) {
    RenderProcessor::deinitialize();
}

} // namespace
