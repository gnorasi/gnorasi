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

#include "volumeinversion.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorinvert.h"

namespace voreen {

VolumeInversion::VolumeInversion()
    : VolumeProcessor()
    , inport_(Port::INPORT, "volumehandle.input")
    , outport_(Port::OUTPORT, "volumehandle.output", 0)
    , enableProcessing_("enableProcessing", "Enable")
    , forceUpdate_(true)
{
    addPort(inport_);
    addPort(outport_);

    enableProcessing_.onChange(CallMemberAction<VolumeInversion>(this, &VolumeInversion::forceUpdate));
    addProperty(enableProcessing_);
}

VolumeInversion::~VolumeInversion() {
}

Processor* VolumeInversion::create() const {
    return new VolumeInversion();
}

void VolumeInversion::process() {
    if (!enableProcessing_.get()) {
        outport_.setData(const_cast<VolumeHandleBase*>(inport_.getData()), false);
    }
    else if (forceUpdate_ || inport_.hasChanged()) {
        invertVolume();
    }
}

// private methods
//
void VolumeInversion::forceUpdate() {
    forceUpdate_ = true;
}

void VolumeInversion::invertVolume() {
    const VolumeHandleBase* handle = inport_.getData();
    tgtAssert(handle, "Inport has no data");

    forceUpdate_ = false;

    if (handle->getRepresentation<Volume>()) {
        //Volume* v = handle->getRepresentation<Volume>()->clone();
        VolumeHandle* v = VolumeOperatorInvert::APPLY_OP(handle);
        outport_.setData(v);
    }
    else {
        outport_.setData(0);
    }
}

}   // namespace
