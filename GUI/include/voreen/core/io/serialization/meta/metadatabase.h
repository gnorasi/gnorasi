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

#ifndef VRN_METADATABASE_H
#define VRN_METADATABASE_H

#include "voreen/core/io/serialization/abstractserializable.h"

namespace voreen {

/**
 * Marker interface for meta data classes that are is used to associate data
 * from outside the core with core data for serialization and deserialization.
 *
 * @note When adding a custom meta data type in a module, you also have to provide
 *       a corresponding SerializableFactory that instantiates the custom type.
 *
 * @see VoreenModule::addSerializerFactory, MetaDataFactory
 */
class VRN_CORE_API MetaDataBase : public AbstractSerializable {
public:
    virtual std::string getClassName() const = 0;
    virtual Serializable* create() const = 0;
    virtual MetaDataBase* clone() const = 0;
    virtual std::string toString() const = 0;
};

} // namespace

#endif // VRN_METADATABASE_H
