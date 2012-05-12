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

#include "voreen/core/io/serialization/meta/networkannotation.h"

namespace voreen {

NetworkAnnotation::NetworkAnnotation() 
    : Serializable()
    , text_("")
    , positionX_(0)
    , positionY_(0)
    , hasFence_(false)
    , positionFenceX_(0)
    , positionFenceY_(0)
{}

void NetworkAnnotation::serialize(XmlSerializer& s) const {
    s.serialize("Note", text_);
    s.serialize("PositionX", positionX_);
    s.serialize("PositionY", positionY_);
    s.serialize("hasFence", hasFence_);
    s.serialize("PositionFenceX_", positionFenceX_);
    s.serialize("PositionFenceY_", positionFenceY_);
}

void NetworkAnnotation::deserialize(XmlDeserializer& s) {
    s.deserialize("Note", text_);
    s.deserialize("PositionX", positionX_);
    s.deserialize("PositionY", positionY_);
    s.deserialize("hasFence", hasFence_);
    s.deserialize("PositionFenceX_", positionFenceX_);
    s.deserialize("PositionFenceY_", positionFenceY_);
}

void NetworkAnnotation::setPosition(int x, int y) {
    positionX_ = x;
    positionY_ = y;
}

std::pair<int,int> NetworkAnnotation::getPosition() const {
    return std::make_pair(positionX_, positionY_);
}

void NetworkAnnotation::setText(const std::string& text) {
    text_ = text;
}

const std::string& NetworkAnnotation::getText() const {
    return text_;
}

bool NetworkAnnotation::hasFence() const {
    return hasFence_;
}

std::pair<int,int> NetworkAnnotation::getFencePosition() const {
    return std::make_pair(positionFenceX_, positionFenceY_);
}

void NetworkAnnotation::setFencePosition(int x, int y) {
    positionFenceX_ = x;
    positionFenceY_ = y;
    hasFence_ = true;
}

//-------------------------------------------------------------------------------------------------


NetworkAnnotationContainer::NetworkAnnotationContainer() {}

Serializable* NetworkAnnotationContainer::create() const {
    return new NetworkAnnotationContainer;
}

MetaDataBase* NetworkAnnotationContainer::clone() const {
    NetworkAnnotationContainer* md = new NetworkAnnotationContainer();
    md->annotations_ = annotations_;
    return md;
}

std::string NetworkAnnotationContainer::toString() const {
    return "";
}

void NetworkAnnotationContainer::serialize(XmlSerializer& s) const {
    if (!annotations_.empty())
        s.serialize("Annotations", annotations_);
    
    s.serialize("ShowAnnotations", showAnnotations_);
}

void NetworkAnnotationContainer::deserialize(XmlDeserializer& s) {
    s.deserialize("Annotations", annotations_);
    s.deserialize("ShowAnnotations", showAnnotations_);
}

void NetworkAnnotationContainer::addNetworkAnnotation(NetworkAnnotation* annotation) {
    annotations_.push_back(annotation);
}

void NetworkAnnotationContainer::removeNetworkAnnotation(NetworkAnnotation* annotation) {
    for (size_t i = 0; i < annotations_.size(); ++i)
        if (annotations_[i] == annotation) {
            annotations_.erase(annotations_.begin() + i);
            break;
        }
}

void NetworkAnnotationContainer::clearNetworkAnnotations() {
    annotations_.clear();
}

bool NetworkAnnotationContainer::isEmpty() const {
    return annotations_.empty();
}

const std::vector<NetworkAnnotation*>& NetworkAnnotationContainer::getAnnotations() const {
    return annotations_;
}

bool NetworkAnnotationContainer::getShowAnnotations() const {
    return showAnnotations_;
}

void NetworkAnnotationContainer::setShowAnnotations(bool v) {
    showAnnotations_ = v;
}

} // namespace
