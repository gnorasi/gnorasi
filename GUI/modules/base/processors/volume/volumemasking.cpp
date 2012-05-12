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

#include "volumemasking.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/volumeoperator.h"
#include "voreen/core/datastructures/transfunc/transfunc.h"

namespace voreen {

VolumeMasking::VolumeMasking()
    : CachingVolumeProcessor()
    , inport_(Port::INPORT, "volumehandle.input")
    , outport_(Port::OUTPORT, "volumehandle.output", 0)
    , enableProcessingProp_("enableProcessing", "Enable")
    , maskFunction_("maskFunction", "Masking function")
    , passedVoxelAction_("passedVoxelAction", "Passed voxels")
    , continousUpdate_("continousUpdates", "Update continously", true, Processor::VALID)
    , updateButton_("update", "Update")
    , forceUpdate_(true)
{
    addPort(inport_);
    addPort(outport_);

    passedVoxelAction_.addOption("maxIntensity", "Assign Max Intensity");
    passedVoxelAction_.addOption("passThrough", "Pass Through");
    passedVoxelAction_.addOption("alpha", "Assign Alpha Value");

    enableProcessingProp_.onChange(CallMemberAction<VolumeMasking>(this, &VolumeMasking::forceUpdate));
    maskFunction_.onChange(CallMemberAction<VolumeMasking>(this, &VolumeMasking::forceUpdate));
    passedVoxelAction_.onChange(CallMemberAction<VolumeMasking>(this, &VolumeMasking::forceUpdate));
    updateButton_.onChange(CallMemberAction<VolumeMasking>(this, &VolumeMasking::maskVolume));

    addProperty(enableProcessingProp_);
    addProperty(maskFunction_);
    addProperty(passedVoxelAction_);
    addProperty(continousUpdate_);
    addProperty(updateButton_);
}

VolumeMasking::~VolumeMasking() {}

Processor* VolumeMasking::create() const {
    return new VolumeMasking();
}

void VolumeMasking::beforeProcess() {
    maskFunction_.setVolumeHandle(inport_.getData());
    LGL_ERROR;
}

void VolumeMasking::process() {
    LGL_ERROR;

    if (!enableProcessingProp_.get()) {
        outport_.setData(const_cast<VolumeHandleBase*>(inport_.getData()), false);
    }
    else if (forceUpdate_ || inport_.hasChanged()) {
        if(continousUpdate_.get())
            maskVolume();
    }
}

// private methods
//

void VolumeMasking::forceUpdate() {
    forceUpdate_ = true;
}

void VolumeMasking::maskVolume() {
    tgtAssert(inport_.hasData(), "Inport has not data");

    forceUpdate_ = false;

    if (inport_.getData()->getRepresentation<Volume>()) {
        Volume* v = inport_.getData()->getRepresentation<Volume>()->clone();
        tgt::Texture* maskTexture = maskFunction_.get()->getTexture();
        maskTexture->downloadTexture();
        const int maskTexDim = maskTexture->getDimensions().x;

        // apply masking
        if (passedVoxelAction_.isSelected("maxIntensity")) {
            for (size_t i=0; i<v->getNumVoxels(); i++) {
                float intensity = v->getVoxelFloat(i);
                int alpha = maskTexture->texel< tgt::Vector4<uint8_t> >(static_cast<size_t>(intensity*(maskTexDim-1))).a;
                v->setVoxelFloat(alpha == 0 ? 0.f : 1.f, i);
                //progressBar_->setProgress(static_cast<float>(i) / static_cast<float>(v->getNumVoxels()));
            }
        }
        else if (passedVoxelAction_.isSelected("passThrough")) {
            for (size_t i=0; i<v->getNumVoxels(); i++) {
                float intensity = v->getVoxelFloat(i);
                int alpha = maskTexture->texel< tgt::Vector4<uint8_t> >(static_cast<size_t>(intensity*(maskTexDim-1))).a;
                if (alpha == 0)
                    v->setVoxelFloat(0.f, i);
            }
        }
        else if (passedVoxelAction_.isSelected("alpha")) {
            for (size_t i=0; i<v->getNumVoxels(); i++) {
                float intensity = v->getVoxelFloat(i);
                int alpha = maskTexture->texel< tgt::Vector4<uint8_t> >(static_cast<size_t>(intensity*(maskTexDim-1))).a;
                v->setVoxelFloat((float)alpha / 255.0f, i);
            }
        }

        outport_.setData(new VolumeHandle(v, inport_.getData()), true);
    }
    else {
        outport_.setData(0);
    }
}

}   // namespace
