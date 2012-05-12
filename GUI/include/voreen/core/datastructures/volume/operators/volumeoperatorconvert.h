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

#ifndef VRN_VOLUMEOPERATORCONVERT_H
#define VRN_VOLUMEOPERATORCONVERT_H

#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/datastructures/volume/volumeoperator.h"
#include "voreen/core/io/progressbar.h"
#include "voreen/core/utils/exception.h"
#include "tgt/vector.h"


namespace voreen {

/**
 * Converts the source volume's data to the destination volume's data type
 * and writes the result to the dest volume.
 *
 * The source volume has to be passed to the constructor,
 * the dest volume has to be passed to apply().
 *
 * @note The dest volume has to be instantiated by the caller
 *       and must match the source volume in dimension and channel count.
 */
class VolumeOperatorConvert  {
public:
    /**
     * @param srcVolume The volume to be converted. Must not be null.
     */
    VolumeOperatorConvert() :
        progressBar_(0),
        inputIntensityRange_(-1.0)
    {}

    /**
     * Sets the intensity range to be used for normalizing the
     * input volume's intensity values during the conversion.
     * If no range is set, the input volume's min/max intensities
     * are used.
     *
     * @param range the normalization range that is mapped to the
     *  data range of the output volume. Precondition: range.x < range.y
     *
     * @note This setting does only affect the conversion of unbounded
     *  data types (float, double).
     */
    void setInputIntensityRange(const tgt::dvec2& range) {
        if (range.x >= range.y)
            throw new VoreenException("Illegal range parameter. Expecting range.x < range.y");
        inputIntensityRange_ = range;
    }

    /**
     * Performs the conversion.
     *
     * Returns a VolumeHandle with a VolumeAtomic<T>
     */
    template<class T>
    VolumeHandle* apply(const VolumeHandleBase* srcVolume) const;

