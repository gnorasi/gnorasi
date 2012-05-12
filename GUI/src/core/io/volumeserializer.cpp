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

#include "voreen/core/io/volumeserializer.h"

#include <cstring>

#include "voreen/core/io/volumereader.h"
#include "voreen/core/io/volumewriter.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/utils/stringconversion.h"
#include "tgt/filesystem.h"

namespace voreen {

//------------------------------------------------------------------------------

VolumeSerializer::VolumeSerializer() {
}

VolumeSerializer::~VolumeSerializer() {
}

std::vector<VolumeReader*> VolumeSerializer::getReaders(const std::string& url) const
    throw (tgt::UnsupportedFormatException)
{
    if (url.empty()) 
        return readers_;
    
    VolumeOrigin origin(url);

    // check if a reader for the URL's resource type is available
    std::string protocol = origin.getProtocol();
    if (!protocol.empty()) {
        if (readersProtocolMap_.find(protocol) != readersProtocolMap_.end())
            return readersProtocolMap_.find(protocol)->second;
        else
            throw tgt::UnsupportedFormatException(protocol, url);
    }

    // check if a reader for the filename is available
    std::string filename = tgt::FileSystem::fileName(origin.getPath());
    if (readersFilenameMap_.find(filename) != readersFilenameMap_.end())
        return readersFilenameMap_.find(filename)->second;

    // check if a reader for the filename extension is available
    std::string extension = tgt::FileSystem::fileExtension(origin.getPath(), true);
    if (readersExtensionMap_.find(extension) != readersExtensionMap_.end())
        return readersExtensionMap_.find(extension)->second;
    else
        throw tgt::UnsupportedFormatException(extension, url);
}

std::vector<VolumeWriter*> VolumeSerializer::getWriters(const std::string& url) const
    throw (tgt::UnsupportedFormatException)
{
    if (url.empty()) 
        return writers_;

    VolumeOrigin origin(url);

    // check if a writer for the 's resource type is available
    std::string protocol = origin.getProtocol();
    if (!protocol.empty()) {
        if (writersProtocolMap_.find(protocol) != writersProtocolMap_.end())
            return writersProtocolMap_.find(protocol)->second;
        else
            throw tgt::UnsupportedFormatException(protocol, url);
    }

    // check if a reader for the filename is available
    std::string filename = tgt::FileSystem::fileName(origin.getPath());
    if (writersFilenameMap_.find(filename) != writersFilenameMap_.end())
        return writersFilenameMap_.find(filename)->second;

    // check if a reader for the filename extension is available
    std::string extension = tgt::FileSystem::fileExtension(origin.getPath(), true);
    if (writersExtensionMap_.find(extension) != writersExtensionMap_.end())
        return writersExtensionMap_.find(extension)->second;
    else
        throw tgt::UnsupportedFormatException(extension, url);
}

VolumeCollection* VolumeSerializer::read(const std::string& url) const
    throw (tgt::FileException, std::bad_alloc)
{
    std::vector<VolumeReader*> matchingReaders = getReaders(url);
    tgtAssert(!matchingReaders.empty(), "readers vector empty (exception expected)");
    if (matchingReaders.size() == 1) {
        return matchingReaders.front()->read(url);
    }
    else {
        // iterate over all possibly matching readers try to load data set, collect error messages
        std::vector<std::string> errors;
        for (size_t i=0; i<matchingReaders.size(); i++) {
            try {
                return matchingReaders.at(i)->read(url);
            }
            catch (const tgt::FileException& e) {
                errors.push_back(e.what());
            }
        }
        std::string errorMsg = "No VolumeReader was able to read the data set: " + strJoin(errors, "; ");
        throw tgt::FileException(errorMsg);
    }
}

VolumeCollection* VolumeSerializer::readSlices(const std::string& url, size_t firstSlice, size_t lastSlice) const
    throw (tgt::FileException, std::bad_alloc)
{
    std::vector<VolumeReader*> matchingReaders = getReaders(url);
    tgtAssert(!matchingReaders.empty(), "readers vector empty (exception expected)");
    if (matchingReaders.size() == 1) {
        return matchingReaders.front()->readSlices(url, firstSlice, lastSlice);
    }
    else {
        // iterate over all possibly matching readers try to load data set, collect error messages
        std::vector<std::string> errors;
        for (size_t i=0; i<matchingReaders.size(); i++) {
            try {
                return matchingReaders.at(i)->readSlices(url, firstSlice, lastSlice);
            }
            catch (const tgt::FileException& e) {
                errors.push_back(e.what());
            }
        }
        std::string errorMsg = "No VolumeReader was able to read the data set: " + strJoin(errors, "; ");
        throw tgt::FileException(errorMsg);
    }
}

VolumeCollection* VolumeSerializer::readBrick(const std::string& url, tgt::ivec3 brickStartPos,
    int brickSize) const throw (tgt::FileException, std::bad_alloc)
{
    std::vector<VolumeReader*> matchingReaders = getReaders(url);
    tgtAssert(!matchingReaders.empty(), "readers vector empty (exception expected)");
    if (matchingReaders.size() == 1) {
        return matchingReaders.front()->readBrick(url, brickStartPos, brickSize);
    }
    else {
        // iterate over all possibly matching readers try to load data set, collect error messages
        std::vector<std::string> errors;
        for (size_t i=0; i<matchingReaders.size(); i++) {
            try {
                return matchingReaders.at(i)->readBrick(url, brickStartPos, brickSize);
            }
            catch (const tgt::FileException& e) {
                errors.push_back(e.what());
            }
        }
        std::string errorMsg = "No VolumeReader was able to read the data set: " + strJoin(errors, "; ");
        throw tgt::FileException(errorMsg);
    }
}

VolumeHandleBase* VolumeSerializer::read(const VolumeOrigin& origin) const
    throw (tgt::FileException, std::bad_alloc)
{
    std::vector<VolumeReader*> matchingReaders = getReaders(origin.getURL());
    tgtAssert(!matchingReaders.empty(), "readers vector empty (exception expected)");
    if (matchingReaders.size() == 1) {
        return matchingReaders.front()->read(origin);
    }
    else {
        // iterate over all possibly matching readers try to load data set, collect error messages
        std::vector<std::string> errors;
        for (size_t i=0; i<matchingReaders.size(); i++) {
            try {
                return matchingReaders.at(i)->read(origin);
            }
            catch (const tgt::FileException& e) {
                errors.push_back(e.what());
            }
        }
        std::string errorMsg = "No VolumeReader was able to read the data set: " + strJoin(errors, "; ");
        throw tgt::FileException(errorMsg);
    }
}

void VolumeSerializer::write(const std::string& url, const VolumeHandleBase* volumeHandle) const throw (tgt::FileException)
{
    std::vector<VolumeWriter*> matchingWriters = getWriters(url);
    tgtAssert(!matchingWriters.empty(), "writers vector empty (exception expected)"); 
    if (matchingWriters.size() == 1) {
        return matchingWriters.front()->write(url, volumeHandle);
    }
    else {
        // iterate over all possibly matching writers try to load data set, collect error messages
        std::vector<std::string> errors;
        for (size_t i=0; i<matchingWriters.size(); i++) {
            try {
                return matchingWriters.at(i)->write(url, volumeHandle);
            }
            catch (const tgt::FileException& e) {
                errors.push_back(e.what());
            }
        }
        std::string errorMsg = "No VolumeWriter was able to write the data set: " + strJoin(errors, "; ");
        throw tgt::FileException(errorMsg);
    }
}

VolumeOrigin VolumeSerializer::convertOriginToRelativePath(const VolumeOrigin& origin, std::string& basePath) const
    throw (tgt::UnsupportedFormatException)
{
    std::vector<VolumeReader*> readers = getReaders(origin.getURL());
    tgtAssert(!readers.empty(), "readers vector must not be empty");
    return readers.front()->convertOriginToRelativePath(origin, basePath); // TODO: how to to select appropriate reader?
}

VolumeOrigin VolumeSerializer::convertOriginToAbsolutePath(const VolumeOrigin& origin, std::string& basePath) const
    throw (tgt::UnsupportedFormatException)
{
    std::vector<VolumeReader*> readers = getReaders(origin.getURL());
    tgtAssert(!readers.empty(), "readers vector must not be empty");
    return readers.front()->convertOriginToAbsolutePath(origin, basePath); // TODO: how to to select appropriate reader?
}

void VolumeSerializer::registerReader(VolumeReader* vr) {
    tgtAssert(vr, "null pointer passed");
    readers_.push_back(vr);

    // update extensions map
    const std::vector<std::string>& extensions = vr->getSupportedExtensions();
    for (size_t i = 0; i < extensions.size(); ++i) {
        // insert new entry (empty vector), if extension is not already mapped
        if (readersExtensionMap_.find(extensions[i]) == readersExtensionMap_.end()) {
            std::vector<VolumeReader*> readersVec;
            readersExtensionMap_.insert(std::make_pair(extensions[i], readersVec));
        }
        // add reader to mapped vector
        tgtAssert(readersExtensionMap_.find(extensions[i]) != readersExtensionMap_.end(), "map entry should exist here");
        readersExtensionMap_.find(extensions[i])->second.push_back(vr);
    }

    // update filename map
    const std::vector<std::string>& filenames = vr->getSupportedFilenames();
    for (size_t i = 0; i < filenames.size(); ++i) {
        // insert new entry (empty vector), if filename is not already mapped
        if (readersFilenameMap_.find(filenames[i]) == readersFilenameMap_.end()) {
            std::vector<VolumeReader*> readersVec;
            readersFilenameMap_.insert(std::make_pair(filenames[i], readersVec));
        }
        // add reader to mapped vector
        tgtAssert(readersFilenameMap_.find(filenames[i]) != readersFilenameMap_.end(), "map entry should exist here");
        readersFilenameMap_.find(filenames[i])->second.push_back(vr);
    }

    // update protocols map
    const std::vector<std::string>& formats = vr->getSupportedProtocols();
    for (size_t i = 0; i < formats.size(); ++i) {
        // insert new entry (empty vector), if extension is not already mapped
        if (readersProtocolMap_.find(formats[i]) == readersProtocolMap_.end()) {
            std::vector<VolumeReader*> readersVec;
            readersProtocolMap_.insert(std::make_pair(formats[i], readersVec));
        }
        // add reader to mapped vector
        tgtAssert(readersProtocolMap_.find(formats[i]) != readersProtocolMap_.end(), "map entry should exist here");
        readersProtocolMap_.find(formats[i])->second.push_back(vr);
    }
}

void VolumeSerializer::registerWriter(VolumeWriter* vw) {
    tgtAssert(vw, "null pointer passed");
    writers_.push_back(vw);

    // update extensions map
    const std::vector<std::string>& extensions = vw->getSupportedExtensions();
    for (size_t i = 0; i < extensions.size(); ++i) {
        // insert new entry (empty vector), if extension is not already mapped
        if (writersExtensionMap_.find(extensions[i]) == writersExtensionMap_.end()) {
            std::vector<VolumeWriter*> writersVec;
            writersExtensionMap_.insert(std::make_pair(extensions[i], writersVec));
        }
        // add writer to mapped vector
        tgtAssert(writersExtensionMap_.find(extensions[i]) != writersExtensionMap_.end(), "map entry should exist here");
        writersExtensionMap_.find(extensions[i])->second.push_back(vw);
    }

    // update filename map
    const std::vector<std::string>& filenames = vw->getSupportedFilenames();
    for (size_t i = 0; i < filenames.size(); ++i) {
        // insert new entry (empty vector), if filename is not already mapped
        if (writersFilenameMap_.find(filenames[i]) == writersFilenameMap_.end()) {
            std::vector<VolumeWriter*> writersVec;
            writersFilenameMap_.insert(std::make_pair(filenames[i], writersVec));
        }
        // add writer to mapped vector
        tgtAssert(writersFilenameMap_.find(filenames[i]) != writersFilenameMap_.end(), "map entry should exist here");
        writersFilenameMap_.find(filenames[i])->second.push_back(vw);
    }

    // update protocols map
    const std::vector<std::string>& formats = vw->getSupportedProtocols();
    for (size_t i = 0; i < formats.size(); ++i) {
        // insert new entry (empty vector), if protocol is not already mapped
        if (writersProtocolMap_.find(formats[i]) == writersProtocolMap_.end()) {
            std::vector<VolumeWriter*> writersVec;
            writersProtocolMap_.insert(std::make_pair(formats[i], writersVec));
        }
        // add writer to mapped vector
        tgtAssert(writersProtocolMap_.find(formats[i]) != writersProtocolMap_.end(), "map entry should exist here");
        writersProtocolMap_.find(formats[i])->second.push_back(vw);
    }
}

void VolumeSerializer::setProgressBar(ProgressBar* progressBar) {
    for (size_t i=0; i<readers_.size(); i++)
        readers_.at(i)->setProgressBar(progressBar);

    for (size_t i=0; i<writers_.size(); i++)
        writers_.at(i)->setProgressBar(progressBar);
}

} // namespace voreen
