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

#include "voreen/core/datastructures/volume/volumecollection.h"

#include "voreen/core/datastructures/volume/volumehandle.h"

using std::vector;

namespace voreen {

const std::string VolumeCollection::loggerCat_ = "voreen.VolumeCollection";

VolumeCollection::VolumeCollection() :
    Observable<VolumeCollectionObserver>()
{
}

VolumeCollection::~VolumeCollection()     {
    clear();
}

void VolumeCollection::add(VolumeHandleBase* volumeHandle) {

    tgtAssert(volumeHandle, "Null pointer as VolumeHandle passed");
    if (!contains(volumeHandle)) {
        volumeHandles_.push_back(volumeHandle);
        volumeHandle->addObserver(this);
        notifyVolumeAdded(volumeHandle);
    }
}

void VolumeCollection::add(const VolumeCollection* volumeCollection) {

    tgtAssert(volumeCollection, "Unexpected null pointer");
    for (size_t i=0; i<volumeCollection->size(); ++i) {
        add(volumeCollection->at(i));
    }
}

void VolumeCollection::remove(const VolumeHandleBase* volumeHandle){
    std::vector<VolumeHandleBase*>::iterator handleIter = find(volumeHandle);
    if (handleIter != volumeHandles_.end()) {
        volumeHandles_.erase(handleIter);
        notifyVolumeRemoved(volumeHandle);
    }
}

void VolumeCollection::remove(const VolumeCollection* volumeCollection) {
   tgtAssert(volumeCollection, "Unexpected null pointer");
    for (size_t i=0; i<volumeCollection->size(); ++i) {
        remove(volumeCollection->at(i));
    }
}

std::vector<VolumeHandleBase*>::iterator VolumeCollection::find(const VolumeHandleBase* volumeHandle) {
    return std::find(volumeHandles_.begin(), volumeHandles_.end(), volumeHandle);
}

std::vector<VolumeHandleBase*>::const_iterator VolumeCollection::find(const VolumeHandleBase* volumeHandle) const {
    return std::find(volumeHandles_.begin(), volumeHandles_.end(), volumeHandle);
}

bool VolumeCollection::contains(const VolumeHandleBase* volumeHandle) const {
    return (find(volumeHandle) != volumeHandles_.end());
}

VolumeHandleBase* VolumeCollection::at(size_t i) const {
    tgtAssert(i < volumeHandles_.size(), "Invalid index");
    return volumeHandles_.at(i);
}

VolumeHandleBase* VolumeCollection::first() const {
    return (!empty() ? volumeHandles_.front() : 0);
}

void VolumeCollection::clear() {
    while (!empty())
        remove(first());
}

VolumeCollection* VolumeCollection::selectEntity(const std::string& /*name*/) const {
    return new VolumeCollection();
}

VolumeCollection* VolumeCollection::selectRepresentation(const std::string& /*name*/) const {
    return new VolumeCollection();
}

VolumeCollection* VolumeCollection::selectModality(const Modality& modality) const {

    VolumeCollection* collection = new VolumeCollection();
    for (size_t i=0; i<volumeHandles_.size(); ++i) {
        if (volumeHandles_[i]->getModality() == modality)
            collection->add(volumeHandles_[i]);
    }
    return collection;
}

VolumeCollection* VolumeCollection::selectTimestep(float timestep) const {

    VolumeCollection* collection = new VolumeCollection();
    for (size_t i=0; i<volumeHandles_.size(); ++i) {
        if (volumeHandles_[i]->getTimestep() == timestep)
            collection->add(volumeHandles_[i]);
    }
    return collection;
}

voreen::VolumeCollection* VolumeCollection::selectOrigin(const VolumeOrigin& origin) const {
    VolumeCollection* collection = new VolumeCollection();
    for (size_t i=0; i<volumeHandles_.size(); ++i) {
        VolumeHandle* vh = dynamic_cast<VolumeHandle*>(volumeHandles_[i]);
        if (vh && vh->getOrigin() == origin)
            collection->add(volumeHandles_[i]);
    }
    return collection;
}

voreen::VolumeCollection* VolumeCollection::selectOriginTimestep(const VolumeOrigin& origin, float timestep) const {
    VolumeCollection* collection = new VolumeCollection();
    for (size_t i=0; i<volumeHandles_.size(); ++i) {
        VolumeHandle* vh = dynamic_cast<VolumeHandle*>(volumeHandles_[i]);
        if (vh && vh->getOrigin() == origin && vh->getTimestep() == timestep)
            collection->add(volumeHandles_[i]);
    }
    return collection;
}

VolumeCollection* VolumeCollection::subCollection(size_t start, size_t end) const {
    VolumeCollection* subCollection = new VolumeCollection();
    tgtAssert(start <= end, "invalid indices");
    tgtAssert(start < volumeHandles_.size(), "invalid start index");
    tgtAssert(end < volumeHandles_.size(), "invalid end index");
    for (size_t index = start; index <= end; index++)
        subCollection->add(volumeHandles_.at(index));
    return subCollection;
}

VolumeCollection* VolumeCollection::subCollection(const std::vector<size_t>& indices) const {
    VolumeCollection* subCollection = new VolumeCollection();
    for (size_t i=0; i<indices.size(); i++) {
        tgtAssert(indices.at(i) < volumeHandles_.size(), "invalid index");
        subCollection->add(volumeHandles_.at(indices.at(i)));
    }
    return subCollection;
}


size_t VolumeCollection::size() const {
    return volumeHandles_.size();
}

bool VolumeCollection::empty() const {
    return (size() == 0);
}

void VolumeCollection::notifyVolumeAdded(const VolumeHandleBase* handle) {
    const vector<VolumeCollectionObserver*> observers = getObservers();
    for (size_t i=0; i<observers.size(); ++i)
        observers[i]->volumeAdded(this, handle);

}

void VolumeCollection::notifyVolumeRemoved(const VolumeHandleBase* handle) {
    const vector<VolumeCollectionObserver*> observers = getObservers();
    for (size_t i=0; i<observers.size(); ++i) 
        observers[i]->volumeRemoved(this, handle);
}

void VolumeCollection::notifyVolumeChanged(const VolumeHandleBase* handle) {
    const vector<VolumeCollectionObserver*> observers = getObservers();
    for (size_t i=0; i<observers.size(); ++i)
        observers[i]->volumeChanged(this, handle);
}

void VolumeCollection::serialize(XmlSerializer& s) const {
    std::vector<VolumeHandle*> vhs;
    for (size_t i=0; i<volumeHandles_.size(); ++i) {
        if (dynamic_cast<VolumeHandle*>(volumeHandles_[i]))
            vhs.push_back(static_cast<VolumeHandle*>(volumeHandles_[i]));
    }
    s.serialize("VolumeHandles", vhs, "VolumeHandle");
}

void VolumeCollection::deserialize(XmlDeserializer& s) {
    std::vector<VolumeHandle*> handleList;
    s.deserialize("VolumeHandles", handleList, "VolumeHandle");

    // add volume handles of loaded volumes...
    std::vector<VolumeHandle*> deleteList;
    for (std::vector<VolumeHandle*>::iterator it = handleList.begin(); it != handleList.end(); ++it)
        if ((*it) && static_cast<VolumeHandle*>((*it))->getNumRepresentations() > 0)
            add(*it);
        else
            deleteList.push_back(*it);

    // remove volume handles that were not able to load their volume...
    for (std::vector<VolumeHandle*>::iterator it = deleteList.begin(); it != deleteList.end(); ++it) {
        //remove(*it);
        s.freePointer(*it);
        delete *it;
    }
}

// implementation of VolumeHandleObserver interface
void VolumeCollection::volumeChange(const VolumeHandleBase* handle) {
    if (contains(handle))
        notifyVolumeChanged(handle);

}

// implementation of VolumeHandleObserver interface
void VolumeCollection::volumeHandleDelete(const VolumeHandleBase* handle) {
    if (contains(handle))
        remove(handle);
}

} // namespace
