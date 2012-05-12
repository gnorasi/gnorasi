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

#ifndef VRN_VOLUMEOPERATORMORPHOLOGY_H
#define VRN_VOLUMEOPERATORMORPHOLOGY_H

#include "voreen/core/datastructures/volume/volumeoperator.h"

namespace voreen {

// Base class, defines interface for the operator (-> apply):
class VolumeOperatorErosionBase : public UnaryVolumeOperatorBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume, int kernelSize = 3) const = 0;
};

// Generic implementation:
template<typename T>
class VolumeOperatorErosionGeneric : public VolumeOperatorErosionBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume, int kernelSize = 3) const;
    //Implement isCompatible using a handy macro:
    IS_COMPATIBLE
};

template<typename T>
VolumeHandle* VolumeOperatorErosionGeneric<T>::apply(const VolumeHandleBase* vh, int kernelSize) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<T>* volume = dynamic_cast<const VolumeAtomic<T>*>(v);
    if(!volume)
        return 0;

    VolumeAtomic<T>* output = volume->clone();
    VolumeAtomic<T>* pong = volume->clone();

    size_t halfKernelDim = static_cast<size_t>(kernelSize / 2);
    tgt::svec3 volDim = volume->getDimensions();

    // kernel is separable => consecutively apply 1D kernel along each axis instead of a 3D kernel

    // x-direction (volumeVol -> outputVol)
    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS_SUB_TASK(pos, tgt::ivec3(0), volDim, progressBar_, 0.f, 1.f/3) { //FIXME
    VRN_FOR_EACH_VOXEL(pos, tgt::svec3(0, 0, 0), volDim) {
        size_t xmin = pos.x >= halfKernelDim ? pos.x - halfKernelDim : 0; 
        size_t xmax = std::min(pos.x+halfKernelDim, volDim.x-1);

        T val = volume->voxel(pos);
        tgt::svec3 npos = pos;
        for (npos.x=xmin; npos.x<=xmax; npos.x++) {
            val = std::min(val, volume->voxel(npos));
        }
        output->voxel(pos) = val;
    }

    // y-direction (outputVol -> volumeVol)
    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS_SUB_TASK(pos, tgt::ivec3(0), volDim, progressBar_, 1.f/3, 1.f/3) { //FIXME
    VRN_FOR_EACH_VOXEL(pos, tgt::svec3(0, 0, 0), volDim) {
        size_t ymin = pos.y >= halfKernelDim ? pos.y - halfKernelDim : 0; 
        size_t ymax = std::min(pos.y+halfKernelDim, volDim.y-1);

        T val = output->voxel(pos);
        tgt::svec3 npos = pos;
        for (npos.y=ymin; npos.y<=ymax; npos.y++) {
            val = std::min(val, output->voxel(npos));
        }
        pong->voxel(pos) = val;
    }

    // z-direction (volumeVol -> outputVol)
    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS_SUB_TASK(pos, tgt::ivec3(0), volDim, progressBar_, 2.f/3, 1.f/3) { //FIXME
    VRN_FOR_EACH_VOXEL(pos, tgt::svec3(0, 0, 0), volDim) {
        size_t zmin = pos.z >= halfKernelDim ? pos.z - halfKernelDim : 0; 
        size_t zmax = std::min(pos.z+halfKernelDim, volDim.z-1);

        T val = pong->voxel(pos);
        tgt::svec3 npos = pos;
        for (npos.z=zmin; npos.z<=zmax; npos.z++) {
            val = std::min(val, pong->voxel(npos));
        }
        output->voxel(pos) = val;
    }

    //if (progressBar_) //FIXME
        //progressBar_->setProgress(1.f);

    return new VolumeHandle(output, vh);
}

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorErosionBase> VolumeOperatorErosion;

// ========================================================================================

// Base class, defines interface for the operator (-> apply):
class VolumeOperatorDilationBase : public UnaryVolumeOperatorBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume, int kernelSize = 3) const = 0;
};

// Generic implementation:
template<typename T>
class VolumeOperatorDilationGeneric : public VolumeOperatorDilationBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume, int kernelSize = 3) const;
    //Implement isCompatible using a handy macro:
    IS_COMPATIBLE
};

template<typename T>
VolumeHandle* VolumeOperatorDilationGeneric<T>::apply(const VolumeHandleBase* vh, int kernelSize) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<T>* volume = dynamic_cast<const VolumeAtomic<T>*>(v);
    if(!volume)
        return 0;

    VolumeAtomic<T>* output = volume->clone();
    VolumeAtomic<T>* pong = volume->clone();

    size_t halfKernelDim = kernelSize / 2;
    tgt::svec3 volDim = volume->getDimensions();

    // kernel is separable => consecutively apply 1D kernel along each axis instead of a 3D kernel

    // x-direction (inputVol -> outputVol)
    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS_SUB_TASK(pos, tgt::ivec3(0), volDim, progressBar_, 0.f, 1.f/3) {//FIXME
    VRN_FOR_EACH_VOXEL(pos, tgt::svec3(0, 0, 0), volDim) {
        size_t xmin = pos.x >= halfKernelDim ? pos.x - halfKernelDim : 0; 
        size_t xmax = std::min(pos.x+halfKernelDim, volDim.x-1);

        T val = volume->voxel(pos);
        tgt::svec3 npos = pos;
        for (npos.x=xmin; npos.x<=xmax; npos.x++) {
            val = std::max(val, volume->voxel(npos));
        }
        output->voxel(pos) = val; 
    }

    // y-direction (outputVol -> inputVol)
    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS_SUB_TASK(pos, tgt::ivec3(0), volDim, progressBar_, 1.f/3, 1.f/3) { //FIXME
    VRN_FOR_EACH_VOXEL(pos, tgt::svec3(0, 0, 0), volDim) {
        size_t ymin = pos.y >= halfKernelDim ? pos.y - halfKernelDim : 0; 
        size_t ymax = std::min(pos.y+halfKernelDim, volDim.y-1);

        T val = output->voxel(pos);
        tgt::svec3 npos = pos;
        for (npos.y=ymin; npos.y<=ymax; npos.y++) {
            val = std::max(val, output->voxel(npos));
        }
        pong->voxel(pos) = val;
    }

    // z-direction (inputVol -> outputVol)
    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS_SUB_TASK(pos, tgt::ivec3(0), volDim, progressBar_, 2.f/3, 1.f/3) {
    VRN_FOR_EACH_VOXEL(pos, tgt::ivec3(0, 0, 0), volDim) {
        size_t zmin = pos.z >= halfKernelDim ? pos.z - halfKernelDim : 0; 
        size_t zmax = std::min(pos.z+halfKernelDim, volDim.z-1);

        T val = pong->voxel(pos);
        tgt::svec3 npos = pos;
        for (npos.z=zmin; npos.z<=zmax; npos.z++) {
            val = std::max(val, pong->voxel(npos));
        }
        output->voxel(pos) = val;
    }

    //if (progressBar_) //FIXME
        //progressBar_->setProgress(1.f);

    return new VolumeHandle(output, vh);
}

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorDilationBase> VolumeOperatorDilation;
} // namespace

#endif // VRN_VOLUMEOPERATOR_H
