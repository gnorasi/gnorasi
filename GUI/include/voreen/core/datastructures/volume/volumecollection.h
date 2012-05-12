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

#ifndef VRN_VOLUMECOLLECTION_H
#define VRN_VOLUMECOLLECTION_H

#include "voreen/core/utils/observer.h"
#include "voreen/core/io/serialization/serializable.h"
#include "voreen/core/datastructures/volume/volumehandle.h"

#include <vector>
#include <string>

namespace voreen {

class VolumeCollection;
class Modality;

/**
 * Interface for volume collection observers.
 */
class VRN_CORE_API VolumeCollectionObserver : public Observer {
public:

    /**
     * This method is called by the observed collection after
     * a volume handle has been added.
     *
     * @param source the calling collection
     * @param handle the volume handle that has been added
     */
    virtual void volumeAdded(const VolumeCollection* /*source*/, const VolumeHandleBase* /*handle*/) {};

    /**
     * This method is called by the observed collection after
     * a volume handle has been removed.
     *
     * @param source the calling collection
     * @param handle the volume handle that has been removed
     */
    virtual void volumeRemoved(const VolumeCollection* /*source*/, const VolumeHandleBase* /*handle*/) {};

    /**
    * This method is called by the observed collection after
    * a volume handle has been changed. This usually happens when
    * a volume handle reloads the wrapped volume.
    *
    * @param source the calling collection
    * @param handle the volume handle that has been changed
    */
    virtual void volumeChanged(const VolumeCollection* /*source*/, const VolumeHandleBase* /*handle*/) {};

};

#ifdef DLL_TEMPLATE_INST
template class VRN_CORE_API Observable<VolumeCollectionObserver>;
#endif

/**
 * Collection of volume handles that can be serialized and observed.
 *
 * The collection does not perform memory management, i.e., added volume handles
 * are not deleted on removal or destruction of the collection.
 *
 * @see VolumeContainer
 */
class VRN_CORE_API VolumeCollection : public Serializable, public Observable<VolumeCollectionObserver>, protected VolumeHandleObserver {

public:

    VolumeCollection();

    virtual ~VolumeCollection();

    /**
     * Adds the passed VolumeHandle to the collection,
     * if it is not already contained.
     *
     * The Collection does not take ownership of the
     * added VolumeHandle and does therefore not delete it
     * on its own destruction.
     */
    virtual void add(VolumeHandleBase* volumeHandle);

    /**
     * Adds VolumeHandles contained by the passed VolumeCollection
     * to the collection, if they are not already contained.
     *
     * The Collection does not take ownership of the
     * added VolumeHandles and does therefore not delete them
     * on its own destruction.
     */
    virtual void add(const VolumeCollection* volumeCollection);

    /**
     * Removes the passed VolumeHandle from the Collection
     * without deleting it.
     */
    virtual void remove(const VolumeHandleBase* volumeHandle);

    /**
     * Removes all VolumeHandles contained by the passed collection
     * without deleting them.
     */
    virtual void remove(const VolumeCollection* volumeCollection);

    /**
     * Returns whether the passed VolumeHandle is contained
     * by the collection.
     */
    virtual bool contains(const VolumeHandleBase* volumeHandle) const;

    /**
     * Returns the VolumeElement at a specified index position.
     *
     * @param The index of the VolumeHandle to return. Must be valid, i.e. i < size().
     */
    virtual VolumeHandleBase* at(size_t i) const;

    /**
     * Returns the first VolumeHandle of the collection, or null
     * if the collection is empty.
     */
    virtual VolumeHandleBase* first() const;

    /**
     * Clears the collection without deleting the VolumeHandles.
     */
    virtual void clear();

    /**
     * Returns a collection containing all VolumeHandles of the
     * specified entity.
     *
     * @note not yet implemented, returns empty collection.
     */
    virtual VolumeCollection* selectEntity(const std::string& name) const;

    /**
     * Returns a collection containing all VolumeHandles of the
     * specified representation.
     *
     * @note not yet implemented, returns empty collection.
     */
    virtual VolumeCollection* selectRepresentation(const std::string& name) const;

    /**
     * Returns a collection containing all VolumeHandles of the
     * specified modality.
     */
    virtual VolumeCollection* selectModality(const Modality& name) const;

    /**
     * Returns a collection containing all VolumeHandles of the
     * specified timestep.
     */
    virtual VolumeCollection* selectTimestep(float timestep) const;

    /**
     * Returns a collection containing all VolumeHandles with the
     * specified origin.
     */
    virtual VolumeCollection* selectOrigin(const VolumeOrigin& origin) const;

    /**
     * Returns a collection containing all VolumeHandles with the
     * specified origin and timestep
     **/
    virtual VolumeCollection* selectOriginTimestep(const VolumeOrigin& origin, float timestep) const;

    /**
     * Returns a new collection containing the volumes between the passed start and end indices (inclusive).
     */
    virtual VolumeCollection* subCollection(size_t start, size_t end) const;

    /**
     * Returns a new collection containing the volumes at the passed indices.
     */
    virtual VolumeCollection* subCollection(const std::vector<size_t>& indices) const;

    /**
     * Returns the number of VolumeHandles contained by the collection.
     */
    virtual size_t size() const;

    /**
     * Returns whether the collection is currently empty.
     */
    virtual bool empty() const;

    /// @see VolumeHandleObserver::volumeChange
    virtual void volumeChange(const VolumeHandleBase* handle);

    /// @see VolumeHandleObserver::volumeHandleDelete
    virtual void volumeHandleDelete(const VolumeHandleBase* handle);

    /// @see Serializable::serialize
    void serialize(XmlSerializer& s) const;

    /// @see Serializable::deserialize
    void deserialize(XmlDeserializer& s);

protected:
    /**
     * Returns an iterator pointing to the position of the passed VolumeHandle
     * within the volumeHandles_ vector. Returns volumeHandles_.end(), if
     * the handle is not contained by the collection.
     */
    std::vector<VolumeHandleBase*>::iterator find(const VolumeHandleBase* volumeHandle);
    std::vector<VolumeHandleBase*>::const_iterator find(const VolumeHandleBase* volumeHandle) const;

    /// Notifies all VolumeCollectionObservers that a handle has been added.
    void notifyVolumeAdded(const VolumeHandleBase* handle);
    /// Notifies all VolumeCollectionObservers that a handle has been removed.
    void notifyVolumeRemoved(const VolumeHandleBase* handle);
    /// Notifies all VolumeCollectionObservers that a handle has been changed.
    void notifyVolumeChanged(const VolumeHandleBase* handle);

    /// Vector storing the VolumeHandles contained by the collection.
    std::vector<VolumeHandleBase*> volumeHandles_;

    /// category for logging.
    static const std::string loggerCat_;
};

}   // namespace

#endif
