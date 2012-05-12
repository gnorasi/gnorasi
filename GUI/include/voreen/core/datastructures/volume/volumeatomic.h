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

#ifndef VRN_VOLUMEATOMIC_H
#define VRN_VOLUMEATOMIC_H

#include <algorithm>
#include <cstring>
#include <fstream>

#include "tgt/assert.h"
#include "tgt/logmanager.h"

#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumeelement.h"

#include "voreen/core/datastructures/tensor.h"

#include <typeinfo>

namespace voreen {

template<class T>
class VolumeAtomic : public Volume {

public:
    typedef T VoxelType;

    enum {
        BYTES_PER_VOXEL = sizeof(T),
        BITS_PER_VOXEL  = BYTES_PER_VOXEL * 8
    };

    /**
     * While using this constructor the class will automatically allocate
     * an appropriate chunk of memory. This memory will be deleted by this class.
     * If allocMem is false, no memory will be allocated. 
     */
    VolumeAtomic(const tgt::svec3& dimensions,
                 int bitsStored = BITS_PER_VOXEL, 
                 const VolumeRepresentation::VolumeBorders& border = VolumeRepresentation::VolumeBorders(),
                 bool allocMem=true) throw (std::bad_alloc);

    /**
     * While using this constructor the class will use an preallocated chunk
     * of memory given in \p data. This memory will be deleted by this class.
     */
    VolumeAtomic(T* data,
                 const tgt::svec3& dimensions,
                 int bitsStored = BITS_PER_VOXEL,
                 const VolumeRepresentation::VolumeBorders& border = VolumeRepresentation::VolumeBorders());

    /// Deletes the \a data_ array
    virtual ~VolumeAtomic();

    virtual VolumeAtomic<T>* clone() const throw (std::bad_alloc);
    virtual VolumeAtomic<T>* clone(void* data) const throw (std::bad_alloc);
    virtual VolumeAtomic<T>* createNew(const tgt::svec3& dimensions,
                             const VolumeRepresentation::VolumeBorders& border = VolumeRepresentation::VolumeBorders(),
                             bool allocMem = false) const throw (std::bad_alloc);
    virtual VolumeAtomic<T>* getSubVolume(const tgt::svec3& dimensions, 
                             const tgt::svec3& offset = tgt::svec3(0,0,0), 
                             const VolumeRepresentation::VolumeBorders& border = VolumeRepresentation::VolumeBorders()) const throw (std::bad_alloc);
    virtual void setSubVolume(const Volume* vol, 
                              const tgt::svec3& offset = tgt::svec3(0,0,0));

    /*
     * Static properties
     */

    virtual int getBitsAllocated() const;

    virtual int getNumChannels() const;

    virtual int getBytesPerVoxel() const;

    virtual void setBitsStored(int bits);

    /**
     * Returns whether the volume's data type is signed.
     */
    static bool isSigned();

    /**
     * Returns the minimal and maximal element data values that can be contained
     * by the volume according to its data type.
     *
     * @note for unbounded types, (0.0, 1.0) is returned
     */
    virtual tgt::vec2 elementRange() const;

    static bool isInteger();

    /**
     * Returns the minimum data value that is actually contained by the volume.
     *
     * @note Calculated min/max values are cached. Call invalidate() in order to
     *      enforce a recalculation.
     */
    T min() const;

    /**
     * Returns the maximal data value that is actually contained by the volume.
     *
     * @note Calculated min/max values are cached. Call invalidate() in order to
     *      enforce a recalculation.
     */
    T max() const;

    /**
     * Returns the minimum value contained by the specified channel converted to float.
     */
    virtual float minValue(size_t channel = 0) const;

    /**
     * Returns the maximum value contained by the specified channel converted to float.
     */
    virtual float maxValue(size_t channel = 0) const;

    virtual size_t getNumBytes() const;

    /*
     * Helpers for calculating the position in 3d
     */

    inline static size_t calcPos(const tgt::svec3& dimensions, const VolumeRepresentation::VolumeBorders& borders, size_t x, size_t y, size_t z);

