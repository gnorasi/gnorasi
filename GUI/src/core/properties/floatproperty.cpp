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

#include "voreen/core/properties/floatproperty.h"

namespace voreen {

FloatProperty::FloatProperty(const std::string& id, const std::string& guiText,
                     float value, float minValue, float maxValue,
                     Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<float>(id, guiText, value, minValue, maxValue, 0.01f,
                               invalidationLevel)
{
    setViews(Property::View(Property::SLIDER | Property::SPINBOX));
}

FloatProperty::FloatProperty() 
    : NumericProperty<float>("", "", 0.f, 0.f, 100.f, 0.01f, Processor::INVALID_RESULT)
{}

Property* FloatProperty::create() const {
    return new FloatProperty();
}

Variant FloatProperty::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = (get() - getMinValue()) / (getMaxValue() - getMinValue());
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void FloatProperty::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        set(getMinValue() + ratio * (getMaxValue() - getMinValue()));
    }
    else
        set(val.getFloat());
}

int FloatProperty::getVariantType() const {
    return Variant::VariantTypeFloat;
}

}   // namespace
