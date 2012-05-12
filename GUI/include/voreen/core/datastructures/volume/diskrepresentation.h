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

#ifndef VRN_DISKREPRESENTATION_H
#define VRN_DISKREPRESENTATION_H

#include <typeinfo>
#include <vector>

#include "voreen/core/datastructures/volume/volume.h"

namespace voreen {

/// A representation storing the information to do a lazy loading of the volume data.
class DiskRepresentation : public VolumeRepresentation {
public:
    /** 
     * @param filename Absolute file name.
     * @param format @see VolumeFactory
     * @param offset If offset >= 0: Bytes offset from beginning of file. If offset < 0: assume data is aligned to end of file.
     */
    DiskRepresentation(const std::string& filename, const std::string& format, tgt::ivec3 dimensions, int64_t offset = 0, const VolumeRepresentation::VolumeBorders& border = VolumeRepresentation::VolumeBorders());
    DiskRepresentation(const DiskRepresentation* diskrep);

    virtual ~DiskRepresentation();

    std::string getFileName() const { return filename_; }
    ///@see VolumeFactory
    std::string getFormat() const { return format_; }
    ///Not implemented TODO
    virtual int getNumChannels() const;
    ///Creates new disk representation based on current disk representation.
    virtual DiskRepresentation* getSubVolume(tgt::svec3 dimensions, tgt::svec3 offset = tgt::svec3(0,0,0), const VolumeRepresentation::VolumeBorders& border = VolumeRepresentation::VolumeBorders()) const throw (std::bad_alloc);
    ///Offset in the file (in bytes).
    int64_t getOffset() const { return offset_; }
protected:
    std::string filename_;
    std::string format_;
    int64_t offset_;

    static const std::string loggerCat_;
};

/// Creates a Volume from a DiskRepresentation.
class RepresentationConverterLoadFromDisk : public RepresentationConverter<Volume> {
public:
    virtual bool canConvert(const VolumeRepresentation* source) const;
    virtual VolumeRepresentation* convert(const VolumeRepresentation* source) const;
};

} // namespace voreen

#endif
