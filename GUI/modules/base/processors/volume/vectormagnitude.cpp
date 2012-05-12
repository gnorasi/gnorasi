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

#include "vectormagnitude.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/gradient.h"

namespace voreen {

const std::string VectorMagnitude::loggerCat_("voreen.VectorMagnitude");

VectorMagnitude::VectorMagnitude()
    : VolumeProcessor(),
    inport_(Port::INPORT, "volumehandle.input"),
    outport_(Port::OUTPORT, "volumehandle.output", 0)
{
    addPort(inport_);
    addPort(outport_);
}

VectorMagnitude::~VectorMagnitude() {
}

Processor* VectorMagnitude::create() const {
    return new VectorMagnitude();
}

void VectorMagnitude::process() {
    const VolumeHandleBase* inputHandle = inport_.getData();
    const Volume* inputVolume = inport_.getData()->getRepresentation<Volume>();
    tgtAssert(inputVolume, "no input volume");
    VolumeHandle* outputVolume = 0;

    tgtAssert(inputVolume->getNumChannels() > 0, "invalid channel count");
    if (inputVolume->getNumChannels() == 3 || inputVolume->getNumChannels() == 4) {
        if (inputVolume->getBitsAllocated() / inputVolume->getNumChannels() > 8)
            outputVolume = calcGradientMagnitudes<uint16_t>(inputHandle);
        else
            outputVolume = calcGradientMagnitudes<uint8_t>(inputHandle);
    }
    else {
        LWARNING("Input volume of format RGB or RGBA expected.");
    }

    // assign computed volume to outport
    if (outputVolume)
        outport_.setData(outputVolume);
    else
        outport_.setData(0);
}

void VectorMagnitude::deinitialize() throw (tgt::Exception) {
    outport_.setData(0);

    VolumeProcessor::deinitialize();
}

}   // namespace
