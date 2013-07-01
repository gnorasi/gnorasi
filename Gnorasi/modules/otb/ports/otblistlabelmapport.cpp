#include "otblistlabelmapport.h"

namespace voreen {

const std::string OTBListLabelMapPort::loggerCat_("voreen.OTBListLabelMapPort");

OTBListLabelMapPort::OTBListLabelMapPort(PortDirection direction, const std::string& name,
                 bool allowMultipleConnections, Processor::InvalidationLevel invalidationLevel)
  : Port(name, direction, allowMultipleConnections, invalidationLevel)
{

}

OTBListLabelMapPort::~OTBListLabelMapPort(){

}

void OTBListLabelMapPort::setData(const std::vector<LabelMapPointer> &list) {
    tgtAssert(isOutport(), "called setData on inport!");
    portData_.clear();
    portData_.insert(portData_.end(),list.begin(),list.end());
    invalidate();
}

std::vector<OTBListLabelMapPort::LabelMapPointer> OTBListLabelMapPort::getData() const {
//    if (isOutport())
//        return portData_;
//    else {
//        for (size_t i = 0; i < connectedPorts_.size(); ++i) {
//            if (!connectedPorts_[i]->isOutport())
//                continue;

//            OTBListLabelMapPort* p = static_cast< OTBListLabelMapPort* >(connectedPorts_[i]);
//            if (p){
//                std::vector<OTBListLabelMapPort::LabelMapPointer> _helperList = p->getData();
//                portData_.insert(portData_.end(),_helperList.begin(),_helperList.end());
//            }
//        }
//    }

    return portData_;
}

std::vector<const OTBListLabelMapPort*> OTBListLabelMapPort::getConnected() const {
    std::vector<const OTBListLabelMapPort*> ports;
    for (size_t i = 0; i < connectedPorts_.size(); ++i) {
        OTBListLabelMapPort* p = static_cast<OTBListLabelMapPort*>(connectedPorts_[i]);

        ports.push_back(p);
    }
    return ports;
}

bool OTBListLabelMapPort::isReady() const {
    return Port::isReady();
}

bool OTBListLabelMapPort::hasData() const {
    return true;
}

}
