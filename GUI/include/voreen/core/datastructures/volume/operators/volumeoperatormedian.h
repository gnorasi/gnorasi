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

#ifndef VRN_VOLUMEOPERATORMEDIAN_H
#define VRN_VOLUMEOPERATORMEDIAN_H

#include "voreen/core/datastructures/volume/volumeoperator.h"

namespace voreen {

// Base class, defines interface for the operator (-> apply):
class VolumeOperatorMedianBase : public UnaryVolumeOperatorBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume, int kernelSize = 3) const = 0;
};

// Generic implementation:
template<typename T>
class VolumeOperatorMedianGeneric : public VolumeOperatorMedianBase {
public:
    virtual VolumeHandle* apply(const VolumeHandleBase* volume, int kernelSize = 3) const;
    //Implement isCompatible using a handy macro:
    IS_COMPATIBLE
};

template<typename T>
VolumeHandle* VolumeOperatorMedianGeneric<T>::apply(const VolumeHandleBase* vh, int kernelSize) const {
    const Volume* v = vh->getRepresentation<Volume>();
    if(!v)
        return 0;

    const VolumeAtomic<T>* va = dynamic_cast<const VolumeAtomic<T>*>(v);
    if(!va)
        return 0;

    VolumeAtomic<T>* output = va->clone();

    size_t halfKernelDim = static_cast<size_t>(kernelSize / 2);
    tgt::svec3 volDim = vh->getDimensions();
    //VRN_FOR_EACH_VOXEL_WITH_PROGRESS(pos, tgt::ivec3(0), volDim, progressBar_) {
    VRN_FOR_EACH_VOXEL(pos, tgt::svec3(0, 0, 0), volDim) {
        size_t zmin = pos.z >= halfKernelDim ? pos.z - halfKernelDim : 0; 
        size_t zmax = std::min(pos.z+halfKernelDim, volDim.z-1);
        size_t ymin = pos.y >= halfKernelDim ? pos.y - halfKernelDim : 0; 
        size_t ymax = std::min(pos.y+halfKernelDim, volDim.y-1);
        size_t xmin = pos.x >= halfKernelDim ? pos.x - halfKernelDim : 0; 
        size_t xmax = std::min(pos.x+halfKernelDim, volDim.x-1);

        tgt::svec3 npos;
        std::vector<T> values;
        for (npos.z=zmin; npos.z<=zmax; npos.z++) {
            for (npos.y=ymin; npos.y<=ymax; npos.y++) {
                for (npos.x=xmin; npos.x<=xmax; npos.x++) {
                    values.push_back(va->voxel(npos));
                }
            }
        }
        size_t len = values.size();
        nth_element(values.begin(), values.begin()+(len/2), values.end());
        output->voxel(pos) = values[len / 2];
    }

    //if (progressBar_) //FIXME
        //progressBar_->setProgress(1.f);

    return new VolumeHandle(output, vh);
}

typedef UniversalUnaryVolumeOperatorGeneric<VolumeOperatorMedianBase> VolumeOperatorMedian;

} // namespace

#endif // VRN_VOLUMEOPERATOR_H
