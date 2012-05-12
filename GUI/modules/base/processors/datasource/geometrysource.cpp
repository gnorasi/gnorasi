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

#include "geometrysource.h"

#include "voreen/core/voreenapplication.h"
#include "voreen/core/datastructures/geometry/meshlistgeometry.h"
#include "voreen/core/datastructures/geometry/pointlistgeometry.h"
#include "voreen/core/datastructures/geometry/pointsegmentlistgeometry.h"

#include "voreen/core/properties/callmemberaction.h"

#include <vector>

using tgt::vec3;
using tgt::ivec3;
using tgt::ivec2;
using std::vector;

namespace voreen {

const std::string GeometrySource::loggerCat_("voreen.base.GeometrySource");

GeometrySource::GeometrySource()
  : Processor(),
    geometryFile_("geometryFile", "Geometry file", "Open Geometry File", VoreenApplication::app()->getDataPath()),
    geometryType_("geometryType", "Geometry type"),
    skipItemCount_("skipItems", "Items to skip after each point", 0),
    outport_(Port::OUTPORT, "geometry.pointlist")
{
    geometryType_.addOption("pointlist", "Pointlist");
    geometryType_.addOption("segmentlist", "Segmented Pointlist");
    geometryType_.addOption("geometry", "Voreen geometry file (.vge)");

    geometryFile_.onChange(CallMemberAction<GeometrySource>(this, &GeometrySource::readGeometry));
    geometryType_.onChange(CallMemberAction<GeometrySource>(this, &GeometrySource::readGeometry));
    skipItemCount_.onChange(CallMemberAction<GeometrySource>(this, &GeometrySource::readGeometry));

    addProperty(geometryFile_);
    addProperty(geometryType_);
    addProperty(skipItemCount_);

    addPort(outport_);
}

GeometrySource::~GeometrySource() {
}

Processor* GeometrySource::create() const {
    return new GeometrySource();
}

void GeometrySource::process() {}

void GeometrySource::initialize() throw (tgt::Exception) {
    Processor::initialize();
    if (!geometryFile_.get().empty()) {
        readGeometry();
        if (!outport_.hasData())
            geometryFile_.set("");
    }
}

void GeometrySource::readGeometry() {
    if (geometryType_.get() == "pointlist" || geometryType_.get() == "segmentlist")
        readPointList();
    else {
        // read Voreen geometry serialization (.vge)
        LINFO("Reading geometry file " << geometryFile_.get());
        std::ifstream stream;
        stream.open(geometryFile_.get().c_str(), std::ios_base::in);
        if (stream.fail()) {
            stream.close();
            LERROR("failed to read file " << geometryFile_.get());
        }
        else {
            XmlDeserializer xmlDeserializer;
            try {
                xmlDeserializer.read(stream);
                Geometry* geometry = 0;
                geometry = readAbstractGeometry(xmlDeserializer);
                if (!geometry)
                    geometry = readMeshListGeometry(xmlDeserializer);
                if (!geometry)
                    geometry = readPointListGeometry(xmlDeserializer);
                if (!geometry)
                    geometry = readPointSegmentListGeometry(xmlDeserializer);

                stream.close();
                if (!geometry) {
                    LERROR("failed to deserialize " << geometryFile_.get());
                    outport_.setData(0);
                } 
                else {
                    outport_.setData(geometry);
                }
            }
            catch (SerializationException &e) {
                LWARNING(std::string("Deserialization failed: ") + e.what());
            }
        }
    }
}

void GeometrySource::readPointList() {

    if (!outport_.isInitialized())
        return;

    delete outport_.getData();
    outport_.setData(0);

    std::string filename = geometryFile_.get();
    if (filename.empty()) {
        return;
    }

    LINFO("Reading geometry file: " << filename);

    std::ifstream inFile;
    inFile.open(filename.c_str());
    if (inFile.fail()) {
        LERROR("Unable to open geometry file for reading: " << filename);
        return;
    }

    PointListGeometryVec3* pointListGeometry = 0;

    PointSegmentListGeometry<tgt::vec3>* pointSegmentListGeometry = 0;
    std::vector<std::vector<tgt::vec3> > segments;

    if (geometryType_.get() == "pointlist")
        pointListGeometry = new PointListGeometryVec3();
    else if (geometryType_.get() == "segmentlist")
        pointSegmentListGeometry = new PointSegmentListGeometry<tgt::vec3>();
    else {
        LWARNING("Unknown geometry type. Abort.");
        inFile.close();
        return;
    }

    int lastSegID = -1;
    while (true) {

        tgt::vec3 point;
        float dummy;
        int segID;

        if (!(inFile >> point.x))
            goto end;

        if (!(inFile >> point.y))
            goto end;

        if (!(inFile >> point.z))
            goto end;

        if (geometryType_.get() == "segmentlist") {
            if (!(inFile >> segID))
                goto end;
        }

        // skip items according to property
        for (int i=0; i<skipItemCount_.get(); ++i) {
            if (!(inFile >> dummy))
                goto end;
        }

        if (geometryType_.get() == "segmentlist") {
            // if new segment started, append vector for it
            if (segID > lastSegID) {
                segments.push_back(std::vector<tgt::vec3>());
                lastSegID = segID;
            }

            // append next skelPoint to last segment vector
            segments.back().push_back(point);
        }
        else if (geometryType_.get() == "pointlist") {
            pointListGeometry->addPoint(point);
        }
    }

end:

    if (geometryType_.get() == "segmentlist") {
        size_t numPoints = 0;
        for (size_t i=0; i<segments.size(); ++i)
            numPoints += segments[i].size();
        LINFO("Read " << segments.size() << " segments consisting of " << numPoints << " points.");
        pointSegmentListGeometry->setData(segments);
    }
    else if (geometryType_.get() == "pointlist") {
        LINFO("Read " << pointListGeometry->getNumPoints() << " points.");
    }

    if (pointListGeometry)
        outport_.setData(pointListGeometry, false);
    else if (pointSegmentListGeometry)
        outport_.setData(pointSegmentListGeometry, false);
    else {
        LERROR("No geometry object created");
    }

}

Geometry* GeometrySource::readAbstractGeometry(XmlDeserializer& deserializer) const {
    Geometry* geometry = 0;
    try {
        deserializer.deserialize("Geometry", geometry);
        return geometry;
    }
    catch (XmlSerializationNoSuchDataException) {
        // invalid geometry type
        deserializer.removeLastError();
        return 0;
    }
}

Geometry* GeometrySource::readMeshListGeometry(XmlDeserializer& deserializer) const {
    MeshListGeometry* geometry = new MeshListGeometry();
    try {
        geometry->deserialize(deserializer);
        return geometry;
    }
    catch (XmlSerializationNoSuchDataException) {
        // invalid geometry type
        deserializer.removeLastError();
        delete geometry;
        return 0;
    }
}

Geometry* GeometrySource::readPointListGeometry(XmlDeserializer& deserializer) const {
    PointListGeometry<tgt::vec3>* geometry = new PointListGeometry<tgt::vec3>();
    try {
        geometry->deserialize(deserializer);
        return geometry;
    }
    catch (XmlSerializationNoSuchDataException) {
        // invalid geometry type
        deserializer.removeLastError();
        delete geometry;
        return 0;
    }
}

Geometry* GeometrySource::readPointSegmentListGeometry(XmlDeserializer& deserializer) const {
    PointSegmentListGeometry<tgt::vec3>* geometry = new PointSegmentListGeometry<tgt::vec3>();
    try {
        geometry->deserialize(deserializer);
        return geometry;
    }
    catch (XmlSerializationNoSuchDataException) {
        // invalid geometry type
        deserializer.removeLastError();
        delete geometry;
        return 0;
    }
}

} // namespace voreen
