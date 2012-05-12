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

#include "rawvoxvolumereader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <limits>

#include "tgt/exception.h"
#include "tgt/logmanager.h"
#include "tgt/vector.h"

#include "voreen/core/datastructures/volume/volumeatomic.h"

using tgt::svec3;
using tgt::vec3;

namespace voreen {

const std::string RawVoxVolumeReader::loggerCat_ = "voreen.io.VolumeReader.rawvox";

struct RawVoxHeader {
    int32_t magic_;

    int32_t sizeX_;
    int32_t sizeY_;
    int32_t sizeZ_;

    int32_t bitsPerVoxel_;
};

VolumeCollection* RawVoxVolumeReader::read(const std::string &url)
    throw (tgt::CorruptedFileException, tgt::IOException, std::bad_alloc)
{
    VolumeOrigin origin(url);
    std::string fileName = origin.getPath();

    LINFO("Reading file " << fileName);

    std::fstream fin(fileName.c_str(), std::ios::in | std::ios::binary);
    if (!fin.good())
        throw tgt::IOException();

    RawVoxHeader header;
    fin.read(reinterpret_cast<char*>(&header), sizeof(header));
    svec3 dimensions = svec3(header.sizeX_, header.sizeY_, header.sizeZ_);

    if(header.magic_ != 1381388120) {
        throw tgt::CorruptedFileException("Wrong magic number.");
    }

    Volume* dataset;
    switch(header.bitsPerVoxel_) {
        case 8:
            LINFO("Reading 8 bit dataset");
            dataset = new VolumeUInt8(dimensions);
            break;
        case 16:
            LINFO("Reading 16 bit dataset");
            dataset = new VolumeUInt16(dimensions);
            break;
        case 32:
            LINFO("Reading 32 bit (float) dataset");
            dataset = new VolumeFloat(dimensions);
            break;
        default:
            LERROR("Unknown bpp!");
            throw tgt::UnsupportedFormatException("Unexpected bpp.");
    }

    fin.read(reinterpret_cast<char*>(dataset->getData()), dataset->getNumBytes());

    if ( fin.eof() ) {
        delete dataset;
        throw tgt::CorruptedFileException();
    }

    fin.close();

    VolumeCollection* volumeCollection = new VolumeCollection();
    VolumeHandle* volumeHandle = new VolumeHandle(dataset, vec3(1.0f), vec3(0.0f));
    oldVolumePosition(volumeHandle);
    volumeHandle->setOrigin(fileName);
    volumeCollection->add(volumeHandle);

    return volumeCollection;
}

VolumeReader* RawVoxVolumeReader::create(ProgressBar* /*progress*/) const {
    return new RawVoxVolumeReader(/*progress*/);
}

} // namespace voreen
