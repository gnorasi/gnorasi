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

#ifndef VRN_PROPERTYSTATE_H
#define VRN_PROPERTYSTATE_H

#include "voreen/core/io/serialization/serializable.h"
#include "voreen/core/io/serialization/xmlserializer.h"
#include "voreen/core/io/serialization/xmldeserializer.h"
#include <string>

namespace voreen {

class Property;

/**
 * A PropertyState stores the state of a single property along with the \sa PropertyOwner's name, the \sa Property name
 * and the serialized content of the property (thus the state). It is used in the \sa PropertyState collection
 */
class VRN_CORE_API PropertyState : public Serializable {
public:
    PropertyState(Property* prop);
    PropertyState();

    bool operator==(const PropertyState& rhs) const;
    bool operator!=(const PropertyState& rhs) const;

    const std::string& getPropertyOwner() const;
    const std::string& getPropertyName() const;
    const std::string& getPropertyID() const;

    void setPropertyOwner(const std::string& name);

    void serialize(XmlSerializer& s) const;
    void deserialize(XmlDeserializer& s);

    void applyStateToProperty(Property* prop) const;

private:
    std::string propertyOwner_;
    std::string propertyName_;
    std::string propertyID_;
    std::string propertyValue_;
};

} // namespace

#endif // VRN_PROPERTYSTATE_H
