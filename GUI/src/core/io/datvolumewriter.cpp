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

#include "voreen/core/io/datvolumewriter.h"
#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/datastructures/volume/volumehandle.h"

#include "tgt/filesystem.h"
#include "tgt/matrix.h"

namespace voreen {

const std::string DatVolumeWriter::loggerCat_("voreen.io.DatVolumeWriter");

DatVolumeWriter::DatVolumeWriter() {
    extensions_.push_back("dat");
}

void DatVolumeWriter::write(const std::string& filename, const VolumeHandleBase* volumeHandle)
    throw (tgt::IOException)
{

    tgtAssert(volumeHandle, "No volume handle");
    const Volume* volume = volumeHandle->getRepresentation<Volume>();
    if (!volume) {
        LWARNING("No volume");
        return;
    }

    std::string datname = filename;
    std::string rawname = getFileNameWithoutExtension(filename) + ".raw";
    LINFO("saving " << datname << " and " << rawname);

    std::fstream datout(datname.c_str(), std::ios::out);
    std::fstream rawout(rawname.c_str(), std::ios::out | std::ios::binary);

    if (!datout.is_open() || !rawout.is_open() || datout.bad() || rawout.bad())
        throw tgt::IOException();

    datout << getDatFileString(volumeHandle, rawname);
    if (datout.bad())
        throw tgt::IOException();
    datout.close();

    // write raw file
    const char* data = static_cast<const char*>(volume->getData());
    size_t numbytes = volume->getNumBytes();
    rawout.write(data, numbytes);
    if (rawout.bad())
        throw tgt::IOException();
    rawout.close();
}

std::string DatVolumeWriter::getDatFileString(const VolumeHandleBase* const volumeHandle, const std::string& rawFileName)
{
    std::ostringstream datout;
    tgtAssert(volumeHandle, "No volume handle");
    const Volume* volume = volumeHandle->getRepresentation<Volume>();
    if (!volume) {
        LWARNING("No volume or no storage for casted volume data!");
        return "";
    }

    // write dat file
    std::string format;
    std::string model = "I";

    if (dynamic_cast<const VolumeUInt8*>(volume)) {
        format = "UCHAR";
    }
    else if (dynamic_cast<const VolumeInt8*>(volume)) {
        format = "CHAR";
    }
    else if (const VolumeUInt16* vol = dynamic_cast<const VolumeUInt16*>(volume)) {
        if (vol->getBitsStored() == 12)
            format = "USHORT_12";
        else
            format = "USHORT";
    }
    else if (dynamic_cast<const VolumeInt16*>(volume)) {
        format = "SHORT";
    }
    else if (dynamic_cast<const VolumeUInt32*>(volume)) {
        format = "UINT";
    }
    else if (dynamic_cast<const VolumeInt32*>(volume)) {
        format = "INT";
    }
    else if (dynamic_cast<const VolumeUInt64*>(volume)) {
        format = "UINT64";
    }
    else if (dynamic_cast<const VolumeInt64*>(volume)) {
        format = "INT64";
    }
    else if (dynamic_cast<const VolumeFloat*>(volume)) {
        format = "FLOAT";
    }
    else if (dynamic_cast<const VolumeDouble*>(volume)) {
        format = "DOUBLE";
    }
    // vec2 types
    else if (dynamic_cast<const Volume2xUInt8*>(volume)) {
        format = "UCHAR";
        model = "LA"; //< luminance alpha
    }
    else if (dynamic_cast<const Volume2xInt8*>(volume)) {
        format = "CHAR";
        model = "LA"; //< luminance alpha
    }
    else if (const Volume2xUInt16* vol = dynamic_cast<const Volume2xUInt16*>(volume)) {
        if (vol->getBitsStored() == 12)
            format = "USHORT_12";
        else
            format = "USHORT";
        model = "LA"; //< luminance alpha

    }
    else if (dynamic_cast<const Volume2xInt16*>(volume)) {
        format = "SHORT";
        model = "LA"; //< luminance alpha
    }
    else if (dynamic_cast<const Volume2xUInt32*>(volume)) {
        format = "UINT";
        model = "LA"; //< luminance alpha
    }
    else if (dynamic_cast<const Volume2xInt32*>(volume)) {
        format = "INT";
        model = "LA"; //< luminance alpha
    }
    else if (dynamic_cast<const Volume2xUInt64*>(volume)) {
        format = "UINT64";
        model = "LA"; //< luminance alpha
    }
    else if (dynamic_cast<const Volume2xInt64*>(volume)) {
        format = "INT64";
        model = "LA"; //< luminance alpha
    }
    else if (dynamic_cast<const Volume2xFloat*>(volume)) {
        format = "FLOAT";
        model = "LA"; //< luminance alpha
    }
    else if (dynamic_cast<const Volume2xDouble*>(volume)) {
        format = "DOUBLE";
        model = "LA"; //< luminance alpha
    }
    // vec3 types
    else if (dynamic_cast<const Volume3xUInt8*>(volume)) {
        format = "UCHAR";
        model = "RGB"; 
    }
    else if (dynamic_cast<const Volume3xInt8*>(volume)) {
        format = "CHAR";
        model = "RGB"; 
    }
    else if (const Volume3xUInt16* vol = dynamic_cast<const Volume3xUInt16*>(volume)) {
        if (vol->getBitsStored() == 12)
            format = "USHORT_12";
        else
            format = "USHORT";
        model = "RGB"; 

    }
    else if (dynamic_cast<const Volume3xInt16*>(volume)) {
        format = "SHORT";
        model = "RGB"; 
    }
    else if (dynamic_cast<const Volume3xUInt32*>(volume)) {
        format = "UINT";
        model = "RGB"; 
    }
    else if (dynamic_cast<const Volume3xInt32*>(volume)) {
        format = "INT";
        model = "RGB"; 
    }
    else if (dynamic_cast<const Volume3xUInt64*>(volume)) {
        format = "UINT64";
        model = "RGB"; 
    }
    else if (dynamic_cast<const Volume3xInt64*>(volume)) {
        format = "INT64";
        model = "RGB"; 
    }
    else if (dynamic_cast<const Volume3xFloat*>(volume)) {
        format = "FLOAT";
        model = "RGB"; 
    }
    else if (dynamic_cast<const Volume3xDouble*>(volume)) {
        format = "DOUBLE";
        model = "RGB"; 
    }
    // vec4 types
    else if (dynamic_cast<const Volume4xUInt8*>(volume)) {
        format = "UCHAR";
        model = "RGBA"; 
    }
    else if (dynamic_cast<const Volume4xInt8*>(volume)) {
        format = "CHAR";
        model = "RGBA"; 
    }
    else if (const Volume4xUInt16* vol = dynamic_cast<const Volume4xUInt16*>(volume)) {
        if (vol->getBitsStored() == 12)
            format = "USHORT_12";
        else
            format = "USHORT";
        model = "RGBA"; 

    }
    else if (dynamic_cast<const Volume4xInt16*>(volume)) {
        format = "SHORT";
        model = "RGBA"; 
    }
    else if (dynamic_cast<const Volume4xUInt32*>(volume)) {
        format = "UINT";
        model = "RGBA"; 
    }
    else if (dynamic_cast<const Volume4xInt32*>(volume)) {
        format = "INT";
        model = "RGBA"; 
    }
    else if (dynamic_cast<const Volume4xUInt64*>(volume)) {
        format = "UINT64";
        model = "RGBA"; 
    }
    else if (dynamic_cast<const Volume4xInt64*>(volume)) {
        format = "INT64";
        model = "RGBA"; 
    }
    else if (dynamic_cast<const Volume4xFloat*>(volume)) {
        format = "FLOAT";
        model = "RGBA"; 
    }
    else if (dynamic_cast<const Volume4xDouble*>(volume)) {
        format = "DOUBLE";
        model = "RGBA"; 
    }
    // special types
    else if (dynamic_cast<const VolumeMat3Float*>(volume)) {
        format = "FLOAT";
        model = "MAT3";
    }
    else if (dynamic_cast<const VolumeTensor2Float*>(volume)) {
        format = "FLOAT";
        model = "TENSOR_UP";
    } 
    else
        LERROR("Format currently not supported");

    datout << "ObjectFileName:\t" << tgt::FileSystem::fileName(rawFileName) << std::endl;

    tgt::ivec3 dimensions = volume->getDimensions();
    datout << "Resolution:\t" << dimensions.x << " " << dimensions.y << " " << dimensions.z << std::endl;

    tgt::vec3 spacing = volumeHandle->getSpacing();
    datout << "SliceThickness:\t" << spacing.x << " " << spacing.y << " " << spacing.z << std::endl;

    datout << "Format:\t\t" << format << std::endl;
    datout << "ObjectModel:\t" << model << std::endl;
    datout << "Modality:\t" << volumeHandle->getModality() << std::endl;
    datout << "Checksum:\t" << volumeHandle->getHash() << std::endl;

    // write transformation matrix unless it is the identity matrix
    tgt::mat4 transformation = volumeHandle->getPhysicalToWorldMatrix();
    if (transformation != tgt::mat4::createIdentity())
        datout << "TransformMatrix: row0\t" << transformation[0][0] << " " << transformation[0][1] << " "
               << transformation[0][2] << " " << transformation[0][3] << std::endl
               << "TransformMatrix: row1\t" << transformation[1][0] << " " << transformation[1][1] << " "
               << transformation[1][2] << " " << transformation[1][3] << std::endl
               << "TransformMatrix: row2\t" << transformation[2][0] << " " << transformation[2][1] << " "
               << transformation[2][2] << " " << transformation[2][3] << std::endl
               << "TransformMatrix: row3\t" << transformation[3][0] << " " << transformation[3][1] << " "
               << transformation[3][2] << " " << transformation[3][3] << std::endl;

    return datout.str();
}

VolumeWriter* DatVolumeWriter::create(ProgressBar* /*progress*/) const {
    return new DatVolumeWriter(/*progress*/);
}

} // namespace voreen
