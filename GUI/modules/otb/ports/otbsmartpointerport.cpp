/**********************************************************************
 *                                                                    *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                *
 *                                                                    *
 **********************************************************************/

#include "otbsmartpointerport.h"
#include "voreen/core/datastructures/rendertarget.h"
#include "voreen/core/processors/renderprocessor.h"
#include "tgt/event/event.h"

namespace voreen {

template <class TObjectType>
const std::string OTBSmartPointerPort<TObjectType>::loggerCat_("voreen.OTBSmartPointerPort");

template <class TObjectType>
OTBSmartPointerPort<TObjectType>::OTBSmartPointerPort(PortDirection direction, const std::string& name,
                   bool allowMultipleConnections, Processor::InvalidationLevel invalidationLevel)
    : Port(name, direction, allowMultipleConnections, invalidationLevel)
{
}

template <class TObjectType>
OTBSmartPointerPort<TObjectType>::~OTBSmartPointerPort() {
}

template <class TObjectType>
void OTBSmartPointerPort<TObjectType>::setData(const SmartPointer& pointer) {
    tgtAssert(isOutport(), "called setData on inport!");
    portData_ = pointer;
    invalidate();
}

template <class TObjectType>
typename OTBSmartPointerPort<TObjectType>::SmartPointer OTBSmartPointerPort<TObjectType>::getData() const {
    if (isOutport())
        return portData_;
    else {
        for (size_t i = 0; i < connectedPorts_.size(); ++i) {
            if (!connectedPorts_[i]->isOutport())
                continue;

            OTBSmartPointerPort<TObjectType>* p = static_cast< OTBSmartPointerPort<TObjectType>* >(connectedPorts_[i]);
            if (p)
                return p->getData();
        }
    }
    OTBSmartPointerPort<TObjectType> po = new OTBSmartPointerPort<TObjectType>;
    return po;
}

template <class TObjectType>
std::vector<const OTBSmartPointerPort<TObjectType>*> OTBSmartPointerPort<TObjectType>::getConnected() const {
    std::vector<const OTBSmartPointerPort<TObjectType>*> ports;
    for (size_t i = 0; i < connectedPorts_.size(); ++i) {
        OTBSmartPointerPort<TObjectType>* p = static_cast<OTBSmartPointerPort<TObjectType>*>(connectedPorts_[i]);

        ports.push_back(p);
    }
    return ports;
}

template <class TObjectType>
bool OTBSmartPointerPort<TObjectType>::isReady() const {
    return Port::isReady();
}

template <class TObjectType>
bool OTBSmartPointerPort<TObjectType>::hasData() const {
    return true;
}

} // namespace
