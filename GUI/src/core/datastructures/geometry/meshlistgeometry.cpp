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

#include "voreen/core/datastructures/geometry/meshlistgeometry.h"
#include "voreen/core/io/serialization/xmlserializer.h"
#include "voreen/core/io/serialization/xmldeserializer.h"

namespace voreen {

MeshListGeometry::MeshListGeometry()
    : Geometry()
{
}

size_t MeshListGeometry::getMeshCount() const {
    return meshes_.size();
}

size_t MeshListGeometry::getFaceCount() const {
    size_t result = 0;
    for (const_iterator it = begin(); it != end(); ++it)
        result += it->getFaceCount();
    return result;
}

size_t MeshListGeometry::getVertexCount() const {
    size_t result = 0;
    for (const_iterator it = begin(); it != end(); ++it)
        result += it->getVertexCount();
    return result;
}

bool MeshListGeometry::empty() const {
    return (getVertexCount() == 0);
}

void MeshListGeometry::addMesh(const MeshGeometry& mesh) {
    meshes_.push_back(mesh);
}

const MeshGeometry& MeshListGeometry::getMesh(size_t index) const {
    tgtAssert(index < meshes_.size(), "Invalid index");
    return meshes_.at(index);
}

MeshGeometry& MeshListGeometry::getMesh(size_t index) {
    tgtAssert(index < meshes_.size(), "Invalid index");
    return meshes_.at(index);
}

void MeshListGeometry::clear() {
    meshes_.clear();
}

MeshListGeometry::iterator MeshListGeometry::begin() {
    return meshes_.begin();
}

MeshListGeometry::const_iterator MeshListGeometry::begin() const {
    return meshes_.begin();
}

MeshListGeometry::iterator MeshListGeometry::end() {
    return meshes_.end();
}

MeshListGeometry::const_iterator MeshListGeometry::end() const {
    return meshes_.end();
}

MeshGeometry& MeshListGeometry::operator[](size_t index) {
    tgtAssert(index < meshes_.size(), "Invalid index");
    return meshes_[index];
}

const MeshGeometry& MeshListGeometry::operator[](size_t index) const {
    tgtAssert(index < meshes_.size(), "Invalid index");
    return meshes_[index];
}

void MeshListGeometry::getBoundingBox(tgt::vec3& llf, tgt::vec3& urb) const {
    llf = tgt::vec3(FLT_MAX);
    urb = tgt::vec3(-FLT_MAX);
    for (size_t i = 0; i < getMeshCount(); ++i) {
        const MeshGeometry& mesh = getMesh(i);
        for (size_t j = 0; j < mesh.getFaceCount(); ++j) {
            const FaceGeometry& face = mesh.getFace(j);
            for (size_t k = 0; k < face.getVertexCount(); ++k) {
                const VertexGeometry& vertex = face.getVertex(k);
                const tgt::vec3& coords = vertex.getCoords();

                llf.x = std::min(llf.x, coords.x);
                urb.x = std::max(urb.x, coords.x);
                llf.y = std::min(llf.y, coords.y);
                urb.y = std::max(urb.y, coords.y);
                llf.z = std::min(llf.z, coords.z);
                urb.z = std::max(urb.z, coords.z);
            }
        }
    }
}

void MeshListGeometry::render() const {
    for (const_iterator it = begin(); it != end(); ++it)
        it->render();
}

void MeshListGeometry::transform(const tgt::mat4& transformation) {
    for (iterator it = begin(); it != end(); ++it)
        it->transform(transformation);
}

MeshListGeometry MeshListGeometry::clip(const tgt::vec4& clipplane, double epsilon) {
    MeshListGeometry closingFaces;
    for (iterator it = begin(); it != end(); ++it) {
        MeshGeometry closingFace = it->clip(clipplane, epsilon);
        if (!closingFace.empty())
            closingFaces.addMesh(closingFace);
    }
    return closingFaces;
}

void MeshListGeometry::serialize(XmlSerializer& s) const {
    s.serialize("meshes", meshes_);
}

void MeshListGeometry::deserialize(XmlDeserializer& s) {
    s.deserialize("meshes", meshes_);
    setHasChanged(true);
}

} // namespace
