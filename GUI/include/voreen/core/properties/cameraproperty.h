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

#ifndef VRN_CAMERAPROPERTY_H
#define VRN_CAMERAPROPERTY_H

#include "voreen/core/properties/templateproperty.h"
#include "voreen/core/properties/condition.h"
#include "tgt/camera.h"

namespace voreen {

#ifdef DLL_TEMPLATE_INST
template class VRN_CORE_API TemplateProperty<tgt::Camera>;
#endif

/**
 * Property encapsulating a tgt::Camera object.
 */
class VRN_CORE_API CameraProperty : public TemplateProperty<tgt::Camera> {

public:

    /**
     * Constructor.
     *
     * @param adjustProjectionToViewport when set to true, the camera's projection matrix
     *        is adjusted automatically to viewport changes. This is especially necessary to
     *        reflect the viewport's aspect ratio.
     */
    CameraProperty(const std::string& id, const std::string& guiText, tgt::Camera const value = tgt::Camera(),
               bool adjustProjectionToViewport = true,
               Processor::InvalidationLevel invalidationLevel=Processor::INVALID_RESULT);
    CameraProperty();
    virtual ~CameraProperty();

    virtual Property* create() const;

    virtual std::string getClassName() const       { return "CameraProperty"; }
    virtual std::string getTypeDescription() const { return "Camera"; }

    /**
     * Assigns the passed camera object to the property.
     */
    void set(const tgt::Camera& camera);
    void setPosition(const tgt::vec3& pos);
    void setFocus(const tgt::vec3& focus);
    void setUpVector(const tgt::vec3& up);
    void setFrustum(const tgt::Frustum& frust);

    /**
     * When set to true, the camera's projection matrix is adjusted automatically
     * to viewport changes. This is especially necessary to
     * reflect the viewport's aspect ratio.
     */
    void setAdjustProjectionToViewport(bool adjust);

    /// \sa setAdjustProjectionToViewport
    bool getAdjustProjectionToViewport() const;

    /**
     * Executes all member actions that belong to the property. Generally the owner of the
     * property is invalidated.
     */
    void notifyChange();

    /**
     * Notifies the CameraProperty that the viewport of the canvas it is (indirectly) associated with,
     * has changed. This causes the camera's projection matrix to be updated,
     * if the according constructor parameter has been set.
     */
    void viewportChanged(const tgt::ivec2& viewport);

    /**
     * @see Property::serialize
     */
    virtual void serialize(XmlSerializer& s) const;

    /**
     * @see Property::deserialize
     */
    virtual void deserialize(XmlDeserializer& s);

    virtual Variant getVariant(bool normalized = false) const;
    virtual void setVariant(const Variant& val, bool normalized = false);
    virtual int getVariantType() const;

    virtual void look();
private:
    bool adjustProjectionToViewport_;
};

} // namespace voreen

#endif // VRN_CAMERAPROPERTY_H
