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

#ifndef VRN_VOLUMEREPRESENTATION_H
#define VRN_VOLUMEREPRESENTATION_H

#include "voreen/core/voreencoredefine.h"
#include "tgt/vector.h"
#include <vector>

namespace voreen {

class VRN_CORE_API VolumeRepresentation {
public:
    /// struct volume borders
    struct VolumeBorders{
        tgt::svec3 llf;
        tgt::svec3 urb;
        size_t numVoxels;
        bool hasBorder;

        VolumeBorders() : llf(tgt::svec3(0,0,0)), urb(tgt::svec3(0,0,0)), numVoxels(0), hasBorder(false){}
        VolumeBorders(size_t front, size_t back, size_t left, size_t right, size_t lower, size_t upper) : llf(tgt::svec3(front, left, lower)), urb(tgt::svec3(back, right, upper)){}
        VolumeBorders(const tgt::svec3& llfBorder, const tgt::svec3& urbBorder) : llf(llfBorder), urb(urbBorder){}
    };

    VolumeRepresentation(const tgt::svec3& dimensions);
    VolumeRepresentation(const VolumeRepresentation* volRep);
    VolumeRepresentation(const tgt::svec3& dimensions, const VolumeBorders& border);
    virtual ~VolumeRepresentation(){};

    /// Returns the VolumeRepresentation's borders
    virtual VolumeBorders getBorder() const;

    /// Returns the VolumeRepresentation's (lower, left, front) border
    virtual tgt::svec3 getBorderLLF() const;

    /// Returns the VolumeRepresentation's (upper, right, back) border
    virtual tgt::svec3 getBorderURB() const;

    /// Returns the VolumeRepresentation's dimensions in voxel coordinates (excluding border).
    virtual tgt::svec3 getDimensions() const;

    /// Returns the VolumeRepresentation's original dimensions in voxel coordinates (different if it is a brick).
    virtual tgt::svec3 getOriginalDimensions() const;

    /// Returns the VolumeRepresentation's dimensions in voxel coordinates (including border).
    virtual tgt::svec3 getDimensionsWithBorder() const;

    /// Returns the number of voxels contained by the VolumeRepresentation (excluding border).
    virtual size_t getNumVoxels() const;

    /// Returns the number of voxels contained by the VolumeRepresentation (including border).
    virtual size_t getNumVoxelsWithBorder() const;

    virtual int getNumChannels() const = 0;

    /// Returns true if the volume has a border
    virtual bool hasBorder() const;

protected:
    // protected default constructor
    VolumeRepresentation() {}

    tgt::svec3    dimensions_;
    tgt::svec3    originalDimensions_;
    VolumeBorders borders_;
    size_t        numVoxels_;

    static const std::string loggerCat_;
private:
    void updateNumVoxels();

    /// Updates the has border boolean
    void updateHasBorder();
};

//---------------------------------------------------------
//TODO: move to separate file...

class VRN_CORE_API RepresentationConverterBase {
public:
    virtual bool canConvert(const VolumeRepresentation* /*source*/) const = 0;
    virtual VolumeRepresentation* convert(const VolumeRepresentation* /*source*/) const = 0;
};

template<class T>
class RepresentationConverter : public RepresentationConverterBase {
};

class VRN_CORE_API ConverterFactory {
public:
    ConverterFactory();
    ~ConverterFactory();

    void addConverter(RepresentationConverterBase* conv) {
        converters_.push_back(conv);
        //TODO: check for duplicates using RTI
    }

    template<class T>
    RepresentationConverter<T>* findConverter(const VolumeRepresentation* source) {
        for(size_t i=0; i<converters_.size(); i++) {
            RepresentationConverter<T>* test = dynamic_cast<RepresentationConverter<T>*> (converters_[i]);

            if(test) {
                //TODO priorities
                if(test->canConvert(source))
                    return test;
            }
        }
        return 0;
    }

protected:
    std::vector<RepresentationConverterBase*> converters_;
};

} // namespace voreen

#endif
