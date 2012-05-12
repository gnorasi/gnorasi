/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2012 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#include "voreen/core/ports/textport.h"
#include "voreen/core/datastructures/rendertarget.h"
#include "voreen/core/processors/renderprocessor.h"

namespace voreen {

const std::string TextPort::loggerCat_("voreen.TextPort");

TextPort::TextPort(PortDirection direction, const std::string& name,
                   bool allowMultipleConnections, Processor::InvalidationLevel invalidationLevel)
    : Port(name, direction, allowMultipleConnections, invalidationLevel)
{
}

TextPort::~TextPort() {
}

void TextPort::setData(const std::string& data) {
    tgtAssert(isOutport(), "called setData on inport!");
    portData_ = data;
    invalidate();
}

std::string TextPort::getData() const {
    if (isOutport())
        return portData_;
    else {
        for (size_t i = 0; i < connectedPorts_.size(); ++i) {
            if (!connectedPorts_[i]->isOutport())
                continue;

            TextPort* p = static_cast< TextPort* >(connectedPorts_[i]);
            if (p)
                return p->getData();
        }
    }
    return "";
}

std::vector<std::string> TextPort::getAllData() const {
    if (isOutport()) {
        std::vector<std::string> allData;
        allData.push_back(portData_);
        return allData;
    }
    else {
        std::vector<std::string> allData;
        for (size_t i = 0; i < connectedPorts_.size(); ++i) {
            if (connectedPorts_[i]->isOutport() == false)
                continue;
            TextPort* p = static_cast<TextPort*>(connectedPorts_[i]);
            allData.push_back(p->getData());
        }
        return allData;
    }
}

std::vector<const TextPort*> TextPort::getConnected() const {
    std::vector<const TextPort*> ports;
    for (size_t i = 0; i < connectedPorts_.size(); ++i) {
        TextPort* p = static_cast<TextPort*>(connectedPorts_[i]);

        ports.push_back(p);
    }
    return ports;
}

bool TextPort::isReady() const {
    return Port::isReady();
}

bool TextPort::hasData() const {
    return true;
}

} // namespace