    inline static size_t calcPos(const tgt::svec3& dimensions, const VolumeRepresentation::VolumeBorders& borders, const tgt::svec3& pos);

    inline static size_t calcPos(const tgt::svec3& dimensions, const VolumeRepresentation::VolumeBorders& borders, size_t i);

    inline static size_t calcPos(const tgt::svec3& dimensions, size_t x, size_t y, size_t z);

    inline static size_t calcPos(const tgt::svec3& dimensions, const tgt::svec3& pos);

    /*
      Methods for accessing the voxels
    */

    /// just get a proper pointer for read and write access
    inline T* voxel();

    /// just get a proper pointer for read access only
    inline const T* voxel() const;

    /// get or set voxel
    inline T& voxel(size_t x, size_t y, size_t z);

    /// get voxel
    inline const T& voxel(size_t x, size_t y, size_t z) const;

    /// get or set voxel
    inline T& voxel(const tgt::svec3& pos);

    /// get voxel
    inline const T& voxel(const tgt::svec3& pos) const;

    /// get or set voxel
    inline T& voxel(size_t i);

    /// get voxel
    inline const T& voxel(size_t i) const;


    /*
     * getVoxelFloat and setVoxelFloat
     */
    virtual float getVoxelFloat(const tgt::svec3& pos, size_t channel = 0) const;
    virtual float getVoxelFloat(size_t x, size_t y, size_t z, size_t channel = 0) const;
    virtual float getVoxelFloat(size_t index, size_t channel = 0) const;
    virtual void setVoxelFloat(float value, const tgt::svec3& pos, size_t channel = 0);
    virtual void setVoxelFloat(float value, size_t x, size_t y, size_t z, size_t channel = 0);
    virtual void setVoxelFloat(float value, size_t index, size_t channel = 0);

    virtual void clear();
    virtual const void* getData() const;
    virtual void* getData();

    /**
     * Invalidates cached values (e.g. min/max), should be called when the volume was modified.
     */
    virtual void invalidate() const;

    //-------------------------------------------------------------------
protected:
    // protected default constructor
    VolumeAtomic() {}

    T* data_;

    tgt::vec2 elementRange_;

