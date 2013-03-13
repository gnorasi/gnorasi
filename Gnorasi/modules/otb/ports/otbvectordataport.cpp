/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#include "otbvectordataport.h"
#include "voreen/core/datastructures/rendertarget.h"
#include "voreen/core/processors/renderprocessor.h"
#include "tgt/event/event.h"

namespace voreen {

const std::string OTBVectorDataPort::loggerCat_("voreen.OTBVectorDataPort");

OTBVectorDataPort::OTBVectorDataPort(PortDirection direction, const std::string& name,
                   bool allowMultipleConnections, Processor::InvalidationLevel invalidationLevel)
    : Port(name, direction, allowMultipleConnections, invalidationLevel)
{
}

OTBVectorDataPort::~OTBVectorDataPort() {
}

void OTBVectorDataPort::setData(const DataPointer& pointer) {
    tgtAssert(isOutport(), "called setData on inport!");
    portData_ = pointer;
    invalidate();
}

OTBVectorDataPort::DataPointer OTBVectorDataPort::getData() const {
    if (isOutport())
        return portData_;
    else {
        for (size_t i = 0; i < connectedPorts_.size(); ++i) {
            if (!connectedPorts_[i]->isOutport())
                continue;

            OTBVectorDataPort* p = static_cast< OTBVectorDataPort* >(connectedPorts_[i]);
            if (p)
                return p->getData();
        }
    }
    DataSmartPointer po = DataType::New();
    return po.GetPointer();
}

std::vector<const OTBVectorDataPort*> OTBVectorDataPort::getConnected() const {
    std::vector<const OTBVectorDataPort*> ports;
    for (size_t i = 0; i < connectedPorts_.size(); ++i) {
        OTBVectorDataPort* p = static_cast<OTBVectorDataPort*>(connectedPorts_[i]);

        ports.push_back(p);
    }
    return ports;
}

bool OTBVectorDataPort::isReady() const {
    return Port::isReady();
}

bool OTBVectorDataPort::hasData() const {
    return true;
}

} // namespace
