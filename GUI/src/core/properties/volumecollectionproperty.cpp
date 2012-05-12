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

#include "voreen/core/properties/volumecollectionproperty.h"

#include "voreen/core/datastructures/volume/volumecontainer.h"
#include "voreen/core/io/volumeserializerpopulator.h"
#include "voreen/core/io/volumeserializer.h"

namespace voreen {

VolumeCollectionProperty::VolumeCollectionProperty(const std::string& id, const std::string& guiText,
                    VolumeCollection* const value, Processor::InvalidationLevel invalidationLevel) :
                    TemplateProperty<VolumeCollection*>(id, guiText, value, invalidationLevel)
{}

VolumeCollectionProperty::VolumeCollectionProperty() 
    : TemplateProperty<VolumeCollection*>("", "", 0, Processor::INVALID_RESULT)
{}

Property* VolumeCollectionProperty::create() const {
    return new VolumeCollectionProperty();
}

Variant VolumeCollectionProperty::getVariant(bool) const {
    return Variant(get());
}

void VolumeCollectionProperty::setVariant(const Variant& val, bool) {
    set(val.getVolumeCollection());
}

int VolumeCollectionProperty::getVariantType() const {
    return Variant::VariantTypeVolumeCollection;
}

void VolumeCollectionProperty::deinitialize() throw (tgt::Exception) {
    delete value_;
    value_ = 0;
}

void VolumeCollectionProperty::serialize(XmlSerializer& s) const {
    Property::serialize(s);

    s.serialize("VolumeCollection", value_);
}

void VolumeCollectionProperty::deserialize(XmlDeserializer& s) {
    Property::deserialize(s);

    VolumeCollection* collection = 0;
    s.deserialize("VolumeCollection", collection);
    if (collection)
        set(collection);

    invalidate();
}

} // namespace voreen
