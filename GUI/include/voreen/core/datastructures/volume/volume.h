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

#ifndef VRN_VOLUME_H
#define VRN_VOLUME_H

// Note: please ensure that no OpenGL dependencies are added to this file

#include "voreen/core/datastructures/volume/volumerepresentation.h"
#include "voreen/core/datastructures/geometry/meshgeometry.h"

namespace voreen {

/**
 * OpenGL-independent base class for volumetric data sets.
 *
 * This class stores the raw data as well as the
 * required meta information about it. It does,
 * however, neither perform any OpenGL operations nor
 * does it hold any OpenGL-related properties.
 *
 * @see VolumeGL
 */
class VRN_CORE_API Volume : public VolumeRepresentation {
public:
    enum Filter {
        NEAREST,
        LINEAR,
        CUBIC
    };

    /**
     * Constructor.
     *
     * @param dimensions dimensions of the new dataset
     * @param bitsStored bit depth of the new dataset
     * @param spacing dimensions of each voxel of the new dataset
     */
    Volume(const tgt::svec3& dimensions, int bitsStored, const VolumeRepresentation::VolumeBorders& border);
    Volume(const Volume* vol);

    virtual ~Volume() {}

    /// Use this as a kind of a virtual constructor.
    virtual Volume* clone() const throw (std::bad_alloc) = 0;

    /**
     * Use this as a kind of a virtual constructor that does _NOT_ copy over
     * the voxels but uses the given pointer instead as a voxel data. If this
     * pointer is 0, an empty volume without voxel data is created.
     */
    virtual Volume* clone(void* data) const throw (std::bad_alloc) = 0;
    virtual Volume* createNew(const tgt::svec3& dimensions, 
                              const VolumeRepresentation::VolumeBorders& border = VolumeRepresentation::VolumeBorders(),
                              bool allocMem = false) const throw (std::bad_alloc) = 0;

    /// Create new volume which contains part of the data of the current volume.
    virtual Volume* getSubVolume(const tgt::svec3& dimensions, 
                                 const tgt::svec3& offset = tgt::svec3(0,0,0), 
                                 const VolumeRepresentation::VolumeBorders& border = VolumeRepresentation::VolumeBorders()) const throw (std::bad_alloc) = 0;

    /// Exchange part of volume
    virtual void setSubVolume(const Volume* vol, const tgt::svec3& offset = tgt::svec3(0,0,0)) = 0;

    /// Returns the number of bytes held in the \a data_ array.
    virtual size_t getNumBytes() const = 0;

    /// Returns the number of channels of this volume.
    virtual int getNumChannels() const = 0;

    /// Set the volume's bit depth.
    virtual void setBitsStored(int bits);

    /// Returns the volume's bit depth.
    virtual int getBitsStored() const;

    /// Returns the number of bits that are allocated by each voxel.
    /// For technical reasons, it may exceed the volume's bit depth.
    virtual int getBitsAllocated() const = 0;

    /// Returns the number of bytes that are allocated for each voxel.
    virtual int getBytesPerVoxel() const = 0;

    //------------------------------------------------------
    //TODO: Make derived data

    /**
     * Returns the minimum value contained by the specified channel converted to float.
     */
    virtual float minValue(size_t channel = 0) const;

    /**
     * Returns the maximum value contained by the specified channel converted to float.
     */
    virtual float maxValue(size_t channel = 0) const;

    //------------------------------------------------------
    
    /**
     * Returns the data set's minimal and maximal possible element values
     * according to its data type converted to float.
     */
    virtual tgt::vec2 elementRange() const = 0;

    /**
     * Returns the voxel of a given postion and channel, converted to a float.
     *
     * BEWARE: Since this method is virtual it can be considered as slow.
     *
     * @param pos the position of the voxel
     * @param channel the channel of the voxel
     */
    virtual float getVoxelFloat(const tgt::svec3& pos, size_t channel = 0) const = 0;

    /**
     * Returns the voxel of a given postion and channel, converted to a float.
     *
     * BEWARE: Since this method is virtual it can be considered as slow.
     *
     * @param x the x-coordinate of the voxel
     * @param y the y-coordinate of the voxel
     * @param z the z-coordinate of the voxel
     * @param channel the channel of the voxel
     */
    virtual float getVoxelFloat(size_t x, size_t y, size_t z, size_t channel = 0) const = 0;

