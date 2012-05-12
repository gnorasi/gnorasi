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

#ifndef VRN_VOLUMESOURCEPROCESSOR_H
#define VRN_VOLUMESOURCEPROCESSOR_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/ports/volumeport.h"
#include "voreen/core/properties/volumehandleproperty.h"
#include "modules/base/basemoduledefine.h"

namespace voreen {

class Volume;

/**
 * Volume data set supplier in the network.
 */
class VRN_MODULE_BASE_API VolumeSource : public Processor, public VolumeHandleObserver {

public:
    VolumeSource();
    virtual Processor* create() const;

    virtual std::string getClassName() const    { return "VolumeSource";    }
    virtual std::string getCategory() const     { return "Data Source";     }
    virtual CodeState getCodeState() const      { return CODE_STATE_STABLE; }

    virtual void invalidate(int inv = INVALID_RESULT);

    /**
     * Loads the volume specified by filename. The loading is
     * delegated to the processor's VolumeHandleProperty.
     *
     * @param filename the volume to load
     */
    void loadVolume(const std::string& filename)
        throw (tgt::FileException, std::bad_alloc);

    /**
     * Clears the loaded volume.
     */
    void clearVolume();

    /**
     * Assigns a volume handle to this processors.
     *
     * @param handle the handle to assign, is written to the processor's outport
     */
    void setVolumeHandle(VolumeHandle* handle);

    /**
     * Returns a reference to the handle of the loaded volume. May be null.
     */
    VolumeHandle* getVolumeHandle() const;

    /**
     * Assigns the null pointer to its VolumeHandleProperty and its outport.
     *
     * @see VolumeHandleObserver
     */
    virtual void volumeHandleDelete(const VolumeHandleBase* source);

    /**
     * Noop.
     *
     * @see VolumeHandleObserver
     */
    virtual void volumeChange(const VolumeHandleBase* source);

protected:
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    VolumeHandleProperty volumeHandle_;

    /// The volume port the loaded data set is written to.
    VolumePort outport_;

    static const std::string loggerCat_;
};

} // namespace

#endif
