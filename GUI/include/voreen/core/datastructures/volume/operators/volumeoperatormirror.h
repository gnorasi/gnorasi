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

#ifndef VRN_VOLUMEOPERATORMIRROR_H
#define VRN_VOLUMEOPERATORMIRROR_H

#include "voreen/core/datastructures/volume/volumeoperator.h"

namespace voreen {

///Mirrors the volume on the X axis.
class VolumeOperatorMirrorXBase : public UnaryVolumeOperatorBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* vh) const = 0;
};

template<typename T>
class VolumeOperatorMirrorXGeneric : public VolumeOperatorMirrorXBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* vh) const;
    IS_COMPATIBLE
};

template<typename T>
VolumeHandle* VolumeOperatorMirrorXGeneric<T>::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<T>* va = dynamic_cast<const VolumeAtomic<T>*>(v);
    if(!va)
        return 0;

    VolumeAtomic<T>* mirror = va->clone();

    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::ivec3(0), mirror->getDimensions(), progressBar_) //FIXME
    VRN_FOR_EACH_VOXEL(i, tgt::ivec3(0), mirror->getDimensions())
        mirror->voxel(i) = va->voxel(mirror->getDimensions().x - i.x - 1, i.y, i.z);

    //if (progressBar_) //FIXME
        //progressBar_->setProgress(1.f);

    return new VolumeHandle(mirror, vh);
}

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorMirrorXBase> VolumeOperatorMirrorX;

// ============================================================================
///Mirrors the volume on the Y axis.
class VolumeOperatorMirrorYBase : public UnaryVolumeOperatorBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume) const = 0;
};

template<typename T>
class VolumeOperatorMirrorYGeneric : public VolumeOperatorMirrorYBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* vh) const;
    IS_COMPATIBLE
};

template<typename T>
VolumeHandle* VolumeOperatorMirrorYGeneric<T>::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<T>* va = dynamic_cast<const VolumeAtomic<T>*>(v);
    if(!va)
        return 0;

    VolumeAtomic<T>* mirror = va->clone();

    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::ivec3(0), mirror->getDimensions(), progressBar_) //FIXME
    VRN_FOR_EACH_VOXEL(i, tgt::ivec3(0), mirror->getDimensions())
        mirror->voxel(i) = va->voxel(i.x, va->getDimensions().y - i.y - 1, i.z);

    //if (progressBar_) //FIXME
        //progressBar_->setProgress(1.f);

    return new VolumeHandle(mirror, vh);
}

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorMirrorYBase> VolumeOperatorMirrorY;

// ============================================================================
///Mirrors the volume on the Z axis.
class VolumeOperatorMirrorZBase : public UnaryVolumeOperatorBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume) const = 0;
};

template<typename T>
class VolumeOperatorMirrorZGeneric : public VolumeOperatorMirrorZBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* vh) const;
    IS_COMPATIBLE
};

template<typename T>
VolumeHandle* VolumeOperatorMirrorZGeneric<T>::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<T>* va = dynamic_cast<const VolumeAtomic<T>*>(v);
    if(!va)
        return 0;

    VolumeAtomic<T>* mirror = va->clone();

    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS(i, tgt::ivec3(0), mirror->getDimensions(), progressBar_)
    VRN_FOR_EACH_VOXEL(i, tgt::ivec3(0), mirror->getDimensions())
        mirror->voxel(i) = va->voxel(i.x, i.y, mirror->getDimensions().z - i.z - 1);

    //if (progressBar_) //FIXME
        //progressBar_->setProgress(1.f);

    return new VolumeHandle(mirror, vh);
}

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorMirrorZBase> VolumeOperatorMirrorZ;

// ============================================================================

} // namespace

#endif // VRN_VOLUMEOPERATOR_H
