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

#include "voreen/core/processors/volumeprocessor.h"

namespace voreen {

VolumeProcessor::VolumeProcessor()
    : Processor()
{}

VolumeProcessor::~VolumeProcessor() {}

tgt::mat4 VolumeProcessor::computeConversionMatrix(const VolumeHandleBase* originVolume, const VolumeHandleBase* destinationVolume) const {
    if (originVolume == destinationVolume)
        return tgt::mat4::identity;
    else {
        //tgt::mat4 voxelToWorldOrigin = originVolume->getVoxelToWorldMatrix();
        //tgt::mat4 worldToVoxelDestination = destinationVolume->getWorldToVoxelMatrix();
        tgt::mat4 voxelToWorldOrigin = originVolume->getPhysicalToWorldMatrix();
        tgt::mat4 worldToVoxelDestination = destinationVolume->getWorldToPhysicalMatrix();

        return worldToVoxelDestination * voxelToWorldOrigin;
    }
}

//------------------------------------------------------------------------

CachingVolumeProcessor::CachingVolumeProcessor()
    : VolumeProcessor()
    , useCaching_("useCaching", "Use Cache", true, VALID)
    , clearCache_("clearCache", "Clear Cache", VALID)
    , cache_(this)
{
    addProperty(useCaching_);

    clearCache_.onChange(CallMemberAction<CachingVolumeProcessor>(this, &CachingVolumeProcessor::clearCache));
    addProperty(clearCache_);

    useCaching_.setGroupID("caching");
    clearCache_.setGroupID("caching");
}

CachingVolumeProcessor::~CachingVolumeProcessor() {}

void CachingVolumeProcessor::beforeProcess() {
    VolumeProcessor::beforeProcess();

    if (useCaching_.get()) {
        if(cache_.restore()) {
            setValid();
        }
    }
}

void CachingVolumeProcessor::afterProcess() {
    VolumeProcessor::afterProcess();

    if(useCaching_.get()) {
        cache_.store();
    }
}

void CachingVolumeProcessor::initialize() throw (tgt::Exception) {
    VolumeProcessor::initialize();

    cache_.addAllInports();
    cache_.addAllOutports();
    cache_.addAllProperties();

    cache_.initialize();
}

void CachingVolumeProcessor::clearCache() {
    cache_.clearCache();
}

}   // namespace
