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

#ifndef VRN_VOLUMEOPERATORNUMSIGNIFICANT_H
#define VRN_VOLUMEOPERATORNUMSIGNIFICANT_H

#include "voreen/core/datastructures/volume/volumeoperator.h"

namespace voreen {

/**
 * Returns the number of significant voxels, i.e., the number of voxels with
 * a value greater than the smallest possible value that can be stored by the volume.
 */
class VolumeOperatorNumSignificantBase : public UnaryVolumeOperatorBase {
public:
    virtual size_t apply(const VolumeHandleBase* volume) const = 0;
};

// ============================================================================

// Generic implementation:
template<typename T>
class VolumeOperatorNumSignificantGeneric : public VolumeOperatorNumSignificantBase {
public:
    virtual size_t apply(const VolumeHandleBase* volume) const;
    //Implement isCompatible using a handy macro:
    IS_COMPATIBLE
};

template<typename T>
size_t VolumeOperatorNumSignificantGeneric<T>::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<T>* volume = dynamic_cast<const VolumeAtomic<T>*>(v);
    if(!volume)
        return 0;

    size_t result = 0;
    VRN_FOR_EACH_VOXEL(pos, tgt::ivec3(0), volume->getDimensions()) {
        if (volume->voxel(pos) > VolumeElement<T>::rangeMin())
            result++;
    }
    return result;
}

// ============================================================================

template<typename S>
class VolumeOperatorNumSignificantGeneric<tgt::Vector2<S> >: public VolumeOperatorNumSignificantBase {
public:
    virtual size_t apply(const VolumeHandleBase* volume) const;
    bool isCompatible(const VolumeHandleBase* volume) const {
        const Volume* v = volume->getRepresentation<Volume>();
        if(!v)
            return false;
        const VolumeAtomic<tgt::Vector2<S> >* va = dynamic_cast<const VolumeAtomic<tgt::Vector2<S> >*>(v);
        if(!va)
            return false;
        return true;
    }
};

template<typename S>
size_t VolumeOperatorNumSignificantGeneric<tgt::Vector2<S> >::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<tgt::Vector2<S> >* volume = dynamic_cast<const VolumeAtomic<tgt::Vector2<S> >*>(v);
    if(!volume)
        return 0;

    size_t result = 0;
    VRN_FOR_EACH_VOXEL(pos, tgt::ivec3(0), volume->getDimensions()) {
        if (tgt::hadd(volume->voxel(pos)) > VolumeElement<S>::rangeMin())
            result++;
    }
    return result;
}

// ============================================================================

template<typename S>
class VolumeOperatorNumSignificantGeneric<tgt::Vector3<S> >: public VolumeOperatorNumSignificantBase {
public:
    virtual size_t apply(const VolumeHandleBase* volume) const;
    bool isCompatible(const VolumeHandleBase* volume) const {
        const Volume* v = volume->getRepresentation<Volume>();
        if(!v)
            return false;
        const VolumeAtomic<tgt::Vector3<S> >* va = dynamic_cast<const VolumeAtomic<tgt::Vector3<S> >*>(v);
        if(!va)
            return false;
        return true;
    }
};

template<typename S>
size_t VolumeOperatorNumSignificantGeneric<tgt::Vector3<S> >::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<tgt::Vector3<S> >* volume = dynamic_cast<const VolumeAtomic<tgt::Vector3<S> >*>(v);
    if(!volume)
        return 0;

    size_t result = 0;
    VRN_FOR_EACH_VOXEL(pos, tgt::ivec3(0), volume->getDimensions()) {
        if (tgt::hadd(volume->voxel(pos)) > VolumeElement<S>::rangeMin())
            result++;
    }
    return result;
}


// ============================================================================

template<typename S>
class VolumeOperatorNumSignificantGeneric<tgt::Vector4<S> >: public VolumeOperatorNumSignificantBase {
public:
    virtual size_t apply(const VolumeHandleBase* volume) const;
    bool isCompatible(const VolumeHandleBase* volume) const {
        const Volume* v = volume->getRepresentation<Volume>();
        if(!v)
            return false;
        const VolumeAtomic<tgt::Vector4<S> >* va = dynamic_cast<const VolumeAtomic<tgt::Vector4<S> >*>(v);
        if(!va)
            return false;
        return true;
    }
};

template<typename S>
size_t VolumeOperatorNumSignificantGeneric<tgt::Vector4<S> >::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<tgt::Vector4<S> >* volume = dynamic_cast<const VolumeAtomic<tgt::Vector4<S> >*>(v);
    if(!volume)
        return 0;

    size_t result = 0;
    VRN_FOR_EACH_VOXEL(pos, tgt::ivec3(0), volume->getDimensions()) {
        if (tgt::hadd(volume->voxel(pos)) > VolumeElement<S>::rangeMin())
            result++;
    }
    return result;
}


// ============================================================================

template<typename S>
class VolumeOperatorNumSignificantGeneric<Tensor2<S> >: public VolumeOperatorNumSignificantBase {
public:
    virtual size_t apply(const VolumeHandleBase* volume) const;
    bool isCompatible(const VolumeHandleBase* volume) const {
        const Volume* v = volume->getRepresentation<Volume>();
        if(!v)
            return false;
        const VolumeAtomic<Tensor2<S> >* va = dynamic_cast<const VolumeAtomic<Tensor2<S> >*>(v);
        if(!va)
            return false;
        return true;
    }
};

template<typename S>
size_t VolumeOperatorNumSignificantGeneric<Tensor2<S> >::apply(const VolumeHandleBase* vh) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<Tensor2<S> >* volume = dynamic_cast<const VolumeAtomic<Tensor2<S> >*>(v);
    if(!volume)
        return 0;

    size_t result = 0;
    VRN_FOR_EACH_VOXEL(pos, tgt::ivec3(0), volume->getDimensions()) {
        const Tensor2<S>& t = volume->voxel(pos);
        if ((t.Dxx + t.Dxy + t.Dxz + t.Dyy + t.Dyz + t.Dzz) > VolumeElement<S>::rangeMin())
            result++;
    }
    return result;
}

// ============================================================================

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorNumSignificantBase> VolumeOperatorNumSignificant;

} // namespace

#endif // VRN_VOLUMEOPERATOR_H
