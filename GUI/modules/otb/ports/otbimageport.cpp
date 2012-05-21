/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#include "voreen/core/ports/otbimageport.h"
#include "voreen/core/datastructures/rendertarget.h"
#include "voreen/core/processors/renderprocessor.h"
#include "tgt/event/event.h"

namespace voreen {

const std::string OTBImagePort::loggerCat_("voreen.OTBImagePort");

OTBImagePort::OTBImagePort(PortDirection direction, const std::string& name,
                   bool allowMultipleConnections, Processor::InvalidationLevel invalidationLevel)
    : Port(name, direction, allowMultipleConnections, invalidationLevel)
{
}

OTBImagePort::~OTBImagePort() {
}

void OTBImagePort::setData(const ImagePointer& pointer) {
    tgtAssert(isOutport(), "called setData on inport!");
    portData_ = pointer;
    invalidate();
}

ImagePointer OTBImagePort::getData() const {
    if (isOutport())
        return portData_;
    else {
        for (size_t i = 0; i < connectedPorts_.size(); ++i) {
            if (!connectedPorts_[i]->isOutport())
                continue;

            OTBImagePort* p = static_cast< OTBImagePort* >(connectedPorts_[i]);
            if (p)
                return p->getData();
        }
    }
    ImagePointer po = ImageType::New();
    return po;
}

std::vector<const OTBImagePort*> OTBImagePort::getConnected() const {
    std::vector<const OTBImagePort*> ports;
    for (size_t i = 0; i < connectedPorts_.size(); ++i) {
        OTBImagePort* p = static_cast<OTBImagePort*>(connectedPorts_[i]);

        ports.push_back(p);
    }
    return ports;
}

bool OTBImagePort::isReady() const {
    return Port::isReady();
}

bool OTBImagePort::hasData() const {
    return true;
}

} // namespace
