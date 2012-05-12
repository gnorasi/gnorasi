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

#ifndef VRN_VOLUMEOPERATORISUNIFORM_H
#define VRN_VOLUMEOPERATORISUNIFORM_H

#include "voreen/core/datastructures/volume/volumeoperator.h"

namespace voreen {

// Base class, defines interface for the operator (-> apply):
class VolumeOperatorIsUniformBase : public UnaryVolumeOperatorBase {
public:
    virtual bool apply(const VolumeHandleBase* volume) const = 0;
};

// Generic implementation:
template<typename T>
class VolumeOperatorIsUniformGeneric : public VolumeOperatorIsUniformBase {
public:
    virtual bool apply(const VolumeHandleBase* volume) const;
    //Implement isCompatible using a handy macro:
    IS_COMPATIBLE
};

template<typename T>
bool VolumeOperatorIsUniformGeneric<T>::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<T>* volume = dynamic_cast<const VolumeAtomic<T>*>(v);
    if(!volume)
        return 0;

    T firstVoxel = volume->voxel(0);
    bool allVoxelsEqual=true;
    for (size_t i=1; i < volume->getNumVoxels(); i++) {
        T currentVoxel = volume->voxel(i);
        if (firstVoxel != currentVoxel) {
            allVoxelsEqual = false;
            break;
        }
    }
    return allVoxelsEqual;
}

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorIsUniformBase> VolumeOperatorIsUniform;

} // namespace

#endif // VRN_VOLUMEOPERATOR_H
