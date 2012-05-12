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

#include "voreen/core/ports/volumeport.h"

#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumecollection.h"
#include "voreen/core/io/volumeserializer.h"
#include "voreen/core/io/volumeserializerpopulator.h"

namespace voreen {

VolumePort::VolumePort(PortDirection direction, const std::string& name,
      bool allowMultipleConnections, Processor::InvalidationLevel invalidationLevel)
    : GenericPort<VolumeHandleBase>(direction, name, allowMultipleConnections, invalidationLevel),
      VolumeHandleObserver()
{}

bool VolumePort::isReady() const {
    if (isOutport())
        return isConnected();
    else
        return (hasData() && getData()->getRepresentation<Volume>() && checkConditions());
}

void VolumePort::setData(const VolumeHandleBase* handle, bool takeOwnership) {
    tgtAssert(isOutport(), "called setData on inport!");

    if (portData_ != handle) {
        if (portData_)
            portData_->removeObserver(this);

        GenericPort<VolumeHandleBase>::setData(handle, takeOwnership);
        //portData_ = handle;
        
        if (handle)
            handle->addObserver(this);
    }
    invalidate();
}

void VolumePort::volumeHandleDelete(const VolumeHandleBase* source) {
    if (getData() == source) {
        portData_ = 0; // we dont want to trigger automatic delete due to ownership
        //setData(0);
    }
}

void VolumePort::volumeChange(const VolumeHandleBase* source) {
    if (getData() == source) {
        hasChanged_ = true;
        invalidate();
    }
}

bool VolumePort::supportsCaching() const {
    return true;
}

std::string VolumePort::getHash() const {
    if (hasData())
        return getData()->getHash();
    else
        return "";
}

void VolumePort::saveData(const std::string& path) const throw (VoreenException) {
    if (!hasData())
        throw VoreenException("Port has no volume");
    tgtAssert(!path.empty(), "empty path");

    // append .dat if no extension specified
    std::string filename = path;
    if (tgt::FileSystem::fileExtension(filename).empty())
        filename += ".dat";

    VolumeSerializerPopulator serializerPop;
    const VolumeSerializer* serializer = serializerPop.getVolumeSerializer();
    try {
        serializer->write(filename, getData());
    }
    catch (VoreenException) {
        throw;
    }
    catch (std::exception& e) {
        throw VoreenException(e.what());
    }
}

void VolumePort::loadData(const std::string& path) throw (VoreenException) {
    tgtAssert(!path.empty(), "empty path");

    // append .dat if no extension specified
    std::string filename = path;
    if (tgt::FileSystem::fileExtension(filename).empty())
        filename += ".dat";

    VolumeSerializerPopulator serializerPop;
    const VolumeSerializer* serializer = serializerPop.getVolumeSerializer();
    try {
        VolumeCollection* volumeCollection = serializer->read(filename);
        tgtAssert(!volumeCollection->empty(), "empty collection");
        VolumeHandleBase* dataset = volumeCollection->first();
        setData(dataset, true);
        //we do not need the collection, just the volume:
        delete volumeCollection;
    }
    catch (VoreenException) {
        throw;
    }
    catch (std::exception& e) {
        throw VoreenException(e.what());
    }
}

} // namespace
