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

#include "otblabelmapport.h"
#include "voreen/core/datastructures/rendertarget.h"
#include "voreen/core/processors/renderprocessor.h"
#include "tgt/event/event.h"

namespace voreen {

const std::string OTBLabelMapPort::loggerCat_("voreen.OTBLabelMapPort");

OTBLabelMapPort::OTBLabelMapPort(PortDirection direction, const std::string& name,
                   bool allowMultipleConnections, Processor::InvalidationLevel invalidationLevel)
    : Port(name, direction, allowMultipleConnections, invalidationLevel)
{
    portData_ = NULL;
}

OTBLabelMapPort::~OTBLabelMapPort() {
}

void OTBLabelMapPort::setData(const LabelMapPointer& pointer) {
    tgtAssert(isOutport(), "called setData on inport!");
    portData_ = pointer;
    invalidate();
}

OTBLabelMapPort::LabelMapPointer OTBLabelMapPort::getData() const {
    if (isOutport())
        return portData_;
    else {
        for (size_t i = 0; i < connectedPorts_.size(); ++i) {
            if (!connectedPorts_[i]->isOutport())
                continue;

            OTBLabelMapPort* p = static_cast< OTBLabelMapPort* >(connectedPorts_[i]);
            if (p)
                return p->getData();
        }
    }
    LabelMapSmartPointer po = LabelMapType::New();
    return po.GetPointer();
}

std::vector<const OTBLabelMapPort*> OTBLabelMapPort::getConnected() const {
    std::vector<const OTBLabelMapPort*> ports;
    for (size_t i = 0; i < connectedPorts_.size(); ++i) {
        OTBLabelMapPort* p = static_cast<OTBLabelMapPort*>(connectedPorts_[i]);

        ports.push_back(p);
    }
    return ports;
}

bool OTBLabelMapPort::isReady() const {
    return Port::isReady();
}

bool OTBLabelMapPort::hasData() const {
    return true;
}

} // namespace
