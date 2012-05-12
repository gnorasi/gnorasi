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

#ifndef VRN_VOLUMEHANDLEPROPERTY_H
#define VRN_VOLUMEHANDLEPROPERTY_H

#include "voreen/core/properties/templateproperty.h"
#include "voreen/core/properties/condition.h"

namespace voreen {

class VolumeHandle;

#ifdef DLL_TEMPLATE_INST
template class TemplateProperty<VolumeHandle*>;
#endif
    
/**
 * Property encapsulating a VolumeHandle object.
 */
class VRN_CORE_API VolumeHandleProperty : public TemplateProperty<VolumeHandle*> {
public:
    /**
     * Constructor.
     *
     * @param ident identifier that is used in serialization
     * @param guiText text that is shown in the gui
     * @param value the initial volume handle to assign
     *
     */
    VolumeHandleProperty(const std::string& id, const std::string& guiText, VolumeHandle* const value = 0,
       Processor::InvalidationLevel invalidationLevel = Processor::INVALID_PARAMETERS);
    VolumeHandleProperty();

    virtual Property* create() const;

    virtual std::string getClassName() const       { return "VolumeHandleProperty"; }
    virtual std::string getTypeDescription() const { return "VolumeHandle"; } 

    virtual Variant getVariant(bool normalized = false) const;
    virtual void setVariant(const Variant& val, bool normalized = false);
    virtual int getVariantType() const;

    /**
     * Sets the stored volume handle to the given one.
     *
     * @note The property does not take ownership of the
     *       assigned volume handle and does therefore NOT
     *       delete it on destruction or when a new handle is assigned.
     */
    void set(VolumeHandle* handle);

    /**
     * Loads the volume specified by filename.
     *
     * @note The property takes ownership of the loaded
     *       volume handle and deletes it on its own
     *       destruction or when a new handle is assigned.
     *
     * @param filename the volume to load
     */
    void loadVolume(const std::string& filename)
        throw (tgt::FileException, std::bad_alloc);

    /// @see Property::serialize
    virtual void serialize(XmlSerializer& s) const;

    /// @see Property::deserialize
    virtual void deserialize(XmlDeserializer& s);

protected:
    /// Deletes the assigned volume, if it is owned by the property.
    virtual void deinitialize() throw (tgt::Exception);

    bool handleOwner_;

};

} // namespace voreen

#endif // VRN_VOLUMEHANDLEPROPERTY_H
