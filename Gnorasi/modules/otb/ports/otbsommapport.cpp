/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
 * Copyright (c) ALTEC SA - All rights reserved.                                *
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

#include "otbsommapport.h"
#include "voreen/core/datastructures/rendertarget.h"
#include "voreen/core/processors/renderprocessor.h"
#include "tgt/event/event.h"

namespace voreen {

const std::string OTBSOMMapPort::loggerCat_("voreen.OTBSOMMapPort");

OTBSOMMapPort::OTBSOMMapPort(PortDirection direction, const std::string& name,
                   bool allowMultipleConnections, Processor::InvalidationLevel invalidationLevel)
    : Port(name, direction, allowMultipleConnections, invalidationLevel)
{
}

OTBSOMMapPort::~OTBSOMMapPort() {
}

void OTBSOMMapPort::setData(const SOMMapPointer& pointer) {
    tgtAssert(isOutport(), "called setData on inport!");
    portData_ = pointer;
    invalidate();
}

OTBSOMMapPort::SOMMapPointer OTBSOMMapPort::getData() const {
    if (isOutport())
        return portData_;
    else {
        for (size_t i = 0; i < connectedPorts_.size(); ++i) {
            if (!connectedPorts_[i]->isOutport())
                continue;

            OTBSOMMapPort* p = static_cast< OTBSOMMapPort* >(connectedPorts_[i]);
            if (p)
                return p->getData();
        }
    }
    SOMMapSmartPointer po = SOMMapType::New();
    return po.GetPointer();
}

std::vector<const OTBSOMMapPort*> OTBSOMMapPort::getConnected() const {
    std::vector<const OTBSOMMapPort*> ports;
    for (size_t i = 0; i < connectedPorts_.size(); ++i) {
        OTBSOMMapPort* p = static_cast<OTBSOMMapPort*>(connectedPorts_[i]);

        ports.push_back(p);
    }
    return ports;
}

bool OTBSOMMapPort::isReady() const {
    return Port::isReady();
}

bool OTBSOMMapPort::hasData() const {
    return true;
}

} // namespace
