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

#ifndef VRN_VOLUMEOPERATORNORMALIZE_H
#define VRN_VOLUMEOPERATORNORMALIZE_H

#include "voreen/core/datastructures/volume/volumeoperator.h"

namespace voreen {

class VolumeOperatorNormalizeBase : public UnaryVolumeOperatorBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* vh) const = 0;
};

template<typename T>
class VolumeOperatorNormalizeGeneric : public VolumeOperatorNormalizeBase {
public:
    VolumeHandle* apply(const VolumeHandleBase* vh) const;
    IS_COMPATIBLE
};

template<typename T>
VolumeHandle* VolumeOperatorNormalizeGeneric<T>::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if (!v)
        return 0;

    const VolumeAtomic<T>* va = dynamic_cast<const VolumeAtomic<T>*>(v);
    if (!va)
        return 0;

    VolumeAtomic<T>* normalized = va->clone();

    T minLocalValue = va->min();
    T maxLocalValue = va->max();
    T maxGlobalValue = static_cast<T>(1 << va->getBitsStored());
    
    VRN_FOR_EACH_VOXEL(i, tgt::ivec3(0), va->getDimensions()) {
        T value = va->voxel(i);
        normalized->voxel(i) = T((float(value - minLocalValue) / float(maxLocalValue - minLocalValue)) * maxGlobalValue);
    }

    return new VolumeHandle(normalized, vh);
}

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorNormalizeBase> VolumeOperatorNormalize;

} // namespace

#endif // VRN_VOLUMEOPERATORNORMALIZE_H
