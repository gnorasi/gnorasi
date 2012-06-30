/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * 										*
 *                                                                    		*
 * This file is part of the GNORASI software package. GNORASI is free  		*
 * software: you can redistribute it and/or modify it under the terms 		*
 * of the GNU General Public License version 2 as published by the    		*
 * Free Software Foundation.                                          		*
 *                                                                    		*
 * GNORASI is distributed in the hope that it will be useful,          		*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     		*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       		*
 * GNU General Public License for more details.                       		*
 *                                                                    		*
 * You should have received a copy of the GNU General Public License  		*
 * in the file "LICENSE.txt" along with this program.                 		*
 * If not, see <http://www.gnu.org/licenses/>.                        		*
 *                                                                    		*
 ********************************************************************************/

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
