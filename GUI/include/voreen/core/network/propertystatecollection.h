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

#ifndef VRN_PROPERTYSTATECOLLECTION_H
#define VRN_PROPERTYSTATECOLLECTION_H

#include "voreen/core/io/serialization/serializable.h"
#include "voreen/core/io/serialization/xmlserializer.h"
#include "voreen/core/io/serialization/xmldeserializer.h"
#include <string>
#include <vector>

namespace voreen {

class PropertyState;

/**
 * A PropertyStateCollection serves as a snapshot of multiple \sa Propertys within the network. It has a name and
 * manages a list of \sa PropertyStates
 */
class VRN_CORE_API PropertyStateCollection : public Serializable {
public:
    PropertyStateCollection();
    PropertyStateCollection(const std::string& name, const std::string& timeStamp, const std::vector<PropertyState*>& states);
    PropertyStateCollection(const std::string& name, const std::string& timeStamp, PropertyState* state);
    ~PropertyStateCollection();

    void serialize(XmlSerializer& s) const;
    void deserialize(XmlDeserializer& s);

    const std::string& getName() const;
    void setName(const std::string& name);

    const std::string& getTimeStamp() const;
    void setTimeStamp(const std::string& timeStamp);

    const std::string& getOrigin() const;
    void setOrigin(const std::string& origin);

    const std::vector<PropertyState*>& getPropertyStates() const;
    void removePropertyState(PropertyState* state);
    void addPropertyState(PropertyState* state);

    void removePropertyOwner(const std::string& propertyOwner);
    void renamePropertyOwner(const std::string& oldName, const std::string& newName);

private:
    std::string name_;
    std::string timeStamp_;
    std::string origin_;
    std::vector<PropertyState*> states_;
};

} // namespace

#endif // VRN_PROPERTYSTATECOLLECTION_H
