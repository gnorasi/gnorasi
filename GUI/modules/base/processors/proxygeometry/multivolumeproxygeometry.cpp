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

#include "multivolumeproxygeometry.h"

namespace voreen {

MultiVolumeProxyGeometry::MultiVolumeProxyGeometry()
    : Processor()
    , inport_(Port::INPORT, "volumehandle.volumehandle", true)
    , outport_(Port::OUTPORT, "proxygeometry.geometry")
{
    addPort(inport_);
    addPort(outport_);
}

MultiVolumeProxyGeometry::~MultiVolumeProxyGeometry() {
}

Processor* MultiVolumeProxyGeometry::create() const {
    return new MultiVolumeProxyGeometry();
}

void MultiVolumeProxyGeometry::process() {
    tgtAssert(inport_.getData()->getRepresentation<Volume>(), "no volume");

    MeshListGeometry* geometry = new MeshListGeometry();

    std::vector<const VolumeHandleBase*> data = inport_.getAllData();
    for(size_t d=0; d<data.size(); ++d) {
        if(!data[d])
            continue;

        const VolumeHandleBase* volume = data[d];

        tgt::vec3 coordLlf = volume->getLLF();
        tgt::vec3 coordUrb = volume->getURB();


        MeshGeometry mesh = MeshGeometry::createCube(coordLlf, coordUrb, coordLlf, coordUrb);
        //apply dataset transformation matrix:
        mesh.transform(volume->getPhysicalToWorldMatrix());

        //reset tex coords to coords after transformation:
        for(size_t j=0; j<mesh.getFaceCount(); ++j) {
            FaceGeometry& fg = mesh.getFace(j);
            for(size_t k=0; k<fg.getVertexCount(); ++k) {
                VertexGeometry& vg = fg.getVertex(k);
                vg.setTexCoords(vg.getCoords());
            }
        }
        geometry->addMesh(mesh);
    }

    outport_.setData(geometry);
}

} // namespace
