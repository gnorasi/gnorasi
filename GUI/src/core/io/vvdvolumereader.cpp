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

#include "voreen/core/io/vvdvolumereader.h"
#include "voreen/core/io/vvdformat.h"

#include <fstream>
#include <iostream>

#include "tgt/exception.h"
#include "tgt/vector.h"

#include "voreen/core/utils/stringconversion.h"
#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/datastructures/volume/diskrepresentation.h"

using tgt::vec3;
using tgt::ivec3;
using tgt::hor;
using tgt::lessThanEqual;

namespace voreen {

const std::string VvdVolumeReader::loggerCat_ = "voreen.io.VolumeReader.dat";

VvdVolumeReader::VvdVolumeReader(ProgressBar* progress)
    : VolumeReader(progress)
{
    extensions_.push_back("vvd");
}

VolumeCollection* VvdVolumeReader::read(const std::string &url)
    throw (tgt::FileException, std::bad_alloc)
{
    VolumeOrigin origin(url);
    std::string fileName = origin.getPath();

    // open file for reading
    std::fstream fileStream(fileName.c_str(), std::ios_base::in);
    if (fileStream.fail()) {
        throw tgt::FileException("Failed to open file '" + tgt::FileSystem::absolutePath(fileName) + "' for reading.");
    }

    // read data stream into deserializer
    XmlDeserializer d(fileName);
    d.setUseAttributes(true);
    try {
        d.read(fileStream);
    }
    catch (SerializationException& e) {
        throw tgt::FileException("SerializationException: Failed to read serialization data stream from file '"
                                     + fileName + "': " + e.what());
    }
    catch (...) {
        throw tgt::FileException("Failed to read serialization data stream from file '"
                                     + fileName + "' (unknown exception).");
    }

    std::vector<VvdObject> vec;
    // deserialize from data stream
    try {
        d.deserialize("Volumes", vec, "Volume");
    }
    catch (std::exception& e) {
        throw tgt::FileException("Deserialization from file '" + fileName + "' failed: " + e.what());
    }
    catch (...) {
        throw tgt::FileException("Deserialization from file '" + fileName + "' failed (unknown exception).");
    }

    VolumeCollection* vc = new VolumeCollection();
    for(size_t i=0; i<vec.size(); i++) {
        VolumeHandle* vh = vec[i].createVolume(tgt::FileSystem::dirName(fileName));
        vh->setOrigin(origin);
        vc->add(vh);
    }

    return vc;
}

VolumeReader* VvdVolumeReader::create(ProgressBar* progress) const {
    return new VvdVolumeReader(progress);
}

} // namespace voreen