    /**
     * Returns the voxel of a given position and channel, converted to a float.
     *
     * BEWARE: Since this method is virtual it can be considered as slow.
     *
     * @param index the index of the voxel
     * @param channel the channel of the voxel
     */
    virtual float getVoxelFloat(size_t index, size_t channel = 0) const = 0;

    /**
     * Sets the voxel of a given postion and channel, converted from a float.
     *
     * BEWARE: Since this method is virtual it can be considered as slow.
     *
     * @param value The float value to be set.
     * @param pos the position of the voxel
     * @param channel the channel of the voxel
     */
    virtual void setVoxelFloat(float value, const tgt::svec3& pos, size_t channel = 0) = 0;

    /**
     * Sets the voxel of a given postion and channel, converted from a float.
     *
     * BEWARE: Since this method is virtual it can be considered as slow.
     *
     * @param value The float value to be set.
     * @param x the x-coordinate of the voxel
     * @param y the y-coordinate of the voxel
     * @param z the z-coordinate of the voxel
     * @param channel the channel of the voxel
     */
    virtual void setVoxelFloat(float value, size_t x, size_t y, size_t z, size_t channel = 0) = 0;

    /**
     * Sets the voxel of a given postion and channel, converted from a float.
     *
     * BEWARE: Since this method is virtual it can be considered as slow.
     *
     * @param value The float value to be set.
     * @param index the index of the voxel
     * @param channel the channel of the voxel
     */
    virtual void setVoxelFloat(float value, size_t index, size_t channel = 0) = 0;

    /**
     * Returns the trilinearly interpolated voxel of a given
     * postion and channel, converted to a float.
     *
     * BEWARE: Since this method is virtual it can be considered as slow.
     *
     * @param pos the position of the voxel
     * @param channel the channel of the voxel
     */
    virtual float getVoxelFloatLinear(const tgt::vec3& pos, size_t channel = 0) const;

    /**
     * Returns the tricubic interpolated voxel of a given position and channel,
     * converted to a normalized float. A Catmull-Rom spline is used for the interpolation.
     *
     * BEWARE: Since this method is virtual it can be considered as slow.
     *
     * @param pos the position of the voxel
     * @param channel the channel of the voxel
     */
    virtual float getVoxelFloatCubic(const tgt::vec3& pos, size_t channel = 0) const;

    /**
     * Helper function that returns the voxel value at the passed voxel position as string (nearest filtering). 
     *
     * The default implementation returns for scalar volumes the "stringified" scalar value. 
     * For vector types the tgt::Vector stream operator is used, which yields "[x y z]" for Vector3 types, for example. 
     * If the passed voxel position lies outside the volume dimensions, an empty string is returned.
     *
     * @note Non-standard volume types should override this function.
     */
    virtual std::string getVoxelValueAsString(const tgt::svec3& voxelPos) const;

    /// Set all volume data to zero
    virtual void clear() = 0;

    /// Gets a void* to the data stored with this Volume
    virtual const void* getData() const = 0;
    virtual void* getData() = 0;

    /**
     * Use this as type safe wrapper in order to get a proper typed pointer.
     */
    template<class T>
    inline static typename T::VoxelType* getData(T* v);
protected:
    // protected default constructor
    Volume() {}

    static const std::string loggerCat_;

private:
    int         bitsStored_;
};


/**
 * You can use this macro if you want to iterate over all voxels and it is
 * important for you that you do it dimensionwise, for instance: <br>
\code
VRN_FOR_EACH_VOXEL(i, svec3(0, 0, 0), svec(15, 20, 30))
    voxel(i) = i.x * i.y + i.z;
\endcode
 * or like this:
\code
VRN_FOR_EACH_VOXEL(i, svec3(0, 0, 0), svec(15, 20, 30)) {
    voxel(i) = i.x * i.y + i.z;
    foo();
}
\endcode
 * If you want to do sth just with each voxel use this simple for-loop
\code
for (size_t i = 0; i \< numVoxels_; ++i)
     voxel(i) = i;
\endcode
*/
#define VRN_FOR_EACH_VOXEL(INDEX, POS, SIZE) \
    for (tgt::svec3 (INDEX) = (POS); (INDEX).z < (SIZE).z; ++(INDEX).z)\
        for ((INDEX).y = (POS).y; (INDEX).y < (SIZE).y; ++(INDEX).y)\
            for ((INDEX).x = (POS).x; (INDEX).x < (SIZE).x; ++(INDEX).x)

} // namespace voreen

#endif // VRN_VOLUME_H
