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

#include "voreen/core/datastructures/geometry/geometryfactory.h"

#include "voreen/core/datastructures/geometry/vertexgeometry.h"
#include "voreen/core/datastructures/geometry/facegeometry.h"
#include "voreen/core/datastructures/geometry/meshgeometry.h"
#include "voreen/core/datastructures/geometry/meshlistgeometry.h"
#include "voreen/core/datastructures/geometry/pointlistgeometry.h"
#include "voreen/core/datastructures/geometry/pointsegmentlistgeometry.h"

namespace voreen {

const std::string GeometryFactory::getTypeString(const std::type_info& type) const {
    if (type == typeid(VertexGeometry))
        return "VertexGeometry";
    else if (type == typeid(FaceGeometry))
        return "FaceGeometry";
    else if (type == typeid(MeshGeometry))
        return "MeshGeometry";
    else if (type == typeid(MeshListGeometry))
        return "MeshListGeometry";
    else if (type == typeid(PointListGeometryVec3))
        return "PointListGeometryVec3";
    else if (type == typeid(PointSegmentListGeometryVec3))
        return "PointSegmentListGeometryVec3";
    else
        return "";
}

Serializable* GeometryFactory::createType(const std::string& typeString) {
    if (typeString == "VertexGeometry")
        return new VertexGeometry();
    else if (typeString == "FaceGeometry")
        return new FaceGeometry();
    else if (typeString == "MeshGeometry")
        return new MeshGeometry();
    else if (typeString == "MeshListGeometry")
        return new MeshListGeometry();
    else if (typeString == "PointListGeometryVec3")
        return new PointListGeometryVec3();
    else if (typeString == "PointSegmentListGeometryVec3")
        return new PointSegmentListGeometryVec3();
    else
        return 0;
}

} // namespace voreen
