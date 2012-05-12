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

#include "voreen/core/datastructures/geometry/vertexgeometry.h"

#include "tgt/glmath.h"
#include "voreen/core/io/serialization/xmlserializer.h"
#include "voreen/core/io/serialization/xmldeserializer.h"

namespace voreen {

VertexGeometry::VertexGeometry(const tgt::vec3& coords, const tgt::vec3& texcoords, const tgt::vec4& color)
    : coords_(coords)
    , texcoords_(texcoords)
    , color_(color)
    , normalIsSet_(false)
    , normal_(tgt::vec3(0.f))
{}

VertexGeometry::VertexGeometry(const tgt::vec3& coords, const tgt::vec3& texcoords, const tgt::vec4& color, const tgt::vec3& normal)
    : coords_(coords)
    , texcoords_(texcoords)
    , color_(color)
    , normalIsSet_(true)
    , normal_(normal)
{}

tgt::vec3 VertexGeometry::getCoords() const {
    return coords_;
}

void VertexGeometry::setCoords(const tgt::vec3& coords) {
    coords_ = coords;

    setHasChanged(true);
}

tgt::vec3 VertexGeometry::getTexCoords() const {
    return texcoords_;
}

void VertexGeometry::setTexCoords(const tgt::vec3& texcoords) {
    texcoords_ = texcoords;

    setHasChanged(true);
}

tgt::vec4 VertexGeometry::getColor() const {
    return color_;
}

void VertexGeometry::setColor(const tgt::vec4& color) {
    color_ = color;

    setHasChanged(true);
}

void VertexGeometry::setColor(const tgt::vec3& color) {
    color_ = tgt::vec4(color, 1);

    setHasChanged(true);
}

tgt::vec3 VertexGeometry::getNormal() const {
    tgtAssert(normalIsSet_, "Tried to access the normal when it was not set");
    return normal_;
}

void VertexGeometry::setNormal(const tgt::vec3& normal) {
    normal_ = normal;
    normalIsSet_ = true;

    setHasChanged(true);
}

void VertexGeometry::removeNormal() {
    normal_ = tgt::vec3(0.f);
    normalIsSet_ = false;
}

bool VertexGeometry::isNormalDefined() const {
    return normalIsSet_;
}

double VertexGeometry::getLength() const {
    return tgt::length(coords_);
}

double VertexGeometry::getDistanceToPlane(const tgt::vec4& plane, double epsilon) const {
    double distance = tgt::dot(plane.xyz(), coords_) - plane.w;
    if (std::abs(distance) <= epsilon)
        return 0;
    else
        return distance;
}

double VertexGeometry::getDistance(const VertexGeometry& vertex) const {
    return tgt::length(vertex.coords_ - coords_);
}

void VertexGeometry::render() const {
    glBegin(GL_POINTS);

    glColor4fv(color_.elem);
    if (normalIsSet_)
        tgt::normal(normal_);
    tgt::texCoord(texcoords_);
    tgt::vertex(coords_);

    glEnd();
}

void VertexGeometry::combine(const VertexGeometry& vertex) {
    interpolate(vertex, 0.5);
}

VertexGeometry VertexGeometry::combine(const VertexGeometry& vertex1, const VertexGeometry& vertex2) {
    return interpolate(vertex1, vertex2, 0.5);
}

void VertexGeometry::interpolate(const VertexGeometry& vertex, double t) {
    coords_ += (vertex.coords_ - coords_) * static_cast<tgt::vec3::ElemType>(t);
    texcoords_ += (vertex.texcoords_ - texcoords_) * static_cast<tgt::vec3::ElemType>(t);
    color_ += (vertex.color_ - color_) * static_cast<tgt::vec4::ElemType>(t);
    normal_ += (vertex.normal_ - normal_) * static_cast<tgt::vec4::ElemType>(t);

    setHasChanged(true);
}

VertexGeometry VertexGeometry::interpolate(const VertexGeometry& vertex1, const VertexGeometry& vertex2, double t) {
    VertexGeometry result = vertex1;
    result.interpolate(vertex2, t);
    return result;
}

void VertexGeometry::transform(const tgt::mat4& transformation) {
    coords_ = transformation * coords_;

    setHasChanged(true);
}

bool VertexGeometry::equals(const VertexGeometry& vertex, double epsilon) const {
    return std::abs(getDistance(vertex)) <= epsilon;
}

bool VertexGeometry::operator==(const VertexGeometry& vertex) const {
    return this->equals(vertex);
}

bool VertexGeometry::operator!=(const VertexGeometry& vertex) const {
    return !(*this == vertex);
}

void VertexGeometry::serialize(XmlSerializer& s) const {
    s.serialize("coords", coords_);
    s.serialize("texcoords", texcoords_);
    s.serialize("color", color_);
    if (normalIsSet_)
        s.serialize("normal", normal_);
}

void VertexGeometry::deserialize(XmlDeserializer& s) {
    s.deserialize("coords", coords_);
    s.deserialize("texcoords", texcoords_);
    s.deserialize("color", color_);
    try {
        s.deserialize("normal", normal_);
        normalIsSet_ = true;
    }
    catch (...) {
        normalIsSet_ = false;
        s.removeLastError();
    }
    setHasChanged(true);
}

} // namespace
