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

#include "volumescaling.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumehandledecorator.h"
#include "voreen/core/io/serialization/meta/primitivemetadata.h"

namespace voreen {

const std::string VolumeScaling::loggerCat_("voreen.VolumeScaling");

VolumeScaling::VolumeScaling()
    : VolumeProcessor(),
    inport_(Port::INPORT, "volumehandle.input"),
    outport_(Port::OUTPORT, "volumehandle.output", 0),
    enableProcessing_("enabled", "Enable", false),
    uniformScaling_("uniformScaling", "Uniform Scaling", false),
    scalingX_("scalingX", "Scaling Factor X", 1.0f, 0.01f, 10.f),
    scalingY_("scalingY", "Scaling Factor Y", 1.0f, 0.01f, 10.f),
    scalingZ_("scalingZ", "Scaling Factor Z", 1.0f, 0.01f, 10.f),
    spacingDisplay_("spacingDisplay", "Resulting Spacing", tgt::vec3(1.0f), tgt::vec3(0.0f), tgt::vec3(100.f))
{
    addPort(inport_);
    addPort(outport_);

    addProperty(enableProcessing_);
    addProperty(uniformScaling_);

    scalingX_.setNumDecimals(5);
    scalingY_.setNumDecimals(5);
    scalingZ_.setNumDecimals(5);
    addProperty(scalingX_);
    addProperty(scalingY_);
    addProperty(scalingZ_);

    spacingDisplay_.setWidgetsEnabled(false);
    addProperty(spacingDisplay_);

    scalingX_.onChange(
        Call1ParMemberAction<VolumeScaling, int>(this, &VolumeScaling::scalingChanged, 0));
    scalingY_.onChange(
        Call1ParMemberAction<VolumeScaling, int>(this, &VolumeScaling::scalingChanged, 1));
    scalingZ_.onChange(
        Call1ParMemberAction<VolumeScaling, int>(this, &VolumeScaling::scalingChanged, 2));

    uniformScaling_.onChange(
        CallMemberAction<VolumeScaling>(this, &VolumeScaling::uniformScalingChanged));
}

Processor* VolumeScaling::create() const {
    return new VolumeScaling();
}

void VolumeScaling::process() {
    const VolumeHandleBase* inputVolume = inport_.getData();  
    
    if (!enableProcessing_.get()) {
        spacingDisplay_.set(inputVolume->getSpacing());
        outport_.setData(const_cast<VolumeHandleBase*>(inputVolume), false);
    }
    else {
        tgt::vec3 scaling(scalingX_.get(), scalingY_.get(), scalingZ_.get());
        tgt::vec3 spacing = inputVolume->getSpacing() * scaling;
        spacingDisplay_.set(spacing);
        VolumeHandleBase* outputVolume = new VolumeHandleDecoratorReplace(inputVolume, "Spacing", new Vec3MetaData(spacing));
        outport_.setData(outputVolume);
    }
}

void VolumeScaling::scalingChanged(int dim) {

    if (!uniformScaling_.get())
        return;

    if (dim == 0) {
        float xScale = scalingX_.get();
        scalingY_.set(xScale);
        scalingZ_.set(xScale);
    }
    else if (dim == 1) {
        float yScale = scalingY_.get();
        scalingX_.set(yScale);
        scalingZ_.set(yScale);
    }
    else if (dim == 2) {
        float zScale = scalingZ_.get();
        scalingX_.set(zScale);
        scalingY_.set(zScale);
    }
}

void VolumeScaling::uniformScalingChanged() {
    if (uniformScaling_.get())
        scalingChanged(0);
}

}   // namespace
