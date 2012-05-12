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

#include "multivolumereader.h"

#include <fstream>
#include <iostream>

#include "voreen/core/io/volumeserializer.h"
#include "voreen/core/io/volumeserializerpopulator.h"
#include "voreen/core/io/serialization/meta/primitivemetadata.h"

using std::string;

namespace {
    std::string extractPurePathFromFileName(const std::string& fileName) {
        if (fileName.rfind("/") != string::npos)
            return fileName.substr(0, fileName.rfind("/") + 1);
        else if (fileName.rfind("\\") != string::npos)
            return fileName.substr(0, fileName.rfind("\\") + 1);
        else
            return "/";
    }
}
namespace voreen {

const string MultiVolumeReader::loggerCat_ = "voreen.base.MultiVolumeReader";

MultiVolumeReader::MultiVolumeReader(VolumeSerializerPopulator* populator, ProgressBar* progress)
    : VolumeReader(progress),
      populator_(populator)
{
    extensions_.push_back("mv");
    protocols_.push_back("mv");
}

VolumeReader* MultiVolumeReader::create(ProgressBar* progress) const {
    return new MultiVolumeReader(0, progress);
}

VolumeCollection* MultiVolumeReader::read(const std::string& url)
    throw (tgt::FileException, std::bad_alloc)
{
    LINFO("Loading multi volume file " << url);
    VolumeOrigin urlOrigin(url);

    std::vector<VolumeOrigin> origins = listVolumes(url);
    if (origins.empty())
        throw tgt::FileException("No volumes listed in multi-volume file", url);

    VolumeCollection* volumeCollection = new VolumeCollection();

    std::string refFile = urlOrigin.getSearchParameter("file"); 
    if (refFile == "") {
        // no particular file specified in URL => load all listed ones
        for (size_t i=0; i<origins.size(); i++) {
            VolumeHandleBase* handle = read(origins.at(i));
            if (handle)
                volumeCollection->add(handle);
        }
    }
    else {
        // load specified file
        for (size_t i=0; i<origins.size(); i++) {
            if (origins.at(i).getSearchParameter("file") == refFile) {
                VolumeHandleBase* handle = read(origins.at(i));
                if (handle) {
                    volumeCollection->add(handle);
                    break;
                }
            }
        }

        if (volumeCollection->empty()) {
            delete volumeCollection;
            throw tgt::FileException("File '" + refFile + "' not listed in multi-volume file", urlOrigin.getPath());
        }
    }
    
    return volumeCollection;
}

VolumeHandleBase* MultiVolumeReader::read(const VolumeOrigin& origin) 
    throw (tgt::FileException, std::bad_alloc) 
{
    std::string path = extractPurePathFromFileName(origin.getPath());
    std::string refFile = origin.getSearchParameter("file");
    if (refFile == "") 
        throw tgt::FileException("'file' parameter missing in URL", origin.getURL());
 
    VolumeOrigin refOrigin(path + refFile);
    LINFO("Loading file " + refOrigin.getPath());
    VolumeHandleBase* handle = 0;
    if (populator_)
        handle = populator_->getVolumeSerializer()->read(refOrigin);
    else {
        VolumeSerializerPopulator populator;
        handle = populator.getVolumeSerializer()->read(refOrigin);
    }
    if (handle)
        handle->setOrigin(origin);

    return handle;
}

std::vector<VolumeOrigin> MultiVolumeReader::listVolumes(const std::string& url) const 
        throw (tgt::FileException)
{
    std::vector<VolumeOrigin> result;
    
    VolumeOrigin urlOrigin(url);
    std::string fileName = urlOrigin.getPath();

    std::fstream fin(fileName.c_str(), std::ios::in | std::ios::binary);
    if (!fin.good() || fin.eof() || !fin.is_open())
        throw tgt::FileNotFoundException("Unable to open multi volume file for reading", fileName);

    do {
        string line;
        getline(fin, line);

        if (line.empty())
            continue;

        // If the line was delimited by a '\r\n' the '\r' will still be the last character
        // so remove it
        if (line[line.length()-1] == char(13))
            line = line.substr(0, line.length()-1);

        VolumeOrigin origin("mv", fileName);
        origin.addSearchParameter("file", line);
        origin.getMetaDataContainer().addMetaData("File Name", new StringMetaData(line));
        result.push_back(origin);
    } while (!fin.eof());

    fin.close();

    return result;
}

} // namespace voreen
