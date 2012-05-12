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

#ifndef VRN_VOLUMEOPERATORINVERT_H
#define VRN_VOLUMEOPERATORINVERT_H

#include "voreen/core/datastructures/volume/operators/volumeoperatorminmax.h"
#include "voreen/core/datastructures/volume/volumeoperator.h"

namespace voreen {

// Base class, defines interface for the operator (-> apply):
class VolumeOperatorInvertBase : public UnaryVolumeOperatorBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume) const = 0;
};

// Generic implementation:
template<typename T>
class VolumeOperatorInvertGeneric : public VolumeOperatorInvertBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume) const;
    //Implement isCompatible using a handy macro:
    IS_COMPATIBLE
};

template<typename T>
VolumeHandle* VolumeOperatorInvertGeneric<T>::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<T>* va = dynamic_cast<const VolumeAtomic<T>*>(v);
    if(!va)
        return 0;

    VolumeAtomic<T>* out = va->clone();
    T max = VolumeOperatorMaxValue::apply(va);
    T* voxel = out->voxel();
    for (size_t i = 0; i < va->getNumVoxels(); ++i)
        voxel[i] = max - voxel[i];

    return new VolumeHandle(out, vh);
}

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorInvertBase> VolumeOperatorInvert;

} // namespace

#endif // VRN_VOLUMEOPERATOR_H
