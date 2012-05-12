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

#include "voreen/core/properties/vectorproperty.h"

namespace voreen {

FloatVec2Property::FloatVec2Property(const std::string& id, const std::string& guiText,
                             const tgt::vec2& value, const tgt::vec2& minValue,
                             const tgt::vec2& maxValue, Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<tgt::vec2>(id, guiText, value, minValue, maxValue, tgt::vec2(0.01f),
        invalidationLevel)
{}

FloatVec2Property::FloatVec2Property() 
    : NumericProperty<tgt::vec2>("", "", tgt::vec2(0.f), tgt::vec2(0.f), tgt::vec2(1.f), tgt::vec2(0.01f),
        Processor::INVALID_RESULT)
{}

Property* FloatVec2Property::create() const {
    return new FloatVec2Property();
}

Variant FloatVec2Property::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = ((get() - getMinValue()) / (getMaxValue() - getMinValue())).x;
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void FloatVec2Property::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        set(getMinValue() + ratio * (getMaxValue() - getMinValue()));
    }
    else
        set(val.getVec2());
}

int FloatVec2Property::getVariantType() const {
    return Variant::VariantTypeVec2;
}

//---------------------------------------------------------------------------

FloatVec3Property::FloatVec3Property(const std::string& id, const std::string& guiText,
                             const tgt::vec3& value, const tgt::vec3& minValue,
                             const tgt::vec3& maxValue, Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<tgt::vec3>(id, guiText, value, minValue, maxValue, tgt::vec3(0.01f),
        invalidationLevel)
{}

FloatVec3Property::FloatVec3Property() 
    : NumericProperty<tgt::vec3>("", "", tgt::vec3(0.f), tgt::vec3(0.f), tgt::vec3(1.f), tgt::vec3(0.01f),
        Processor::INVALID_RESULT)
{}

Property* FloatVec3Property::create() const {
    return new FloatVec3Property();
}

Variant FloatVec3Property::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = ((get() - getMinValue()) / (getMaxValue() - getMinValue())).x;
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void FloatVec3Property::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        set(getMinValue() + ratio * (getMaxValue() - getMinValue()));
    }
    else
        set(val.getVec3());
}

int FloatVec3Property::getVariantType() const {
    return Variant::VariantTypeVec3;
}

//---------------------------------------------------------------------------

FloatVec4Property::FloatVec4Property(const std::string& id, const std::string& guiText,
                             const tgt::vec4& value, const tgt::vec4& minValue,
                             const tgt::vec4& maxValue, Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<tgt::vec4>(id, guiText, value, minValue, maxValue, tgt::vec4(0.01f),
        invalidationLevel)
{}

FloatVec4Property::FloatVec4Property() 
    : NumericProperty<tgt::vec4>("", "", tgt::vec4(0.f), tgt::vec4(0.f), tgt::vec4(1.f), tgt::vec4(0.01f),
        Processor::INVALID_RESULT)
{}

Property* FloatVec4Property::create() const {
    return new FloatVec4Property();
}

Variant FloatVec4Property::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = ((get() - getMinValue()) / (getMaxValue() - getMinValue())).x;
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void FloatVec4Property::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        set(getMinValue() + ratio * (getMaxValue() - getMinValue()));
    }
    else
        set(val.getVec4());
}

int FloatVec4Property::getVariantType() const {
    return Variant::VariantTypeIVec4;
}

//---------------------------------------------------------------------------

IntVec2Property::IntVec2Property(const std::string& id, const std::string& guiText,
                         const tgt::ivec2& value, const tgt::ivec2& minValue,
                         const tgt::ivec2& maxValue, Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<tgt::ivec2>(id, guiText, value, minValue, maxValue, tgt::ivec2(1),
            invalidationLevel)
{}

IntVec2Property::IntVec2Property() 
    : NumericProperty<tgt::ivec2>("", "", tgt::ivec2(0), tgt::ivec2(0), tgt::ivec2(100), tgt::vec2(1),
        Processor::INVALID_RESULT)
{}

Property* IntVec2Property::create() const {
    return new IntVec2Property();
}

Variant IntVec2Property::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = (static_cast<float>((get() - getMinValue()).x) / static_cast<float>((getMaxValue() - getMinValue()).x));
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void IntVec2Property::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        int x = static_cast<int>(ratio * (getMaxValue().x + getMinValue().x));
        int y = static_cast<int>(ratio * (getMaxValue().y + getMinValue().y));
        set(getMinValue() + tgt::ivec2(x,y));
    }
    else
        set(val.getIVec2());
}

int IntVec2Property::getVariantType() const {
    return Variant::VariantTypeIVec2;
}

//---------------------------------------------------------------------------

IntVec3Property::IntVec3Property(const std::string& id, const std::string& guiText,
                         const tgt::ivec3& value, const tgt::ivec3& minValue,
                         const tgt::ivec3& maxValue, Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<tgt::ivec3>(id, guiText, value, minValue, maxValue, tgt::ivec3(1),
        invalidationLevel)
{}

IntVec3Property::IntVec3Property() 
    : NumericProperty<tgt::ivec3>("", "", tgt::ivec3(0), tgt::ivec3(0), tgt::ivec3(100), tgt::vec3(1),
        Processor::INVALID_RESULT)
{}

Property* IntVec3Property::create() const {
    return new IntVec3Property();
}

Variant IntVec3Property::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = (static_cast<float>((get() - getMinValue()).x) / static_cast<float>((getMaxValue() - getMinValue()).x));
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void IntVec3Property::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        int x = static_cast<int>(ratio * (getMaxValue().x + getMinValue().x));
        int y = static_cast<int>(ratio * (getMaxValue().y + getMinValue().y));
        int z = static_cast<int>(ratio * (getMaxValue().z + getMinValue().z));
        set(getMinValue() + tgt::ivec3(x,y,z));
    }
    else
        set(val.getIVec3());
}

int IntVec3Property::getVariantType() const {
    return Variant::VariantTypeIVec3;
}

//---------------------------------------------------------------------------

IntVec4Property::IntVec4Property(const std::string& id, const std::string& guiText,
                         const tgt::ivec4& value, const tgt::ivec4& minValue,
                         const tgt::ivec4& maxValue, Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<tgt::ivec4>(id, guiText, value, minValue, maxValue, tgt::ivec4(1),
        invalidationLevel)
{}

IntVec4Property::IntVec4Property() 
    : NumericProperty<tgt::ivec4>("", "", tgt::ivec4(0), tgt::ivec4(0), tgt::ivec4(100), tgt::vec4(1),
        Processor::INVALID_RESULT)
{}

Property* IntVec4Property::create() const {
    return new IntVec4Property();
}

Variant IntVec4Property::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = (static_cast<float>((get() - getMinValue()).x) / static_cast<float>((getMaxValue() - getMinValue()).x));
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void IntVec4Property::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        int x = static_cast<int>(ratio * (getMaxValue().x + getMinValue().x));
        int y = static_cast<int>(ratio * (getMaxValue().y + getMinValue().y));
        int z = static_cast<int>(ratio * (getMaxValue().z + getMinValue().z));
        int w = static_cast<int>(ratio * (getMaxValue().w + getMinValue().w));
        set(getMinValue() + tgt::ivec4(x,y,z,w));
    }
    else
        set(val.getIVec4());
}

int IntVec4Property::getVariantType() const {
    return Variant::VariantTypeIVec4;
}

} // namespace voreen
