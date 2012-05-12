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

#include "nrrdvolumewriter.h"
#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "tgt/filesystem.h"

namespace voreen {

const std::string NrrdVolumeWriter::loggerCat_ = "voreen.base.NrrdVolumeWriter";

NrrdVolumeWriter::NrrdVolumeWriter() {
    extensions_.push_back("nrrd");
    extensions_.push_back("nhdr");
}

void NrrdVolumeWriter::write(const std::string& filename, const VolumeHandleBase* volumeHandle)
    throw (tgt::IOException)
{

    tgtAssert(volumeHandle, "No volume handle");
    const Volume* volume = volumeHandle->getRepresentation<Volume>();
    if (!volume) {
        LWARNING("No volume");
        return;
    }

    std::string nhdrname = filename;
    std::string rawname = getFileNameWithoutExtension(filename) + ".raw";
    LINFO("saving " << nhdrname << " and " << rawname);

    std::fstream nhdrout(nhdrname.c_str(), std::ios::out);
    std::fstream rawout(rawname.c_str(), std::ios::out | std::ios::binary);

    if (nhdrout.bad() || rawout.bad()) {
        LWARNING("Can't open file");
        throw tgt::IOException();
    }

    // write nrrd header
    std::string type;
    const char* data = 0;
    size_t numbytes = 0;

    if (const VolumeUInt8* vol = dynamic_cast<const VolumeUInt8*>(volume)) {
        type = "uchar";
        data = reinterpret_cast<const char*>(vol->voxel());
        numbytes = vol->getNumBytes();
    }
    else if (const VolumeUInt16* vol = dynamic_cast<const VolumeUInt16*>(volume)) {
        type = "ushort";
        data = reinterpret_cast<const char*>(vol->voxel());
        numbytes = vol->getNumBytes();
    }
    else if (const Volume4xUInt8* vol = dynamic_cast<const Volume4xUInt8*>(volume)) {
        type = "uint";
        data = reinterpret_cast<const char*>(vol->voxel());
        numbytes = vol->getNumBytes();
    }
    else
        LERROR("Format currently not supported");

    tgt::ivec3 dimensions = volumeHandle->getDimensions();
    tgt::vec3 spacing = volumeHandle->getSpacing();

    nhdrout << "NRRD0001" << std::endl; // magic number
    nhdrout << "content:      " << tgt::FileSystem::fileName(filename) << std::endl;
    nhdrout << "dimension:    3" << std::endl;
    nhdrout << "type:         " << type << std::endl;
    nhdrout << "sizes:        " << dimensions.x << " " << dimensions.y << " " << dimensions.z << std::endl;
    nhdrout << "spacings:     " << spacing.x << " " << spacing.y << " " << spacing.z << std::endl;
    nhdrout << "datafile:     " << tgt::FileSystem::fileName(rawname) << std::endl;
    nhdrout << "encoding:     raw" << std::endl;

    nhdrout.close();

    // write raw file
    rawout.write(data, numbytes);
    rawout.close();
}

VolumeWriter* NrrdVolumeWriter::create(ProgressBar* /*progress*/) const {
    return new NrrdVolumeWriter(/*progress*/);
}

}; //namespace voreen
