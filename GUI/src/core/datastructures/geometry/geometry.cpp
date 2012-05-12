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

#include "voreen/core/datastructures/geometry/geometry.h"

#include "voreen/core/io/serialization/xmlserializer.h"
#include "voreen/core/utils/hashing.h"

#include <sstream>

namespace voreen {

Geometry::Geometry()
    : changed_(true)
{}

bool Geometry::hasChanged() const {
    return changed_;
}

void Geometry::setHasChanged(bool changed) {
    changed_ = changed;
}

void Geometry::serialize(XmlSerializer& /*s*/) const {
}

void Geometry::deserialize(XmlDeserializer& /*s*/) {
}

std::string Geometry::getHash() const {
    XmlSerializer s;
    s.setUseAttributes(true);
    serialize(s);

    std::stringstream stream;
    s.write(stream);
    return VoreenHash::getHash(stream.str());
}

} // namespace
