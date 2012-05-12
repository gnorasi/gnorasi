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

#include "volumecrop.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/operators/volumeoperatorsubset.h"
#include "voreen/core/datastructures/volume/gradient.h"

namespace voreen {

using tgt::vec3;
using tgt::ivec3;
using tgt::mat4;

const std::string VolumeCrop::loggerCat_("voreen.base.VolumeCrop");

VolumeCrop::VolumeCrop()
    : CachingVolumeProcessor()
    , inport_(Port::INPORT, "volumehandle.input")
    , outport_(Port::OUTPORT, "volumehandle.output", 0)
    , clipRight_("rightClippingPlane", "Right Clip Plane (x)", 0, 0, 100000)
    , clipLeft_("leftClippingPlane", "Left Clip Plane (x)", 1, 1, 100000)
    , clipFront_("frontClippingPlane", "Front Clip Plane (y)", 0, 0, 100000)
    , clipBack_("backClippingPlane", "Back Clip Plane (y)", 1, 1, 100000)
    , clipBottom_("bottomClippingPlane", "Bottom Clip Plane (z)", 0, 0, 100000)
    , clipTop_("topClippingPlane", "Top Clip Plane (z)", 1, 1, 100000)
    , preserveLocation_("preserveLocation", "Preserve Location", true)
    , continuousCropping_("continuousCropping", "Continuous Cropping", false)
    , button_("button", "Crop")
    , croppedDimensions_("croppedDimensions", "Cropped Dimensions",
        tgt::ivec3(0), tgt::ivec3(0), tgt::ivec3(100000))
    , croppedSize_("croppedSize", "Cropped Size (MB)", 0, 0, 100000)
{
    addPort(inport_);
    addPort(outport_);

    clipRight_.onChange(CallMemberAction<VolumeCrop>(this, &VolumeCrop::onClipRightChange));
    clipLeft_.onChange(CallMemberAction<VolumeCrop>(this, &VolumeCrop::onClipLeftChange));
    clipFront_.onChange(CallMemberAction<VolumeCrop>(this, &VolumeCrop::onClipFrontChange));
    clipBack_.onChange(CallMemberAction<VolumeCrop>(this, &VolumeCrop::onClipBackChange));
    clipBottom_.onChange(CallMemberAction<VolumeCrop>(this, &VolumeCrop::onClipBottomChange));
    clipTop_.onChange(CallMemberAction<VolumeCrop>(this, &VolumeCrop::onClipTopChange));
    button_.onChange(CallMemberAction<VolumeCrop>(this, &VolumeCrop::crop));

    addProperty(clipRight_);
    addProperty(clipLeft_);
    addProperty(clipFront_);
    addProperty(clipBack_);
    addProperty(clipBottom_);
    addProperty(clipTop_);
    addProperty(preserveLocation_);
    addProperty(continuousCropping_);
    addProperty(button_);

    addProperty(croppedDimensions_);
    addProperty(croppedSize_);

    croppedDimensions_.setWidgetsEnabled(false);
    croppedSize_.setWidgetsEnabled(false);

    oldVolumeDimensions_ = tgt::ivec3(0,0,0);
    setExpensiveComputationStatus(COMPUTATION_STATUS_PROGRESSBAR);
}

Processor* VolumeCrop::create() const {
    return new VolumeCrop();
}

void VolumeCrop::process() {
    tgtAssert(inport_.getData()->getRepresentation<Volume>(), "no input volume");

    if (oldVolumeDimensions_ == tgt::ivec3(0,0,0))
        oldVolumeDimensions_ = inport_.getData()->getRepresentation<Volume>()->getDimensions();

    // adapt clipping plane properties on volume change
    if (inport_.hasChanged()) {
        adjustClipPropertiesRanges();
    }

    tgt::svec3 inputDimensions = inport_.getData()->getRepresentation<Volume>()->getDimensions();

    // compute size of cropped volume in each rendering pass
    tgt::svec3 dimensions;
    dimensions.x = clipLeft_.get() - clipRight_.get() + 1;
    dimensions.y = clipBack_.get() - clipFront_.get() + 1;
    dimensions.z = clipTop_.get() - clipBottom_.get() + 1;
    croppedDimensions_.setMaxValue(inputDimensions);
    croppedDimensions_.set(dimensions);

    size_t bpp = static_cast<size_t>(inport_.getData()->getRepresentation<Volume>()->getBytesPerVoxel());
    size_t inputRamSize = hmul(inputDimensions) * bpp / (1024 * 1024);
    size_t croppedRamSize = hmul(dimensions) * bpp / (1024 * 1024);
    croppedSize_.setMaxValue(static_cast<int>(inputRamSize));
    croppedSize_.set(static_cast<int>(croppedRamSize));

    // actually crop volume only, if continuous cropping is enabled (potentially slow)
    if (continuousCropping_.get())
        crop();
}

void VolumeCrop::crop() {
    if (!inport_.hasData())
        return;
    const VolumeHandleBase* inputHandle = inport_.getData();
    VolumeHandle* outputVolume = 0;

    tgt::ivec3 start, dimensions;
    start.x = clipRight_.get();
    start.y = clipFront_.get();
    start.z = clipBottom_.get();
    dimensions.x = clipLeft_.get() - clipRight_.get() + 1;
    dimensions.y = clipBack_.get() - clipFront_.get() + 1;
    dimensions.z = clipTop_.get() - clipBottom_.get() + 1;

    outputVolume = VolumeOperatorSubset::APPLY_OP(inport_.getData(), start, dimensions, progressBar_);
    outputVolume->setSpacing(inputHandle->getSpacing());

    // assign computed volume to outport
    if (outputVolume) {
        if(preserveLocation_.get()) {
            outputVolume->setPhysicalToWorldMatrix(inputHandle->getPhysicalToWorldMatrix());
        }
        else {
            oldVolumePosition(outputVolume);
            outputVolume->setPhysicalToWorldMatrix(tgt::mat4::identity);
        }
        outport_.setData(outputVolume);
    }
    else
        outport_.setData(0);
}

void VolumeCrop::onClipRightChange() {
    if (clipRight_.get() >= clipLeft_.get())
        clipLeft_.set(clipRight_.get() + 1);
}

void VolumeCrop::onClipLeftChange() {
    if (clipRight_.get() >= clipLeft_.get())
        clipRight_.set(clipLeft_.get() - 1);
}

void VolumeCrop::onClipFrontChange() {
    if (clipFront_.get() >= clipBack_.get())
        clipBack_.set(clipFront_.get() + 1);
}

void VolumeCrop::onClipBackChange() {
    if (clipFront_.get() >= clipBack_.get())
        clipFront_.set(clipBack_.get() - 1);
}

void VolumeCrop::onClipBottomChange() {
    if (clipBottom_.get() >= clipTop_.get())
        clipTop_.set(clipBottom_.get() + 1);
}

void VolumeCrop::onClipTopChange() {
    if (clipBottom_.get() >= clipTop_.get())
        clipBottom_.set(clipTop_.get() - 1);
}

void VolumeCrop::adjustClipPropertiesRanges() {
    tgtAssert(inport_.getData() && inport_.getData()->getRepresentation<Volume>(), "No input volume");
    tgt::ivec3 numSlices = inport_.getData()->getRepresentation<Volume>()->getDimensions();

    // adapt clipping plane properties to volume dimensions
    clipRight_.setMaxValue(numSlices.x-1);
    clipLeft_.setMaxValue(numSlices.x-1);

    clipFront_.setMaxValue(numSlices.y-1);
    clipBack_.setMaxValue(numSlices.y-1);

    clipBottom_.setMaxValue(numSlices.z-1);
    clipTop_.setMaxValue(numSlices.z-1);

    // assign new clipping values while taking care that the right>left validation
    // does not alter the assigned values
    float rightVal = clipRight_.get()/static_cast<float>(oldVolumeDimensions_.x-1) * (numSlices.x-1);
    float leftVal = clipLeft_.get()/static_cast<float>(oldVolumeDimensions_.x-1) * (numSlices.x-1);
    clipLeft_.set(clipLeft_.getMaxValue());
    clipRight_.set(tgt::ifloor(rightVal));
    clipLeft_.set(tgt::ifloor(leftVal));

    float frontVal = clipFront_.get()/static_cast<float>(oldVolumeDimensions_.y-1) * (numSlices.y-1);
    float backVal = clipBack_.get()/static_cast<float>(oldVolumeDimensions_.y-1) * (numSlices.y-1);
    clipBack_.set(clipBack_.getMaxValue());
    clipFront_.set(tgt::ifloor(frontVal));
    clipBack_.set(tgt::ifloor(backVal));

    float bottomVal = clipBottom_.get()/static_cast<float>(oldVolumeDimensions_.z-1) * (numSlices.z-1);
    float topVal = clipTop_.get()/static_cast<float>(oldVolumeDimensions_.z-1) * (numSlices.z-1);
    clipTop_.set(clipTop_.getMaxValue());
    clipBottom_.set(tgt::ifloor(bottomVal));
    clipTop_.set(tgt::ifloor(topVal));

    if (clipRight_.get() > clipRight_.getMaxValue())
        clipRight_.set(clipRight_.getMaxValue());

    if (clipLeft_.get() > clipLeft_.getMaxValue())
        clipLeft_.set(clipLeft_.getMaxValue());

    if (clipFront_.get() > clipFront_.getMaxValue())
        clipFront_.set(clipFront_.getMaxValue());

    if (clipBack_.get() > clipBack_.getMaxValue())
        clipBack_.set(clipBack_.getMaxValue());

    if (clipBottom_.get() > clipBottom_.getMaxValue())
        clipBottom_.set(clipBottom_.getMaxValue());

    if (clipTop_.get() > clipTop_.getMaxValue())
        clipTop_.set(clipTop_.getMaxValue());

    oldVolumeDimensions_ = numSlices;
}

}   // namespace
