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

#include "voreen/core/properties/matrixproperty.h"

namespace voreen {

FloatMat2Property::FloatMat2Property(const std::string& id, const std::string& guiText,
                                     const tgt::mat2& value, const tgt::mat2& minimum,
                                     const tgt::mat2& maximum, Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<tgt::mat2>(id, guiText, value, minimum, maximum, tgt::mat2(0.1f),
        invalidationLevel)
{}

FloatMat2Property::FloatMat2Property() 
    : NumericProperty<tgt::mat2>("", "", tgt::mat2(0.f), tgt::mat2(-10.f), tgt::mat2(10.f), tgt::mat2(0.1f),
        Processor::INVALID_RESULT)
{}

Property* FloatMat2Property::create() const {
    return new FloatMat2Property();
}

Variant FloatMat2Property::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = ((get() - getMinValue()) / (getMaxValue() - getMinValue())).t00;
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void FloatMat2Property::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        set(getMinValue() + ratio * (getMaxValue() - getMinValue()));
    }
    else
        set(val.getMat2());
}

int FloatMat2Property::getVariantType() const {
    return Variant::VariantTypeMat2;
}

//---------------------------------------------------------------------------

FloatMat3Property::FloatMat3Property( const std::string& id, const std::string& guiText,
                                     const tgt::mat3& value, const tgt::mat3& minimum,
                                     const tgt::mat3& maximum, Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<tgt::mat3>(id, guiText, value, minimum, maximum, tgt::mat3(0.1f),
        invalidationLevel)
{}

FloatMat3Property::FloatMat3Property() 
    : NumericProperty<tgt::mat3>("", "", tgt::mat3(0.f), tgt::mat3(-10.f), tgt::mat3(10.f), tgt::mat3(0.1f),
        Processor::INVALID_RESULT)
{}

Property* FloatMat3Property::create() const {
    return new FloatMat3Property();
}

Variant FloatMat3Property::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = (get() - (getMinValue()) / (getMaxValue() - getMinValue())).t00;
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void FloatMat3Property::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        set(getMinValue() + ratio * (getMaxValue() - getMinValue()));
    }
    else
        set(val.getMat3());
}

int FloatMat3Property::getVariantType() const {
    return Variant::VariantTypeMat3;
}

//---------------------------------------------------------------------------

FloatMat4Property::FloatMat4Property(const std::string& id, const std::string& guiText,
                                     const tgt::mat4& value, const tgt::mat4& minimum,
                                     const tgt::mat4& maximum, Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<tgt::mat4>(id, guiText, value, minimum, maximum, tgt::mat4(0.1f),
        invalidationLevel)
{}

FloatMat4Property::FloatMat4Property() 
    : NumericProperty<tgt::mat4>("", "", tgt::mat4(0.f), tgt::mat4(-10.f), tgt::mat4(10.f), tgt::mat4(0.1f),
        Processor::INVALID_RESULT)
{}

Property* FloatMat4Property::create() const {
    return new FloatMat4Property();
}

Variant FloatMat4Property::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = (get() - (getMinValue()) / (getMaxValue() - getMinValue())).t00;
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void FloatMat4Property::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        set(getMinValue() + ratio * (getMaxValue() - getMinValue()));
    }
    else
        set(val.getMat4());
}

int FloatMat4Property::getVariantType() const {
    return Variant::VariantTypeMat4;
}

} // namespace voreen
