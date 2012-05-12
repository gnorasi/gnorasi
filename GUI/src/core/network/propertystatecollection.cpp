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

#include "voreen/core/network/propertystatecollection.h"

#include "voreen/core/network/propertystate.h"

using std::string;
using std::vector;

namespace voreen {

PropertyStateCollection::PropertyStateCollection()
    : Serializable()
{}

PropertyStateCollection::PropertyStateCollection(const string& name, const string& timeStamp, const vector<PropertyState*>& states)
    : Serializable()
    , name_(name)
    , timeStamp_(timeStamp)
    , origin_("")
    , states_(states)
{}

PropertyStateCollection::PropertyStateCollection(const std::string& name, const std::string& timeStamp, PropertyState* state) 
    : Serializable()
    , name_(name)
    , timeStamp_(timeStamp)
    , origin_("")
{
    states_.push_back(state);
}

PropertyStateCollection::~PropertyStateCollection() {
    for (size_t i = 0; i < states_.size(); ++i)
        delete states_[i];
}

void PropertyStateCollection::serialize(XmlSerializer& s) const {
    s.serialize("name", name_);
    s.serialize("timestamp", timeStamp_);
    s.serialize("PropertyStates", states_, "PropertyState");
}

void PropertyStateCollection::deserialize(XmlDeserializer& s) {
    s.deserialize("name", name_);
    s.deserialize("timestamp", timeStamp_);
    s.deserialize("PropertyStates", states_, "PropertyState");
}

const std::string& PropertyStateCollection::getName() const {
    return name_;
}

void PropertyStateCollection::setName(const std::string& name) {
    name_ = name;
}

const std::string& PropertyStateCollection::getTimeStamp() const {
    return timeStamp_;
}

void PropertyStateCollection::setTimeStamp(const std::string& timeStamp) {
    timeStamp_ = timeStamp;
}

const std::string& PropertyStateCollection::getOrigin() const {
    return origin_;
}

void PropertyStateCollection::setOrigin(const std::string& origin) {
    origin_ = origin;
}

const std::vector<PropertyState*>& PropertyStateCollection::getPropertyStates() const {
    return states_;
}

void PropertyStateCollection::removePropertyState(PropertyState* state) {
    for (size_t i = 0 ; i < states_.size(); ++i) {
        PropertyState* curState = states_[i];
        if (*curState == *state) {
            delete curState;
            states_.erase(states_.begin() + i);
            return;
        }
    }
}

void PropertyStateCollection::addPropertyState(PropertyState* state) {
    states_.push_back(state);
}

void PropertyStateCollection::removePropertyOwner(const std::string& propertyOwner) {
    std::vector<PropertyState*> toDelete;
    for (size_t i = 0; i < states_.size(); ++i) {
        PropertyState* curState = states_[i];
        if (curState->getPropertyOwner() == propertyOwner)
            toDelete.push_back(curState);
    }

    for (size_t i = 0; i < toDelete.size(); ++i)
        removePropertyState(toDelete[i]);
}

void PropertyStateCollection::renamePropertyOwner(const std::string& oldName, const std::string& newName) {
    for (size_t i = 0; i < states_.size(); ++i) {
        PropertyState* curState = states_[i];
        if (curState->getPropertyOwner() == oldName)
            curState->setPropertyOwner(newName);
    }
}

} // namespace
