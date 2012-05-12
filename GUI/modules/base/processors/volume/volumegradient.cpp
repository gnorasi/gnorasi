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

#include "volumegradient.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/gradient.h"

namespace voreen {

const std::string VolumeGradient::loggerCat_("voreen.VolumeGradient");

VolumeGradient::VolumeGradient()
    : VolumeProcessor(),
    technique_("technique", "Technique"),
    inport_(Port::INPORT, "volumehandle.input"),
    outport_(Port::OUTPORT, "volumehandle.output", 0)

{
    technique_.addOption("central-differences", "Central differences");
    technique_.addOption("sobel", "Sobel");
    technique_.addOption("linear-regression", "Linear regression");
    addProperty(technique_);

    addPort(inport_);
    addPort(outport_);
}

VolumeGradient::~VolumeGradient() {}

Processor* VolumeGradient::create() const {
    return new VolumeGradient();
}

void VolumeGradient::process() {

    const VolumeHandleBase* inputHandle = inport_.getData();
    const Volume* inputVolume = inputHandle->getRepresentation<Volume>();
    VolumeHandle* outputVolume = 0;

    // expecting a single-channel volume
    if (inputVolume->getNumChannels() == 1) {

        bool bit16 = inputVolume->getBitsAllocated() > 8;

        if (technique_.get() == "central-differences") {
            if (bit16)
                outputVolume = calcGradientsCentralDifferences<uint16_t>(inputHandle);
            else if (!bit16)
                outputVolume = calcGradientsCentralDifferences<uint8_t>(inputHandle);
            else {
                tgtAssert(false, "Should not get here");
            }
        }
        else if (technique_.get() == "sobel") {
            if (bit16)
                outputVolume = calcGradientsSobel<uint16_t>(inputHandle);
            else if (!bit16)
                outputVolume = calcGradientsSobel<uint8_t>(inputHandle);
            else {
                tgtAssert(false, "Should not get here");
            }
        }
        else if (technique_.get() == "linear-regression") {
            if (bit16)
                outputVolume = calcGradientsLinearRegression<uint16_t>(inputHandle);
            else if (!bit16)
                outputVolume = calcGradientsLinearRegression<uint8_t>(inputHandle);
            else {
                tgtAssert(false, "Should not get here");
            }
        }
        else {
            LERROR("Unknown technique");
        }
    }
    else {
        LWARNING("Intensity volume expected, but passed volume consists of " << inputVolume->getNumChannels() << " channels.");
    }

    outport_.setData(outputVolume);
}

}   // namespace
