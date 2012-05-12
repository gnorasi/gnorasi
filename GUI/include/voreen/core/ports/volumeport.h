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

#ifndef VRN_VOLUMEPORT_H
#define VRN_VOLUMEPORT_H

#include "voreen/core/ports/genericport.h"
#include "voreen/core/datastructures/volume/volumehandle.h"

namespace voreen {

#ifdef DLL_TEMPLATE_INST
template class VRN_CORE_API GenericPort<VolumeHandleBase>;
#endif

class VRN_CORE_API VolumePort : public GenericPort<VolumeHandleBase>, public VolumeHandleObserver {
public:
    VolumePort(PortDirection direction, const std::string& name,
               bool allowMultipleConnections = false,
               Processor::InvalidationLevel invalidationLevel = Processor::INVALID_RESULT);

    /**
     * Assigns the passed volume handle to the port.
     */
    void setData(const VolumeHandleBase* handle, bool takeOwnership = true);

    /**
     * Returns true, if the port contains a VolumeHandle object
     * and the VolumeHandle has a valid volume.
     */
     virtual bool isReady() const;

    /**
     * Implementation of VolumeHandleObserver interface.
     */
    virtual void volumeHandleDelete(const VolumeHandleBase* source);

    /**
     * Implementation of VolumeHandleObserver interface.
     */
    virtual void volumeChange(const VolumeHandleBase* source);

    /// This port type supports caching.
    virtual bool supportsCaching() const;

    /**
     * Returns an MD5 hash of the stored volume,
     * or and empty string, if no volume is assigned.
     */
    virtual std::string getHash() const;

    /**
     * Saves the assigned volume to the given path.
     * If a filename without extension is passed,
     * ".dat" is appended to it.
     *
     * @throws VoreenException If saving failed or
     *      no volume is assigned.
     */
    virtual void saveData(const std::string& path) const
        throw (VoreenException);

    /**
     * Loads a volume from the given path and assigns it
     * to the port. If a filename without extension is passed,
     * ".dat" is appended to it.
     *
     * @throws VoreenException if loading failed.
     */
    virtual void loadData(const std::string& path)
        throw (VoreenException);
};

} // namespace

#endif // VRN_VOLUMEPORT_H