    /**
     * Assigns a progress bar that should be used by the
     * operator for indicating progress.
     */
    void setProgressBar(ProgressBar* progress) {
        progressBar_ = progress;
    }

protected:
    ProgressBar* progressBar_;  ///< to be used by concrete subclasses for indicating progress
private:
    tgt::dvec2 inputIntensityRange_;
};

template<class T>
VolumeHandle* VolumeOperatorConvert::apply(const VolumeHandleBase* srcVolumeHandle) const {
    const Volume* srcVolume = srcVolumeHandle->getRepresentation<Volume>();
    VolumeAtomic<T>* destVolume = new VolumeAtomic<T>(srcVolume->getDimensions());

    if (!srcVolume)
        throw VoreenException("VolumeOperatorConvert: source volume is null pointer");
    if (!destVolume)
        throw VoreenException("VolumeOperatorConvert: dest volume is null pointer");

    if (destVolume->getDimensions() != srcVolume->getDimensions())
        throw VoreenException("VolumeOperatorConvert: volume dimensions must match");
    if (destVolume->getNumChannels() != srcVolume->getNumChannels())
        throw VoreenException("VolumeOperatorConvert: number of channels must match");

    // check the source volume's type
    const VolumeUInt8* src8 = dynamic_cast<const VolumeUInt8*>(srcVolume);
    const VolumeUInt16* src16 = dynamic_cast<const VolumeUInt16*>(srcVolume);
    const VolumeFloat* srcFloat = dynamic_cast<const VolumeFloat*>(srcVolume);
    const VolumeDouble* srcDouble = dynamic_cast<const VolumeDouble*>(srcVolume);

    // check the dest volume's type
    VolumeUInt8* dest8 = dynamic_cast<VolumeUInt8*>(destVolume);
    VolumeUInt16* dest16 = dynamic_cast<VolumeUInt16*>(destVolume);

    if (src8 && dest8) {
        LINFOC("voreen.VolumeOperatorConvert" ,"No conversion necessary: source and dest type equal (VolumeUInt8)");
        VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::svec3(0, 0, 0), src8->getDimensions(), progressBar_)
            dest8->voxel(i) = src8->voxel(i);
    }
    else if (src16 && dest16) {
        LINFOC("voreen.VolumeOperatorConvert" ,"No conversion necessary: source and dest type equal (VolumeUInt16)");
        VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::svec3(0, 0, 0), src16->getDimensions(), progressBar_)
            dest16->voxel(i) = src16->voxel(i);
    }
    else if (src16 && dest8) {
        LINFOC("voreen.VolumeOperatorConvert", "Using accelerated conversion from VolumeUInt16 -> VolumeUInt8");
        // because the number of shifting bits varies by the number of bits used it must be calculated
        int shift = src16->getBitsStored() - dest8->getBitsStored();
        VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::svec3(0, 0, 0), src16->getDimensions(), progressBar_)
            dest8->voxel(i) = src16->voxel(i) >> shift;
    }
    else if (src8 && dest16) {
        LINFOC("voreen.VolumeOperatorConvert", "Using accelerated conversion from VolumeUInt8 -> VolumeUInt16");
        // because the number of shifting bits varies by the number of bits used it must be calculated
        int shift = dest16->getBitsStored() - src8->getBitsStored();
        VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::svec3(0, 0, 0), src8->getDimensions(), progressBar_)
            dest16->voxel(i) = src8->voxel(i) << shift;
    }
    else if (srcFloat) {
        float min, max;
        if (inputIntensityRange_ == tgt::dvec2(-1.0)) {
            // conversion range not set => use input volume's intensity range
            min = srcFloat->min();
            max = srcFloat->max();
        }
        else {
            // use assigned conversion range
            tgtAssert(inputIntensityRange_.x < inputIntensityRange_.y, "invalid intensity range");
            min = static_cast<float>(inputIntensityRange_.x);
            max = static_cast<float>(inputIntensityRange_.y);
        }
        float spread = max - min;

        LINFOC("voreen.VolumeOperatorConvert", "Converting float volume with data range [" << min << "; " << max << "] to "
            << destVolume->getBitsAllocated() << " bit integer (normalized).");

        VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::svec3(0, 0, 0), srcFloat->getDimensions(), progressBar_)
            destVolume->setVoxelFloat((srcFloat->voxel(i) - min) / spread, i);
    }
    else if (srcDouble) {
        double min, max;
        if (inputIntensityRange_ == tgt::dvec2(-1.0)) {
            // conversion range not set => use input volume's intensity range
            min = srcDouble->min();
            max = srcDouble->max();
        }
        else {
            // use assigned conversion range
            tgtAssert(inputIntensityRange_.x < inputIntensityRange_.y, "invalid intensity range");
            min = inputIntensityRange_.x;
            max = inputIntensityRange_.y;
        }
        double spread = max - min;

        LINFOC("voreen.VolumeOperatorConvert", "Converting double volume with data range [" << min << "; " << max << "] to "
            << destVolume->getBitsAllocated() << " bit integer (normalized).");

        VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::svec3(0, 0, 0), srcDouble->getDimensions(), progressBar_)
            destVolume->setVoxelFloat(static_cast<float>((srcDouble->voxel(i) - min) / spread), i);
    }
    else {
        // differentiate single-channel from multi-channel volumes
        if (srcVolume->getNumChannels() == 1) {
            LINFOC("voreen.VolumeOperatorConvert", "Using fallback with setVoxelFloat and getVoxelFloat (single-channel)");
            VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::ivec3(0, 0, 0), srcVolume->getDimensions(), progressBar_)
                destVolume->setVoxelFloat(srcVolume->getVoxelFloat(i), i);
        }
        else {
            tgtAssert(srcVolume->getNumChannels() == destVolume->getNumChannels(), "channel-count mis-match");
            int numChannels = srcVolume->getNumChannels();
            LINFOC("voreen.VolumeOperatorConvert", "Using fallback with setVoxelFloat and getVoxelFloat (" << numChannels << " channels)");
            VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::ivec3(0, 0, 0), srcVolume->getDimensions(), progressBar_) {
                for (int channel=0; channel < numChannels; channel++)
                    destVolume->setVoxelFloat(srcVolume->getVoxelFloat(i, channel), i, channel);
            }
        }
    }

    if (progressBar_)
        progressBar_->setProgress(1.f);

    return new VolumeHandle(destVolume, srcVolumeHandle);
}

} // namespace

#endif // VRN_VOLUMEOPERATOR_H
