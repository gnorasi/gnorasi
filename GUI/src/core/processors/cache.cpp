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

#include "voreen/core/processors/cache.h"

#include "voreen/core/properties/property.h"
#include "voreen/core/ports/port.h"

#include "voreen/core/utils/stringconversion.h"
#include "voreen/core/utils/hashing.h"
#include "tgt/filesystem.h"

#include <stdio.h>

namespace voreen {

const std::string Cache::loggerCat_("voreen.Cache");

Cache::Cache(Processor* proc) : processor_(proc), initialized_(false) {
    tgtAssert(proc, "Null processor!");
}

void Cache::initialize() {
    std::string interfaceStr = getInterfaceString();
    std::string fname = processor_->getCachePath() + "/interfaceStr.txt";

    if (FileSys.dirExists(processor_->getCachePath())) {
        if(!FileSys.fileExists(fname) || !stringEqualsFileContent(interfaceStr, fname)) {
            LERROR("Interface changed! Clearing cache.");
            FileSys.deleteDirectoryRecursive(processor_->getCachePath());
        }
    }

    //if cache was broken it has been deleted, re-create:
    if (!FileSys.dirExists(processor_->getCachePath())) {
        if(!FileSys.createDirectoryRecursive(processor_->getCachePath()))
            return;

        std::fstream out(fname.c_str(), std::ios::out | std::ios::binary);

        if (out.is_open() || !out.bad()) {
            out.write(interfaceStr.c_str(), interfaceStr.length());
        }
        else {
            LERROR("Could not write interfaceStr file!");
            return;
        }

        out.close();
    }

    initialized_ = true;
}

bool Cache::stringEqualsFileContent(std::string str, std::string fname) {
    tgt::File* f = FileSys.open(fname);
    std::string cmp;
    if(f) {
        cmp = f->getAsString();
    }
    else
        return false;
    delete f;

    return (str == cmp);
}

std::string Cache::getInterfaceString() {
    std::string interfaceStr;

    interfaceStr += "Inports:\n";
    for(size_t i=0; i<inports_.size(); i++) {
        interfaceStr += inports_[i] + "\n";
    }
    interfaceStr += "\n";

    interfaceStr += "Outports:\n";
    for(size_t i=0; i<outports_.size(); i++) {
        interfaceStr += outports_[i] + "\n";
    }
    interfaceStr += "\n";

    interfaceStr += "Properites:\n";
    for(size_t i=0; i<properties_.size(); i++) {
        Property* prop = processor_->getProperty(properties_[i]);
        if(!prop) {
            LWARNING("Property " << properties_[i] << " is not registered at the processor!");
        }

        interfaceStr += prop->getID() + " [" + prop->getClassName() + "]\n";
    }

    return interfaceStr;
}

void Cache::addInport(Port* inport) {
    tgtAssert(!initialized_, "Cache is initialized!");
    if (initialized_)
        return;

    if (inport->supportsCaching()) {
        inports_.push_back(inport->getName());
    }
    else {
        LWARNING("Port '" << inport->getName() << "' does not support caching");
    }
}

void Cache::addAllInports() {
    tgtAssert(!initialized_, "Cache is initialized!");
    if(initialized_)
        return;

    const std::vector<Port*>& inports = processor_->getInports();
    for(size_t i=0; i<inports.size(); i++) {
        addInport(inports[i]);
    }
}

void Cache::addOutport(Port* outport) {
    tgtAssert(!initialized_, "Cache is initialized!");
    if (initialized_)
        return;

    if (outport->supportsCaching()) {
        outports_.push_back(outport->getName());
    }
    else {
        LWARNING("Port '" << outport->getName() << "' does not support caching");
    }
}

void Cache::addAllOutports() {
    tgtAssert(!initialized_, "Cache is initialized!");
    if(initialized_)
        return;

    const std::vector<Port*>& outports = processor_->getOutports();
    for (size_t i=0; i<outports.size(); i++) {
        addOutport(outports[i]);
    }
}

void Cache::addProperty(Property* prop) {
    tgtAssert(!initialized_, "Cache is initialized!");

    if(!initialized_)
        properties_.push_back(prop->getID());
}

void Cache::addAllProperties() {
    tgtAssert(!initialized_, "Cache is initialized!");

    if(initialized_)
        return;

    const std::vector<Property*>& properties = processor_->getProperties();
    for(size_t i=0; i<properties.size(); i++) {
        if(properties[i]->getID() == "useCaching")
            continue;
        if(properties[i]->getID() == "clearCache")
            continue;

        addProperty(properties[i]);
    }
}

std::string Cache::getAllInportHashes() {
    std::string s;
    for(size_t i=0; i<inports_.size(); i++) {
        Port* p = processor_->getPort(inports_[i]);
        std::string hash = p->getHash();
        if (!hash.empty())
            s += "/" + hash;
        else
            s += "/disconnected";
    }
    return s;
}

std::string Cache::getPropertyState() {
    // create temporary property map for serialization
    std::map<std::string, Property*> propertyMap;
    for (size_t i=0; i < properties_.size(); ++i) {
        Property* p = processor_->getProperty(properties_[i]);
        if(p)
            propertyMap[properties_[i]] = p;
    }

    // serialize properties
    XmlSerializer s;

    const bool usePointerContentSerialization = s.getUsePointerContentSerialization();
    s.setUsePointerContentSerialization(true);
    try {
        s.serialize("Properties", propertyMap, "Property", "name");
    }
    catch (SerializationException& e) {
        LWARNING(e.what());
    }
    s.setUsePointerContentSerialization(usePointerContentSerialization);

    std::stringstream stream;
    s.write(stream);
    return stream.str();
}

std::string Cache::getPropertyStateHash() {
    return VoreenHash::getHash(getPropertyState());
}

std::string Cache::getCurrentCacheDir() {
    return processor_->getCachePath() + getAllInportHashes() + "/" + getPropertyStateHash() + "/";
}

bool Cache::restoreOutportsFromDir(const std::string& dir) {
    if (!initialized_)
        return false;

    if (!FileSys.dirExists(dir)) {
        return false;
    }

    for(size_t i=0; i<outports_.size(); i++) {
        Port* p = processor_->getPort(outports_[i]);
        if (!p) {
            LERROR("Could not find port " << outports_[i]);
            return false;
        }

        std::string portName = p->getName();
        // remove points from port name since it is used as filename
        portName = strReplaceAll(portName, ".", "_");

        if(!FileSys.fileExists(dir + portName + ".empty")) {
            try {
                p->loadData(dir + portName);
            }
            catch (VoreenException& e) {
                LERROR("Failed to restore data for port '" << p->getName() << "': "
                        << e.what());
                return false;
            }
        }
    }

    return true;
}

bool Cache::storeOutportsToDir(const std::string& dir) {
    if (!initialized_)
        return false;

    if (!FileSys.createDirectoryRecursive(dir)) {
        LERROR("Failed to create dir!");
        return false;
    }

    for(size_t i=0; i<outports_.size(); i++) {
        Port* p = processor_->getPort(outports_[i]);
        if (!p) {
            LERROR("Could not find port " << outports_[i]);
            FileSys.deleteDirectoryRecursive(dir);
            return false;
        }

        std::string portName = p->getName();
        // remove points from port name since it is used as filename
        portName = strReplaceAll(portName, ".", "_");
        if (p->hasData()) {
            try {
                p->saveData(dir + portName);
            }
            catch(VoreenException& e) {
                LERROR("Failed to serialize data of port '" + p->getName() + "': "
                    << e.what() << ". Removing broken cache directory.");
                FileSys.deleteDirectoryRecursive(dir);
                return false;
            }
        }
        else {
            //handle empty outports (write PORTNAME.empty file)
            std::string fname = dir + portName + ".empty";
            std::fstream out(fname.c_str(), std::ios::out | std::ios::binary);

            if (!out.is_open() || out.bad()) {
                FileSys.deleteDirectoryRecursive(dir);
                LERROR("Could not serialize file dummy! Removing broken cache directory.");
                return false;
            }

            out.close();
        }
    }

    return true;
}

bool Cache::store() {
    if(!initialized_)
        return false;

    std::string dir = getCurrentCacheDir();
    if (!FileSys.dirExists(dir)) {
        if(!FileSys.createDirectoryRecursive(dir))
            return false;

        //write property state:
        std::string propertyState = getPropertyState();
        std::string fname = dir + "/propertystate.txt";

        std::fstream out(fname.c_str(), std::ios::out | std::ios::binary);

        if (out.is_open() || !out.bad()) {
            out.write(propertyState.c_str(), propertyState.length());
        }
        else {
            LERROR("Could not write propertystate file!");
            FileSys.deleteDirectoryRecursive(dir);
            return false;
        }

        out.close();

        return storeOutportsToDir(dir);
    }
    else {
        std::string fname = dir + "/propertystate.txt";

        if(FileSys.fileExists(fname)) {
            std::string propertyState = getPropertyState();
            if(stringEqualsFileContent(propertyState, fname))
                return true;
            else {
                LWARNING("PropertyState Collision! Deleting cache entry.");
                FileSys.deleteDirectoryRecursive(dir);
                return storeOutportsToDir(dir);
            }
        }
        else {
            LWARNING("No PropertyState in cache entry! Deleting.");
            FileSys.deleteDirectoryRecursive(dir);
            return storeOutportsToDir(dir);
        }
    }
}

bool Cache::restore() {
    if(!initialized_)
        return false;

    //check for collisions
    std::string dir = getCurrentCacheDir();
    if(FileSys.dirExists(dir)) {
        std::string fname = dir + "/propertystate.txt";

        if(!FileSys.fileExists(fname))
            return false;

        std::string propertyState = getPropertyState();
        if(stringEqualsFileContent(propertyState, fname))
            return restoreOutportsFromDir(dir);
        else {
            LWARNING("PropertyState Collision! Deleting cache entry.");
            FileSys.deleteDirectoryRecursive(dir);
            return false;
        }
    }
    else
        return false;
}

void Cache::clearCache() {
    std::string dir = processor_->getCachePath();
    LINFO("Clearing cache path: " << dir);

    if(FileSys.dirExists(dir)) {
        std::vector<std::string> subDirs = FileSys.listSubDirectories(dir);
        for(size_t i=0; i<subDirs.size(); i++) {
            FileSys.deleteDirectoryRecursive(dir + "/" + subDirs[i]);
        }

        std::vector<std::string> files = FileSys.listFiles(dir);
        for(size_t i=0; i<files.size(); i++) {
            if(files[i] == "interfaceStr.txt")
                continue;

            FileSys.deleteFile(dir + "/" + files[i]);
        }
    }
}

} // namespace
