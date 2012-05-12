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

#ifndef VRN_STRINGPROPERTY_H
#define VRN_STRINGPROPERTY_H

#include "voreen/core/properties/templateproperty.h"

namespace voreen {

#ifdef DLL_TEMPLATE_INST
template class VRN_CORE_API TemplateProperty<std::string>;
#endif

class VRN_CORE_API StringProperty : public TemplateProperty<std::string> {
public:
    StringProperty(const std::string& id, const std::string& guiText, const std::string& value = "",
        Processor::InvalidationLevel invalidationLevel=Processor::INVALID_RESULT);
    StringProperty();
    virtual ~StringProperty() {}

    virtual Property* create() const;

    virtual std::string getClassName() const       { return "StringProperty"; }
    virtual std::string getTypeDescription() const { return "String"; }

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
};

}

#endif
