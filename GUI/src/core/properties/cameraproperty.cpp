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

#include "voreen/core/properties/cameraproperty.h"

#include "tgt/camera.h"

namespace voreen {

using tgt::Camera;

CameraProperty::CameraProperty(const std::string& id, const std::string& guiText,
                       tgt::Camera const value, bool adjustProjectionToViewport,
                       Processor::InvalidationLevel invalidationLevel) :
        TemplateProperty<Camera>(id, guiText, value, invalidationLevel)
{
    adjustProjectionToViewport_ = adjustProjectionToViewport;
}

CameraProperty::CameraProperty() {}

CameraProperty::~CameraProperty() {}

Property* CameraProperty::create() const {
    return new CameraProperty();
}

void CameraProperty::set(const tgt::Camera& camera) {
    value_ = Camera(camera);
    invalidate();
}

void CameraProperty::setPosition(const tgt::vec3& pos) {
    value_.setPosition(pos);
    notifyChange();
}

void CameraProperty::setFocus(const tgt::vec3& focus) {
    value_.setFocus(focus);
    notifyChange();
}

void CameraProperty::setUpVector(const tgt::vec3& up) {
    value_.setUpVector(up);
    notifyChange();
}

void CameraProperty::setFrustum(const tgt::Frustum& frust) { 
    value_.setFrustum(frust);
    notifyChange();
}

void CameraProperty::setAdjustProjectionToViewport(bool adjust) {
    adjustProjectionToViewport_ = adjust;
}

bool CameraProperty::getAdjustProjectionToViewport() const {
    return adjustProjectionToViewport_;
}

Variant CameraProperty::getVariant(bool) const {
    return Variant(&(get()));
}

void CameraProperty::setVariant(const Variant& val, bool) {
    set(*(val.getCamera()));
}

int CameraProperty::getVariantType() const {
    return Variant::VariantTypeCamera;
}

void CameraProperty::notifyChange() {
    // execute links
    executeLinks();

    // check if conditions are met and exec actions
    for (size_t j = 0; j < conditions_.size(); ++j)
        conditions_[j]->exec();

    updateWidgets();

    // invalidate owner:
    invalidateOwner();
}

void CameraProperty::viewportChanged(const tgt::ivec2& viewport) {

    if (adjustProjectionToViewport_) {
        //value_.setRatio(static_cast<float>(viewport.x) / viewport.y);
        value_.setWindowRatio(static_cast<float>(viewport.x) / viewport.y);
        invalidateOwner(invalidationLevel_);
    }

}

void CameraProperty::serialize(XmlSerializer& s) const {
    Property::serialize(s);

    s.serialize("adjustProjectionToViewport", adjustProjectionToViewport_);
    s.serialize("projectionMode", (int)value_.getProjectionMode());

    s.serialize("position", value_.getPosition());
    s.serialize("focus", value_.getFocus());
    s.serialize("upVector", value_.getUpVector());

    s.serialize("frustLeft", value_.getFrustLeft());
    s.serialize("frustRight", value_.getFrustRight());
    s.serialize("frustBottom", value_.getFrustBottom());
    s.serialize("frustTop", value_.getFrustTop());
    s.serialize("frustNear", value_.getNearDist());
    s.serialize("frustFar", value_.getFarDist());

    s.serialize("fovy", value_.getFovy());
}

void CameraProperty::deserialize(XmlDeserializer& s) {
    Property::deserialize(s);

    s.deserialize("adjustProjectionToViewport", adjustProjectionToViewport_);

    try {
        float left, right, bottom, top, nearP, farP;
        s.deserialize("frustLeft", left);
        s.deserialize("frustRight", right);
        s.deserialize("frustBottom", bottom);
        s.deserialize("frustTop", top);
        s.deserialize("frustNear", nearP);
        s.deserialize("frustFar", farP);
        value_.setFrustum(tgt::Frustum(left, right, bottom, top, nearP, farP));
    } catch(SerializationException&) {
        s.removeLastError();
    }

    int projMode;
    try {
        s.deserialize("projectionMode", projMode);
    } catch(SerializationException&) {
        s.removeLastError();
        projMode = tgt::Camera::PERSPECTIVE;
    }

    value_.setProjectionMode((tgt::Camera::ProjectionMode)projMode);

    tgt::vec3 vector;
    s.deserialize("position", vector);
    value_.setPosition(vector);
    s.deserialize("focus", vector);
    value_.setFocus(vector);
    s.deserialize("upVector", vector);
    value_.setUpVector(vector);

    try {
        float fovy;
        s.deserialize("fovy", fovy);
        value_.setFovy(fovy);
    }
    catch(SerializationException&) {
        s.removeLastError();
    }
}

void CameraProperty::look() {
    value_.look();
}

} // namespace voreen
