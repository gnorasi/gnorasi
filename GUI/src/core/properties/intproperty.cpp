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

#include "voreen/core/properties/intproperty.h"

namespace voreen {

IntProperty::IntProperty(const std::string& id, const std::string& guiText,
                    int value, int minValue, int maxValue,
                    Processor::InvalidationLevel invalidationLevel)
    : NumericProperty<int>(id, guiText, value, minValue, maxValue, 1,
                            invalidationLevel)
{}

IntProperty::IntProperty() 
    : NumericProperty<int>("", "", 0, 0, 100, 1, Processor::INVALID_RESULT)
{}

Property* IntProperty::create() const {
    return new IntProperty();
}

Variant IntProperty::getVariant(bool normalized) const {
    if (normalized) {
        float ratio = static_cast<float>(get() - getMinValue()) / static_cast<float>(getMaxValue() - getMinValue());
        return Variant(ratio);
    }
    else
        return Variant(get());
}

void IntProperty::setVariant(const Variant& val, bool normalized) {
    if (normalized) {
        float ratio = val.getFloat();
        set(getMinValue() + static_cast<int>(ratio * (getMaxValue() - getMinValue())));
    }
    else
        set(val.getInt());
}

int IntProperty::getVariantType() const {
    return Variant::VariantTypeInteger;
}

}   // namespace