    mutable T maxValue_;
    mutable T minValue_;
    mutable bool minMaxValid_;


private:
    /**
     * Calculates the minimum and maximum data value that is actually contained by the volume.
     */
    void updateMinMax() const;
};

/*
 * typedefs for easy usage
 */

typedef VolumeAtomic<uint8_t>   VolumeUInt8;
typedef VolumeAtomic<uint16_t>  VolumeUInt16;
typedef VolumeAtomic<uint32_t>  VolumeUInt32;
typedef VolumeAtomic<uint64_t>  VolumeUInt64;

typedef VolumeAtomic<int8_t>    VolumeInt8;
typedef VolumeAtomic<int16_t>   VolumeInt16;
typedef VolumeAtomic<int32_t>   VolumeInt32;
typedef VolumeAtomic<int64_t>   VolumeInt64;

typedef VolumeAtomic<float>     VolumeFloat;
typedef VolumeAtomic<double>    VolumeDouble;

typedef VolumeAtomic<tgt::Vector2<uint8_t> >    Volume2xUInt8;
typedef VolumeAtomic<tgt::Vector2< int8_t> >    Volume2xInt8;
typedef VolumeAtomic<tgt::Vector2<uint16_t> >   Volume2xUInt16;
typedef VolumeAtomic<tgt::Vector2< int16_t> >   Volume2xInt16;
typedef VolumeAtomic<tgt::Vector2<uint32_t> >   Volume2xUInt32;
typedef VolumeAtomic<tgt::Vector2< int32_t> >   Volume2xInt32;
typedef VolumeAtomic<tgt::Vector2<uint64_t> >   Volume2xUInt64;
typedef VolumeAtomic<tgt::Vector2< int64_t> >   Volume2xInt64;
typedef VolumeAtomic<tgt::vec2>                 Volume2xFloat;
typedef VolumeAtomic<tgt::dvec2>                Volume2xDouble;

typedef VolumeAtomic<tgt::Vector3<uint8_t> >    Volume3xUInt8;
typedef VolumeAtomic<tgt::Vector3<int8_t> >     Volume3xInt8;
typedef VolumeAtomic<tgt::Vector3<uint16_t> >   Volume3xUInt16;
typedef VolumeAtomic<tgt::Vector3<int16_t> >    Volume3xInt16;
typedef VolumeAtomic<tgt::Vector3<uint32_t> >   Volume3xUInt32;
typedef VolumeAtomic<tgt::Vector3<int32_t> >    Volume3xInt32;
typedef VolumeAtomic<tgt::Vector3<uint64_t> >   Volume3xUInt64;
typedef VolumeAtomic<tgt::Vector3<int64_t> >    Volume3xInt64;
typedef VolumeAtomic<tgt::vec3>                 Volume3xFloat;
typedef VolumeAtomic<tgt::dvec3>                Volume3xDouble;

typedef VolumeAtomic<tgt::Vector4<uint8_t> >    Volume4xUInt8;
typedef VolumeAtomic<tgt::Vector4<int8_t> >     Volume4xInt8;
typedef VolumeAtomic<tgt::Vector4<uint16_t> >   Volume4xUInt16;
typedef VolumeAtomic<tgt::Vector4<int16_t> >    Volume4xInt16;
typedef VolumeAtomic<tgt::Vector4<uint32_t> >   Volume4xUInt32;
typedef VolumeAtomic<tgt::Vector4<int32_t> >    Volume4xInt32;
typedef VolumeAtomic<tgt::Vector4<uint64_t> >   Volume4xUInt64;
typedef VolumeAtomic<tgt::Vector4<int64_t> >    Volume4xInt64;
typedef VolumeAtomic<tgt::vec4>                 Volume4xFloat;
typedef VolumeAtomic<tgt::dvec4>                Volume4xDouble;

typedef VolumeAtomic<tgt::mat3>  VolumeMat3Float;

typedef VolumeAtomic<Tensor2<float> >  VolumeTensor2Float;

//------------------------------------------------------------------------------
//  implementation
//------------------------------------------------------------------------------

/*
 * constructors and destructor
 */

template<class T>
VolumeAtomic<T>::VolumeAtomic(const tgt::svec3& dimensions, int bitsStored, const VolumeRepresentation::VolumeBorders& border, bool allocMem)
    throw (std::bad_alloc)
    : Volume(dimensions, bitsStored, border)
    , data_(0)
    , elementRange_(static_cast<float>(VolumeElement<T>::rangeMinElement()),
        static_cast<float>(VolumeElement<T>::rangeMaxElement()))
    , minMaxValid_(false)
{

    // special treatment for 12 bit volumes stored in 16 bit
    if (typeid(T) == typeid(uint16_t) && bitsStored == 12)
        elementRange_.y = static_cast<float>((1 << 12) - 1);

    if (allocMem) {
        try {
            data_ = new T[getNumVoxelsWithBorder()];
        }
        catch (std::bad_alloc) {
            LERROR("Failed to allocate memory: bad allocation");
            throw; // throw it to the caller
        }
    }
}

template<class T>
VolumeAtomic<T>::VolumeAtomic(T* data,
                              const tgt::svec3& dimensions,
                              int bitsStored,
                              const VolumeRepresentation::VolumeBorders& border)
    : Volume(dimensions, bitsStored, border)
    , data_(data)
    , elementRange_(static_cast<float>(VolumeElement<T>::rangeMinElement()),
         static_cast<float>(VolumeElement<T>::rangeMaxElement()))
    , minMaxValid_(false)
{
    // special treatment for 12 bit volumes stored in 16 bit
    if (typeid(T) == typeid(uint16_t) && bitsStored == 12)
        elementRange_.y = static_cast<float>((1 << 12) - 1);
}

template<class T>
VolumeAtomic<T>* VolumeAtomic<T>::clone() const
    throw (std::bad_alloc)
{
    // create clone
    VolumeAtomic<T>* newVolume = 0;
    try {
        newVolume = new VolumeAtomic<T>(dimensions_, getBitsStored(), getBorder()); // allocate a chunk of data
    }
    catch (const std::bad_alloc&) {
        LERROR("Failed to clone volume: bad allocation");
        throw; // throw it to the caller
    }

    // copy over the voxel data
    memcpy(newVolume->data_, data_, getNumBytes());

    return newVolume;
}

template<class T>
VolumeAtomic<T>* VolumeAtomic<T>::clone(void* data) const
    throw (std::bad_alloc)
{
    // create clone
    VolumeAtomic<T>* newVolume = 0;
    if (data) {
        // use preallocated data
        newVolume = new VolumeAtomic<T>((T*) data, dimensions_, getBitsStored(), getBorder());
    }
    else {
        // create volume without allocating memory
        newVolume = new VolumeAtomic<T>(dimensions_, getBitsStored(), getBorder(), false);
    }

    return newVolume;
}

template<class T>
VolumeAtomic<T>* VolumeAtomic<T>::createNew(const tgt::svec3& dimensions, const VolumeRepresentation::VolumeBorders& border, bool allocMem) const
throw (std::bad_alloc)
{
    // create new volume
    VolumeAtomic<T>* newVolume = new VolumeAtomic<T>(dimensions, getBitsStored(), border, allocMem);
        
    return newVolume;
}

template<class T>
VolumeAtomic<T>* VolumeAtomic<T>::getSubVolume(const tgt::svec3& dimensions, const tgt::svec3& offset, const VolumeRepresentation::VolumeBorders& border) const
throw (std::bad_alloc)
{
    // create new volume
    VolumeAtomic<T>* newVolume = new VolumeAtomic<T>(dimensions, getBitsStored(), border, true);
    newVolume->originalDimensions_ = getDimensions();
    T* data = reinterpret_cast<T*>(newVolume->getData());

    // determine parameters
    tgt::svec3 dataDims = getDimensions();
    tgt::svec3 offsetDims = offset-border.llf;
    offsetDims = tgt::max(offsetDims, tgt::svec3(0,0,0));;
    size_t initialStartPos = (offsetDims.z * (dataDims.x*dataDims.y))+(offsetDims.y * dataDims.x) + offsetDims.x;
    tgt::svec3 dimsWithBorder = dimensions+border.llf+border.urb;

    // per row
    size_t dataSize = dimsWithBorder.x*static_cast<size_t>(getBytesPerVoxel());

    // memcpy each row for every slice to form sub volume
    size_t volumePos;
    size_t subVolumePos;
    for (size_t i=0; i < dimsWithBorder.z; i++) {
        for (size_t j=0; j < dimsWithBorder.y; j++) {
            volumePos = (j*dataDims.x) + (i*dataDims.x*dataDims.y);
            subVolumePos = (j*dimsWithBorder.x) + (i*dimsWithBorder.x*dimsWithBorder.y);
            memcpy(data + subVolumePos, (data_ + volumePos + initialStartPos), dataSize);
        }
    }

    return newVolume;
}

template<class T>
void VolumeAtomic<T>::setSubVolume(const Volume* vol, const tgt::svec3& offset)
{
    const T* data = reinterpret_cast<const T*>(vol->getData());

    // determine parameters
    tgt::svec3 dataDims = getDimensions();
    size_t initialStartPos = (offset.z * (dataDims.x*dataDims.y))+(offset.y * dataDims.x) + offset.x;

    // per row
    tgt::svec3 dimensions = vol->getDimensions();
    size_t dataSize = dimensions.x*static_cast<size_t>(getBytesPerVoxel());

    // internal dimension including volume of the subvolume
    VolumeRepresentation::VolumeBorders border = vol->getBorder();
    tgt::svec3 internalDimension = vol->getDimensionsWithBorder();

    // memcpy each row for every slice in sub volume to form this volume
    size_t volumePos;
    size_t subVolumePos;
    for (size_t i=0; i < dimensions.z; i++) {
        for (size_t j=0; j < dimensions.y; j++) {
            volumePos =  (j*dataDims.x) + (i*dataDims.x*dataDims.y);
            subVolumePos = ((j+border.llf.y)*internalDimension.x) + ((i+border.llf.z)*internalDimension.x*internalDimension.y) + border.llf.x;
            memcpy((data_ + volumePos + initialStartPos), (data + subVolumePos), dataSize);
        }
    }
}

template<class T>
VolumeAtomic<T>::~VolumeAtomic() {
    delete[] data_;
}


template<class T>
int VolumeAtomic<T>::getNumChannels() const {
    return VolumeElement<T>::getNumChannels();
}

template<class T>
bool VolumeAtomic<T>::isSigned() {
    return VolumeElement<T>::isSigned();
}

template<class T>
bool voreen::VolumeAtomic<T>::isInteger() {
    return VolumeElement<T>::isInteger();
}

template<class T>
void voreen::VolumeAtomic<T>::setBitsStored(int bits) {
    setBitsStored(bits);

    // special treatment for 12 bit volumes stored in 16 bit
    if (typeid(T) == typeid(uint16_t) && getBitsStored() == 12)
        elementRange_.y = static_cast<float>((1 << 12) - 1);
}

/*
 * getVoxelFloat and setVoxelFloat
 */

template<class T>
float VolumeAtomic<T>::getVoxelFloat(const tgt::svec3& pos, size_t channel) const {
    return getVoxelFloat(calcPos(dimensions_, pos), channel);
}

template<class T>
float VolumeAtomic<T>::getVoxelFloat(size_t x, size_t y, size_t z, size_t channel) const {
    return getVoxelFloat(tgt::svec3(x, y, z), channel);
}

template<class T>
float voreen::VolumeAtomic<T>::getVoxelFloat(size_t index, size_t channel) const {
    typedef typename VolumeElement<T>::BaseType Base;

    Base value = VolumeElement<T>::getChannel(voxel(index), channel);
    return getTypeAsFloat(value);
}

template<class T>
void VolumeAtomic<T>::setVoxelFloat(float value, const tgt::svec3& pos, size_t channel) {

    setVoxelFloat(value, calcPos(dimensions_, pos), channel);
}

template<class T>
void VolumeAtomic<T>::setVoxelFloat(float value, size_t x, size_t y, size_t z, size_t channel) {
    setVoxelFloat(value, tgt::svec3(x, y, z), channel);
}

template<class T>
void voreen::VolumeAtomic<T>::setVoxelFloat(float value, size_t index, size_t channel) {
    typedef typename VolumeElement<T>::BaseType Base;

    VolumeElement<T>::setChannel(getFloatAsType<Base>(value), voxel(index), channel);
}


/*
 * getters and setters
 */

template<class T>
int VolumeAtomic<T>::getBitsAllocated() const {
    return BITS_PER_VOXEL;
}

template<class T>
int VolumeAtomic<T>::getBytesPerVoxel() const {
    return BYTES_PER_VOXEL;
}

template<class T>
size_t VolumeAtomic<T>::getNumBytes() const {
    return sizeof(T) * getNumVoxelsWithBorder();
}

/*
 * Helpers for calculating the position in 3d
 */

template<class T>
inline size_t VolumeAtomic<T>::calcPos(const tgt::svec3& dimensions, const VolumeRepresentation::VolumeBorders& borders, size_t x, size_t y, size_t z) {
    //we need to work with size_t to avoid problems when working with very large datasets
    tgt::svec3 dimsInclBorders = dimensions + borders.llf + borders.urb;
    return (z+borders.llf.z)*dimsInclBorders.x*dimsInclBorders.y + (y+borders.llf.y)*dimsInclBorders.x + x + borders.llf.x;
}

template<class T>
inline size_t VolumeAtomic<T>::calcPos(const tgt::svec3& dimensions, const VolumeRepresentation::VolumeBorders& borders, const tgt::svec3& pos) {
    //we need to work with size_t to avoid problems when working with very large datasets
    tgt::svec3 dimsInclBorders = dimensions + borders.llf + borders.urb;
    return (pos.z+borders.llf.z)*dimsInclBorders.x*dimsInclBorders.y + (pos.y+borders.llf.y)*dimsInclBorders.x + pos.x + borders.llf.x;
}

template<class T>
inline size_t VolumeAtomic<T>::calcPos(const tgt::svec3& dimensions, const VolumeRepresentation::VolumeBorders& borders, size_t i) {
    //we need to work with size_t to avoid problems when working with very large datasets
    //get position based on i
    size_t z = i / (dimensions.x*dimensions.y);
    z = (z*dimensions.x*dimensions.y > i ? z - 1 : z);
    i -= z*dimensions.x*dimensions.y;
    size_t y = i / dimensions.x;
    y = (y*dimensions.x > i ? y - 1 : y);
    size_t x = i - y*dimensions.x;
    return calcPos(dimensions, borders, x, y, z);
}

template<class T>
inline size_t VolumeAtomic<T>::calcPos(const tgt::svec3& dimensions, size_t x, size_t y, size_t z) {
    //we need to work with size_t to avoid problems when working with very large datasets
    return z*dimensions.x*dimensions.y + y*dimensions.x + x;
}

template<class T>
inline size_t VolumeAtomic<T>::calcPos(const tgt::svec3& dimensions, const tgt::svec3& pos) {
    //we need to work with size_t to avoid problems when working with very large datasets
    return pos.z*dimensions.x*dimensions.y + pos.y*dimensions.x + pos.x;
}

/*
 * further methods
 */

template<class T>
tgt::vec2 VolumeAtomic<T>::elementRange() const {
    return elementRange_;
}

template<class T>
void VolumeAtomic<T>::updateMinMax() const {
    if(hasBorder()){
        T val = voxel(0,0,0);
        maxValue_ = minValue_ = val;
        for(size_t z=0; z < dimensions_.z; z++){
            for(size_t y=0; y < dimensions_.y; y++){
                for(size_t x=0; x < dimensions_.x; x++){
                    val = voxel(x,y,z);
                    minValue_ = std::min(minValue_, val);
                    maxValue_ = std::max(maxValue_, val);
                }
            }
        }
    }
    else{
        minValue_ = *std::min_element(data_, data_ + getNumVoxels());
        maxValue_ = *std::max_element(data_, data_ + getNumVoxels());
    }

    minMaxValid_ = true;
}

template<class T>
T VolumeAtomic<T>::min() const {
    if (!minMaxValid_) {
        updateMinMax();
    }

    return minValue_;
}

template<class T>
T VolumeAtomic<T>::max() const {
    if (!minMaxValid_) {
        updateMinMax();
    }

    return maxValue_;
}

template<class T>
float voreen::VolumeAtomic<T>::minValue(size_t channel) const {
    typedef typename VolumeElement<T>::BaseType Base;
    Base value = VolumeElement<T>::getChannel(minValue_, channel);
    return getTypeAsFloat(value);
}

template<class T>
float voreen::VolumeAtomic<T>::maxValue(size_t channel) const {
    typedef typename VolumeElement<T>::BaseType Base;
    Base value = VolumeElement<T>::getChannel(maxValue_, channel);
    return getTypeAsFloat(value);
}

template<class T>
void VolumeAtomic<T>::clear() {
    memset(data_, 0, getNumBytes());
    invalidate();
}

template<class T>
void VolumeAtomic<T>::invalidate() const {
    minMaxValid_ = false;
}

template<class T>
const void* VolumeAtomic<T>::getData() const {
    return reinterpret_cast<const void*>(data_);
}

template<class T>
void* VolumeAtomic<T>::getData() {
    return reinterpret_cast<void*>(data_);
}

/*
  Methods for accessing the voxels
*/

/// just get a proper pointer for read and write access
template<class T>
inline T* VolumeAtomic<T>::voxel() {
    return data_;
}

/// just get a proper pointer for read access only
template<class T>
inline const T* VolumeAtomic<T>::voxel() const {
    return data_;
}

/// get or set voxel
template<class T>
inline T& VolumeAtomic<T>::voxel(size_t x, size_t y, size_t z) {
    if(hasBorder()){
        tgtAssert(x < size_t(dimensions_.x + borders_.llf.x + borders_.urb.x), "x index out of bounds");
        tgtAssert(y < size_t(dimensions_.y + borders_.llf.y + borders_.urb.y), "y index out of bounds");
        tgtAssert(z < size_t(dimensions_.z + borders_.llf.z + borders_.urb.z), "z index out of bounds");
        return data_[calcPos(dimensions_, borders_, x, y, z)];
    }
    else{
        tgtAssert(x < size_t(dimensions_.x), "x index out of bounds");
        tgtAssert(y < size_t(dimensions_.y), "y index out of bounds");
        tgtAssert(z < size_t(dimensions_.z), "z index out of bounds");
        return data_[calcPos(dimensions_, x, y, z)];
    }
}

/// get voxel
template<class T>
inline const T& VolumeAtomic<T>::voxel(size_t x, size_t y, size_t z) const {
    if(hasBorder()){
        tgtAssert(x < size_t(dimensions_.x + borders_.llf.x + borders_.urb.x), "x index out of bounds");
        tgtAssert(y < size_t(dimensions_.y + borders_.llf.y + borders_.urb.y), "y index out of bounds");
        tgtAssert(z < size_t(dimensions_.z + borders_.llf.z + borders_.urb.z), "z index out of bounds");
        return data_[calcPos(dimensions_, borders_, x, y, z)];
    }
    else{
        tgtAssert(x < size_t(dimensions_.x), "x index out of bounds");
        tgtAssert(y < size_t(dimensions_.y), "y index out of bounds");
        tgtAssert(z < size_t(dimensions_.z), "z index out of bounds");
        return data_[calcPos(dimensions_, x, y, z)];
    }
}

/// get or set voxel
template<class T>
inline T& VolumeAtomic<T>::voxel(const tgt::svec3& pos) {
    if(hasBorder()){
        tgtAssert(pos.x < dimensions_.x + borders_.llf.x + borders_.urb.x, "x index out of bounds");
        tgtAssert(pos.y < dimensions_.y + borders_.llf.y + borders_.urb.y, "y index out of bounds");
        tgtAssert(pos.z < dimensions_.z + borders_.llf.z + borders_.urb.z, "z index out of bounds");
        return data_[calcPos(dimensions_, borders_, pos)];
    }
    else{
        tgtAssert(pos.x < dimensions_.x, "x index out of bounds");
        tgtAssert(pos.y < dimensions_.y, "y index out of bounds");
        tgtAssert(pos.z < dimensions_.z, "z index out of bounds");
        return data_[calcPos(dimensions_, pos)];
    }
}

/// get voxel
template<class T>
inline const T& VolumeAtomic<T>::voxel(const tgt::svec3& pos) const {
    if(hasBorder()){
        tgtAssert(pos.x < dimensions_.x + borders_.llf.x + borders_.urb.x, "x index out of bounds");
        tgtAssert(pos.y < dimensions_.y + borders_.llf.y + borders_.urb.y, "y index out of bounds");
        tgtAssert(pos.z < dimensions_.z + borders_.llf.z + borders_.urb.z, "z index out of bounds");
        return data_[calcPos(dimensions_, borders_, pos)];
    }
    else{
        tgtAssert(pos.x < dimensions_.x, "x index out of bounds");
        tgtAssert(pos.y < dimensions_.y, "y index out of bounds");
        tgtAssert(pos.z < dimensions_.z, "z index out of bounds");
        return data_[calcPos(dimensions_, pos)];
    }
}

/// get or set voxel
template<class T>
inline T& VolumeAtomic<T>::voxel(size_t i) {
    if(hasBorder()){
        tgtAssert(i < numVoxels_, "index out of bounds");
        return data_[calcPos(dimensions_, borders_, i)];
    }
    else{
        tgtAssert(i < numVoxels_, "index out of bounds");
        return data_[i];
    }
}

/// get voxel
template<class T>
inline const T& VolumeAtomic<T>::voxel(size_t i) const {
    if(hasBorder()){
        tgtAssert(i < numVoxels_, "index out of bounds");
        return data_[calcPos(dimensions_, borders_, i)];
    }
    else{
        tgtAssert(i < numVoxels_, "index out of bounds");
        return data_[i];
    }
}


} // namespace voreen

#endif // VRN_VOLUMEATOMIC_H
