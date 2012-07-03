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

#include "otbimageport.h"
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

OTBImagePort::ImagePointer OTBImagePort::getData() const {
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
    ImageSmartPointer po = ImageType::New();
    return po.GetPointer();
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
