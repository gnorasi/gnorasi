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

#include "coordinatetransformation.h"

#include "voreen/core/voreenapplication.h"
#include "voreen/core/datastructures/geometry/pointlistgeometry.h"
#include "voreen/core/datastructures/geometry/pointsegmentlistgeometry.h"
#include "voreen/core/properties/callmemberaction.h"

#include <vector>

using tgt::vec3;
using tgt::ivec3;
using tgt::ivec2;
using std::vector;

namespace voreen {

const std::string CoordinateTransformation::loggerCat_("voreen.CoordinateTransformation");

CoordinateTransformation::CoordinateTransformation()
  : Processor(),
    sourceCoordinateSystem_("sourceCoordinateSystem", "Source coordinate system"),
    targetCoordinateSystem_("targetCoordinateSystem", "Dest coordinate system"),
    geometryInport_(Port::INPORT, "geometry.inport"),
    volumeInport_(Port::INPORT, "volume.inport"),
    geometryOutport_(Port::OUTPORT, "geometry.outport"),
    forceUpdate_(true)
{

    std::vector<std::string> sourceCoordinates;
    sourceCoordinateSystem_.addOption("voxel-coordinates", "Voxel Coordinates");
    sourceCoordinateSystem_.addOption("world-coordinates", "world Coordinates");
    /*sourceCoordinateSystem_.push_back("volume-coordinates", "Volume Coordinates");
    sourceCoordinateSystem_.push_back("texture-coordinates", "Texture Coordinates");*/
    sourceCoordinateSystem_.onChange(CallMemberAction<CoordinateTransformation>(this, &CoordinateTransformation::forceUpdate));
    addProperty(sourceCoordinateSystem_);

    targetCoordinateSystem_.addOption("voxel-coordinates", "Voxel Coordinates");
    targetCoordinateSystem_.addOption("volume-coordinates", "Volume Coordinates");
    targetCoordinateSystem_.addOption("world-coordinates", "World Coordinates");
    targetCoordinateSystem_.addOption("texture-coordinates", "Texture Coordinates");
    targetCoordinateSystem_.select("world-coordinates");
    targetCoordinateSystem_.onChange(CallMemberAction<CoordinateTransformation>(this, &CoordinateTransformation::forceUpdate));
    addProperty(targetCoordinateSystem_);

    addPort(geometryInport_);
    addPort(volumeInport_);
    addPort(geometryOutport_);
}

CoordinateTransformation::~CoordinateTransformation() {
}

Processor* CoordinateTransformation::create() const {
    return new CoordinateTransformation();
}

void CoordinateTransformation::process() {

    // do nothing, if neither input data nor coordinate system selection has changed
    if (!geometryInport_.hasChanged() && !volumeInport_.hasChanged() && !forceUpdate_)
        return;

    // clear output
    geometryOutport_.setData(0);
    forceUpdate_ = false;

    // return if no input data present
    if (!geometryInport_.hasData() || !volumeInport_.hasData())
        return;

    // retrieve and check geometry from inport
    const PointListGeometryVec3* geometrySrc = dynamic_cast<const PointListGeometryVec3*>(geometryInport_.getData());
    const PointSegmentListGeometryVec3* geometrySegmentSrc = dynamic_cast<const PointSegmentListGeometryVec3*>(geometryInport_.getData());
    if (!geometrySrc && !geometrySegmentSrc) {
        LWARNING("Geometry of type TGTvec3PointListGeometry or TGTvec3PointSegmentListGeometry expected");
        return;
    }

    // retrieve volume handle from inport
    const VolumeHandleBase* volumeHandle = volumeInport_.getData();
    tgtAssert(volumeHandle, "No volume handle");
    tgtAssert(volumeHandle->getRepresentation<Volume>(), "No volume");

    // create output geometry object
    PointListGeometryVec3* geometryConv = 0;
    PointSegmentListGeometryVec3* geometrySegmentConv = 0;
    if (geometrySrc)
        geometryConv = new PointListGeometryVec3();
    else if (geometrySegmentSrc)
        geometrySegmentConv = new PointSegmentListGeometryVec3();

    //
    // convert points
    //

    if (targetCoordinateSystem_.isSelected("voxel-coordinates")) {
        if (sourceCoordinateSystem_.isSelected("voxel-coordinates")) {
            // voxel to voxel coordinates (no transformation necessary)
            // pointlist
            if (geometrySrc) {
                std::vector<tgt::vec3> geomPointsConv = std::vector<tgt::vec3>(geometrySrc->getData());
                geometryConv->setData(geomPointsConv);
            }
            // segmentlist
            else if (geometrySegmentSrc) {
                std::vector< std::vector<tgt::vec3> >geomPointsConv = std::vector< std::vector<tgt::vec3> >(geometrySegmentSrc->getData());
                geometrySegmentConv->setData(geomPointsConv);
            }
        }
        else if (sourceCoordinateSystem_.isSelected("world-coordinates")) {
            tgt::mat4 voxelToWorldTrafo = volumeHandle->getVoxelToWorldMatrix();
            tgt::mat4 worldToVoxelTrafo;
            if(!voxelToWorldTrafo.invert(worldToVoxelTrafo)) {
                LERROR("Failed to invert voxelToWorld matrix!");
                return;
            }
            else {
                // pointlist
                if (geometrySrc) {
                    std::vector<tgt::vec3> geomPointsConv = std::vector<tgt::vec3>(geometrySrc->getData());
                    for (size_t i=0; i<geomPointsConv.size(); i++)
                        geomPointsConv[i] = worldToVoxelTrafo * geomPointsConv[i];
                    geometryConv->setData(geomPointsConv);
                }
                // segmentlist
                else if (geometrySegmentSrc) {
                    std::vector< std::vector<tgt::vec3> >geomPointsConv = std::vector< std::vector<tgt::vec3> >(geometrySegmentSrc->getData());
                    for (size_t i=0; i<geomPointsConv.size(); ++i) {
                        for (size_t j=0; j<geomPointsConv[i].size(); ++j) {
                            geomPointsConv[i][j] = worldToVoxelTrafo * geomPointsConv[i][j];
                        }
                        geometrySegmentConv->addSegment(geomPointsConv[i]);
                    }
                }
            }
        }
    }
    // volume coordinates
    else if (targetCoordinateSystem_.isSelected("volume-coordinates")) {
        tgt::vec3 dims(volumeHandle->getDimensions());
        if (sourceCoordinateSystem_.isSelected("voxel-coordinates")) {
            tgt::mat4 m = volumeHandle->getVoxelToPhysicalMatrix();
            // pointlist
            if (geometrySrc) {
                std::vector<tgt::vec3> geomPointsConv = std::vector<tgt::vec3>(geometrySrc->getData());
                for (size_t i=0; i<geomPointsConv.size(); i++)
                    geomPointsConv[i] = m * geomPointsConv[i];
                geometryConv->setData(geomPointsConv);
            }
            // segmentlist
            else if (geometrySegmentSrc) {
                std::vector< std::vector<tgt::vec3> >geomPointsConv = std::vector< std::vector<tgt::vec3> >(geometrySegmentSrc->getData());
                for (size_t i=0; i<geomPointsConv.size(); ++i) {
                    for (size_t j=0; j<geomPointsConv[i].size(); ++j) {
                        geomPointsConv[i][j] = m * geomPointsConv[i][j];
                    }
                    geometrySegmentConv->addSegment(geomPointsConv[i]);
                }
            }
        }
        else if (sourceCoordinateSystem_.isSelected("world-coordinates")) {
            LERROR("Conversion not implemented!");
        }
    }
    // world coordinates
    else if (targetCoordinateSystem_.isSelected("world-coordinates")) {
        if (sourceCoordinateSystem_.isSelected("voxel-coordinates")) {
            tgt::mat4 voxelToWorldTrafo = volumeHandle->getVoxelToWorldMatrix();
            // pointlist
            if (geometrySrc) {
                std::vector<tgt::vec3> geomPointsConv = std::vector<tgt::vec3>(geometrySrc->getData());
                for (size_t i=0; i<geomPointsConv.size(); i++)
                    geomPointsConv[i] = voxelToWorldTrafo * geomPointsConv[i];
                geometryConv->setData(geomPointsConv);
            }
            // segmentlist
            else if (geometrySegmentSrc) {
                std::vector< std::vector<tgt::vec3> >geomPointsConv = std::vector< std::vector<tgt::vec3> >(geometrySegmentSrc->getData());
                for (size_t i=0; i<geomPointsConv.size(); ++i) {
                    for (size_t j=0; j<geomPointsConv[i].size(); ++j) {
                        geomPointsConv[i][j] = voxelToWorldTrafo * geomPointsConv[i][j];
                    }
                    geometrySegmentConv->addSegment(geomPointsConv[i]);
                }
            }
        }
        else if (sourceCoordinateSystem_.isSelected("world-coordinates")) {
            // world to world coordinates (no transformation necessary)
            // pointlist
            if (geometrySrc) {
                std::vector<tgt::vec3> geomPointsConv = std::vector<tgt::vec3>(geometrySrc->getData());
                geometryConv->setData(geomPointsConv);
            }
            // segmentlist
            else if (geometrySegmentSrc) {
                std::vector< std::vector<tgt::vec3> >geomPointsConv = std::vector< std::vector<tgt::vec3> >(geometrySegmentSrc->getData());
                geometrySegmentConv->setData(geomPointsConv);
            }
        }
    }
    // texture coordinates: [0:1.0]^3
    else if (targetCoordinateSystem_.isSelected("texture-coordinates")) {
        tgt::vec3 dims(volumeHandle->getRepresentation<Volume>()->getDimensions());
        if (sourceCoordinateSystem_.isSelected("voxel-coordinates")) {
            // pointlist
            if (geometrySrc) {
                std::vector<tgt::vec3> geomPointsConv = std::vector<tgt::vec3>(geometrySrc->getData());
                for (size_t i=0; i<geomPointsConv.size(); i++)
                    geomPointsConv[i] /= dims;
                geometryConv->setData(geomPointsConv);
            }
            // segmentlist
            else if (geometrySegmentSrc) {
                std::vector< std::vector<tgt::vec3> >geomPointsConv = std::vector< std::vector<tgt::vec3> >(geometrySegmentSrc->getData());
                for (size_t i=0; i<geomPointsConv.size(); ++i) {
                    for (size_t j=0; j<geomPointsConv[i].size(); ++j) {
                        geomPointsConv[i][j] /= dims;
                    }
                    geometrySegmentConv->addSegment(geomPointsConv[i]);
                }
            }
        }
        else if (sourceCoordinateSystem_.isSelected("world-coordinates")) {
            LERROR("Conversion not implemented!");
        }
    }

    // assign result to outport
    if (geometrySrc)
        geometryOutport_.setData(geometryConv);
    else if (geometrySegmentSrc)
        geometryOutport_.setData(geometrySegmentConv);
    else {
        LWARNING("No geometry object created");
    }
}

void CoordinateTransformation::forceUpdate() {
    forceUpdate_ = true;
}


} // namespace voreen
