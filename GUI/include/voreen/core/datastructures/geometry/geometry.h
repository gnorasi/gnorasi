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

#ifndef VRN_GEOMETRY_H
#define VRN_GEOMETRY_H

#include "voreen/core/io/serialization/abstractserializable.h"

#include "tgt/vector.h"
#include "tgt/matrix.h"

#include <string>
#include <vector>
#include <limits>

namespace voreen {

class VolumeHandle;

/**
 * Abstract base class for Geometry objects that
 * can be passed through GeometryPorts.
 *
 * @note In order to enable serialization (necessary for caching),
 *  a factory has to be provided for each concrete subtype.
 *
 * @see GeometryFactory
 * @see VoreenModule::addSerializerFactory
 */
class VRN_CORE_API Geometry : public AbstractSerializable {
public:
    Geometry();
    virtual ~Geometry() {}

    virtual void render() const = 0;

    bool hasChanged() const;
    void setHasChanged(bool changed);

    /**
     * Dummy implementation of the AbstractSerializable interface.
     * Supposed to be overridden by each concrete subclass.
     */
    virtual void serialize(XmlSerializer& s) const;

    /**
     * Dummy implementation of the AbstractSerializable interface.
     * Supposed to be overridden by each concrete subclass.
     */
    virtual void deserialize(XmlDeserializer& s);

    /**
     * Returns a hash of the geometry object,
     * which is used for the caching mechanism.
     *
     * The default implementation computes an MD5 hash
     * of the serialized XML string. Subclasses may
     * override this method, but are not required to do so.
     */
    virtual std::string getHash() const;

protected:
    /// indicates whether the geometry has changed.
    bool changed_;
};

} // namespace

#endif  //VRN_GEOMETRY_H
