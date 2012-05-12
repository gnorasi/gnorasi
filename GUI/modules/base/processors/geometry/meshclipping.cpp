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

#include "meshclipping.h"

namespace voreen {

const std::string MeshClipping::loggerCat_("voreen.MeshClipping");

MeshClipping::MeshClipping()
    : Processor()
    , inport_(Port::INPORT, "geometry.geometry")
    , outport_(Port::OUTPORT, "geometry.clippedgeometry")
    , enabled_("enabled", "Enabled", true)
    , normal_("planeNormal", "Plane Normal", tgt::vec3(0, 1, 0), tgt::vec3(-1), tgt::vec3(1))
    , position_("planePosition", "Plane Position", 0.0f, -10.0f, 10.0f)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(enabled_);
    addProperty(normal_);
    addProperty(position_);
}

MeshClipping::~MeshClipping()
{}

Processor* MeshClipping::create() const {
    return new MeshClipping();
}

void MeshClipping::process() {
    tgtAssert(inport_.getData(), "no geometry");

    const MeshListGeometry* inportGeometry = dynamic_cast<const MeshListGeometry*>(inport_.getData());
    if (!inportGeometry) {
        LWARNING("Input geometry type not supported, expecting MeshListGeometry.");
        outport_.setData(0);
        return;
    }

    MeshListGeometry* geometry = new MeshListGeometry(*inportGeometry);

    if (enabled_.get()) {
        geometry->clip(tgt::vec4(tgt::normalize(normal_.get()), position_.get()));
    }

    outport_.setData(geometry);
}

}  //namespace
