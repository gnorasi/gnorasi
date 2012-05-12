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

#include "volumesource.h"

#include "voreen/core/datastructures/volume/volumehandle.h"
#include "voreen/core/processors/processorwidget.h"
#include "voreen/core/io/volumeserializerpopulator.h"
#include "voreen/core/io/volumeserializer.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string VolumeSource::loggerCat_("voreen.VolumeSourceProcessor");

VolumeSource::VolumeSource()
    : Processor()
    , volumeHandle_("volumeHandle", "Volume", 0)
    , outport_(Port::OUTPORT, "volumehandle.volumehandle", 0)
{
    addPort(outport_);
    addProperty(volumeHandle_);
}

Processor* VolumeSource::create() const {
    return new VolumeSource();
}

void VolumeSource::process() {}

void VolumeSource::initialize() throw (tgt::Exception) {
    Processor::initialize();

    outport_.setData(volumeHandle_.get(), false);

    if (volumeHandle_.get())
        volumeHandle_.get()->addObserver(this);

    if (getProcessorWidget())
        getProcessorWidget()->updateFromProcessor();
}

void VolumeSource::deinitialize() throw (tgt::Exception) {
    clearVolume();

    Processor::deinitialize();
}

void VolumeSource::loadVolume(const std::string& filename) throw (tgt::FileException, std::bad_alloc) {
    clearVolume();

    if (volumeHandle_.get())
        stopObservation(volumeHandle_.get());

    volumeHandle_.loadVolume(filename);

    if (volumeHandle_.get())
        volumeHandle_.get()->addObserver(this);

    if (getProcessorWidget())
        getProcessorWidget()->updateFromProcessor();
}

void VolumeSource::clearVolume() {
    if (volumeHandle_.get()) {
        stopObservation(volumeHandle_.get());
        volumeHandle_.set(0);
    }
}

void VolumeSource::setVolumeHandle(VolumeHandle* handle) {
    if (volumeHandle_.get())
        stopObservation(volumeHandle_.get());

    volumeHandle_.set(handle);

    if (handle)
        handle->addObserver(this);
}

VolumeHandle* VolumeSource::getVolumeHandle() const {
    return volumeHandle_.get();
}

void VolumeSource::invalidate(int /*inv*/) {
    if (outport_.getData() != volumeHandle_.get()) {
        outport_.setData(volumeHandle_.get(), false);
        if (getProcessorWidget())
            getProcessorWidget()->updateFromProcessor();
    }
}

void VolumeSource::volumeHandleDelete(const VolumeHandleBase* source) {
    if (volumeHandle_.get() == source)
        volumeHandle_.set(0);
}

void VolumeSource::volumeChange(const VolumeHandleBase* /*source*/) {
    // noop
}

} // namespace
