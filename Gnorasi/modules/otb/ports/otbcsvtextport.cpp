/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
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

#include "otbcsvtextport.h"
#include "voreen/core/datastructures/rendertarget.h"
#include "voreen/core/processors/renderprocessor.h"
#include "tgt/event/event.h"

namespace voreen {

const std::string OTBCSVTextPort::loggerCat_("voreen.OTBCSVTextPort");

OTBCSVTextPort::OTBCSVTextPort(PortDirection direction, const std::string& name,
                   bool allowMultipleConnections, Processor::InvalidationLevel invalidationLevel)
    : TextPort(direction, name, allowMultipleConnections, invalidationLevel)
{
}

OTBCSVTextPort::~OTBCSVTextPort() {
}

std::string OTBCSVTextPort::DataPath() const {
    if (isOutport())
        return m_DataPath;
    else {
        for (size_t i = 0; i < connectedPorts_.size(); ++i) {
            if (!connectedPorts_[i]->isOutport())
                continue;

            OTBCSVTextPort* p = static_cast< OTBCSVTextPort* >(connectedPorts_[i]);
            if (p)
                return p->DataPath();
        }
    }
    std::string po;
    return po;
}

void OTBCSVTextPort::setDataPath(const std::string p) {
    tgtAssert(isOutport(), "called setDataPath on inport!");
    m_DataPath = p;
    invalidate();

}

} // namespace
