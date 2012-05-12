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

#include "meshslabclipping.h"

namespace voreen {

MeshSlabClipping::MeshSlabClipping()
    : Processor()
    , inport_(Port::INPORT, "geometry.geometry")
    , outport_(Port::OUTPORT, "geometry.clippedgeometry")
    , normal_("slabNormal", "Slab Normal", tgt::vec3(0, 1, 0), tgt::vec3(-1), tgt::vec3(1))
    , position_("slabPosition", "Slab Position", 0.0f, -10.0f, 10.0f)
    , thickness_("slabThickness", "Slab Thickness", 0.5f, 0.0f, 3.0f)
    , wheelInteractionHandler_("wheelInteractionHandler", "Slab Position", &position_,
        tgt::Event::MODIFIER_NONE, false, false)
{
    addPort(inport_);
    addPort(outport_);

    thickness_.onChange(CallMemberAction<MeshSlabClipping>(this, &MeshSlabClipping::thicknessChanged));
    addProperty(normal_);
    addProperty(position_);
    addProperty(thickness_);

    addInteractionHandler(wheelInteractionHandler_);
}

MeshSlabClipping::~MeshSlabClipping() {
}

Processor* MeshSlabClipping::create() const {
    return new MeshSlabClipping();
}

void MeshSlabClipping::process() {
    tgtAssert(inport_.getData(), "no geometry");

    const MeshListGeometry* inportGeometry = dynamic_cast<const MeshListGeometry*>(inport_.getData());
    if (!inportGeometry) {
        LWARNING("Input geometry of type MeshListGeometry expected.");
        outport_.setData(0);
        return;
    }

    MeshListGeometry* geometry = new MeshListGeometry(*inportGeometry);

    geometry->clip(tgt::vec4(tgt::normalize( normal_.get()), position_.get() + thickness_.get()/2.f));
    geometry->clip(tgt::vec4(tgt::normalize(-normal_.get()), -position_.get() + thickness_.get()/2.f));

    outport_.setData(geometry);
}

void MeshSlabClipping::thicknessChanged() {
    position_.setStepping(std::min(thickness_.get(), 0.1f));
}

}  //namespace
