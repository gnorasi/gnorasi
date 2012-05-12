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

#include "voreen/core/network/propertystate.h"

#include "voreen/core/properties/property.h"
#include "voreen/core/properties/propertyowner.h"

namespace voreen {

PropertyState::PropertyState()
    : Serializable()
{}

PropertyState::PropertyState(Property* prop) 
    : Serializable()
{
    propertyOwner_ = prop->getOwner()->getName();
    propertyName_ = prop->getGuiName();
    propertyID_ = prop->getID();

    XmlSerializer s;

    prop->serializeValue(s);

    std::stringstream stream;
    s.write(stream);
    propertyValue_ = stream.str();
}

bool PropertyState::operator==(const PropertyState& rhs) const {
    bool owner = propertyOwner_ == rhs.propertyOwner_;
    bool name = propertyName_ == rhs.propertyName_;
    bool id = propertyID_ == rhs.propertyID_;
    bool value = propertyValue_ == rhs.propertyValue_;
    return owner && name && id && value;
}

bool PropertyState::operator!=(const PropertyState& rhs) const {
    return !(*this == rhs);
}

void PropertyState::applyStateToProperty(Property* prop) const {
    tgtAssert(prop->getOwner()->getName() == propertyOwner_, "Property owner's name is different");
    tgtAssert(prop->getGuiName() == propertyName_, "Property's name is different");
    tgtAssert(prop->getID() == propertyID_, "Property's ID is different");

    XmlDeserializer d;

    std::stringstream stream(propertyValue_);
    d.read(stream);

    prop->deserializeValue(d);
    prop->invalidate();
}

const std::string& PropertyState::getPropertyOwner() const {
    return propertyOwner_;
}

const std::string& PropertyState::getPropertyName() const {
    return propertyName_;
}

void PropertyState::setPropertyOwner(const std::string& name) {
    propertyOwner_ = name;
}

const std::string& PropertyState::getPropertyID() const {
    return propertyID_;
}

void PropertyState::serialize(XmlSerializer& s) const {
    s.serialize("propertyOwner", propertyOwner_);
    s.serialize("propertyName", propertyName_);
    s.serialize("propertyID", propertyID_);
    s.serialize("propertyValue", propertyValue_);
}

void PropertyState::deserialize(XmlDeserializer& s) {
    s.deserialize("propertyOwner", propertyOwner_);
    s.deserialize("propertyName", propertyName_);
    s.deserialize("propertyID", propertyID_);
    s.deserialize("propertyValue", propertyValue_);
}

} // namespace
