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

#include "voreen/core/io/serialization/meta/realworldmappingmetadata.h"

namespace voreen {

RealWorldMapping::RealWorldMapping() : scale_(1.0f), offset_(0.0f), unit_("") {
}

RealWorldMapping::RealWorldMapping(float scale, float offset, std::string unit
) : scale_(scale), offset_(offset), unit_(unit) {
}

void RealWorldMapping::serialize(XmlSerializer& s) const {
    s.serialize("scale", scale_);
    s.serialize("offset", offset_);
    s.serialize("unit", unit_);
}

void RealWorldMapping::deserialize(XmlDeserializer& s) {
    s.deserialize("scale", scale_);
    s.deserialize("offset", offset_);
    s.deserialize("unit", unit_);
}

float RealWorldMapping::normalizedToRealWorld(float normalized) const {
    return (normalized * scale_) + offset_; 
}

float RealWorldMapping::realWorldToNormalized(float realWorld) const {
    return (realWorld - offset_) / scale_;
}

std::string RealWorldMapping::getUnit() const {
    return unit_;
}

void RealWorldMapping::setUnit(std::string unit) {
    unit_ = unit;
}

float RealWorldMapping::getScale() const {
    return scale_;
}

void RealWorldMapping::setScale(float scale) {
    scale_ = scale;
}

float RealWorldMapping::getOffset() const {
    return offset_;
}

void RealWorldMapping::setOffset(float offset) {
    offset_ = offset;
}

//--------------------------------------------------------------------------------------

RealWorldMappingMetaData::RealWorldMappingMetaData(float scale, float offset, std::string unit) : PrimitiveMetaDataBase<RealWorldMapping>(RealWorldMapping(scale, offset, unit))
{}

} // namespace
