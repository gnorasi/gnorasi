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

#include "volumetransformation.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/volumehandledecorator.h"
#include "voreen/core/io/serialization/meta/primitivemetadata.h"

namespace voreen {

const std::string VolumeTransformation::loggerCat_("voreen.VolumeTransformation");

VolumeTransformation::VolumeTransformation()
    : VolumeProcessor(),
    inport_(Port::INPORT, "volumehandle.input"),
    outport_(Port::OUTPORT, "volumehandle.output", 0),
    voxel2Cube_("voxel2Cube", "Voxel- to cube-matrix transformation", false),
    transformMatrix_("transformMatrix", "Transformation Matrix", tgt::mat4::identity, tgt::mat4(-200.0), tgt::mat4(200.0))
{
    addPort(inport_);
    addPort(outport_);

    addProperty(transformMatrix_);
    addProperty(voxel2Cube_);
}

VolumeTransformation::~VolumeTransformation() {}

Processor* VolumeTransformation::create() const {
    return new VolumeTransformation();
}

void VolumeTransformation::process() {

    const VolumeHandleBase* inputVolume = inport_.getData();

    tgt::mat4 cubeToVoxel = tgt::mat4::identity;
    if (voxel2Cube_.get()) {
        cubeToVoxel = inputVolume->getPhysicalToVoxelMatrix();
    }
    tgt::mat4 m = transformMatrix_.get() * inputVolume->getPhysicalToWorldMatrix() * cubeToVoxel;

    VolumeHandleBase* outputVolume = new VolumeHandleDecoratorReplace(inport_.getData(), "Transformation", new Mat4MetaData(m));

    outport_.setData(outputVolume);
}

}   // namespace
