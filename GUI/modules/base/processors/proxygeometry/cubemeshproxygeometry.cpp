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

#include "cubemeshproxygeometry.h"
#include "voreen/core/datastructures/geometry/meshlistgeometry.h"

namespace voreen {

const std::string CubeMeshProxyGeometry::loggerCat_("voreen.CubeMeshProxyGeometry");

CubeMeshProxyGeometry::CubeMeshProxyGeometry()
    : Processor()
    , inport_(Port::INPORT, "volumehandle.volumehandle")
    , outport_(Port::OUTPORT, "proxygeometry.geometry")
    , enableClipping_("useClipping", "Enable Clipping", true)
    , clipRight_("rightClippingPlane", "Right Clip Plane (x)", 0.f, 0.f, 1e5f)
    , clipLeft_("leftClippingPlane", "Left Clip Plane (x)", 1e5f, 0.f, 1e5f)
    , clipFront_("frontClippingPlane", "Front Clip Plane (y)", 0.f, 0.f, 1e5f)
    , clipBack_("backClippingPlane", "Back Clip Plane (y)", 1e5f, 0.f, 1e5f)
    , clipBottom_("bottomClippingPlane", "Bottom Clip Plane (z)", 0.f, 0.f, 1e5f)
    , clipTop_("topClippingPlane", "Top Clip Plane (z)", 1e5f, 0.f, 1e5f)
    , resetClipPlanes_("resetClipPlanes", "Reset Planes")
{

    addPort(inport_);
    addPort(outport_);

    clipRight_.onChange(CallMemberAction<CubeMeshProxyGeometry>(this, &CubeMeshProxyGeometry::onClipRightChange));
    clipLeft_.onChange(CallMemberAction<CubeMeshProxyGeometry>(this, &CubeMeshProxyGeometry::onClipLeftChange));
    clipFront_.onChange(CallMemberAction<CubeMeshProxyGeometry>(this, &CubeMeshProxyGeometry::onClipFrontChange));
    clipBack_.onChange(CallMemberAction<CubeMeshProxyGeometry>(this, &CubeMeshProxyGeometry::onClipBackChange));
    clipBottom_.onChange(CallMemberAction<CubeMeshProxyGeometry>(this, &CubeMeshProxyGeometry::onClipBottomChange));
    clipTop_.onChange(CallMemberAction<CubeMeshProxyGeometry>(this, &CubeMeshProxyGeometry::onClipTopChange));
    enableClipping_.onChange(CallMemberAction<CubeMeshProxyGeometry>(this, &CubeMeshProxyGeometry::adjustClipPropertiesVisibility));
    resetClipPlanes_.onChange(CallMemberAction<CubeMeshProxyGeometry>(this, &CubeMeshProxyGeometry::resetClipPlanes));

    addProperty(enableClipping_);
    addProperty(clipRight_);
    addProperty(clipLeft_);
    addProperty(clipFront_);
    addProperty(clipBack_);
    addProperty(clipBottom_);
    addProperty(clipTop_);
    addProperty(resetClipPlanes_);

    clipRight_.setGroupID("clipping");
    clipLeft_.setGroupID("clipping");
    clipFront_.setGroupID("clipping");
    clipBack_.setGroupID("clipping");
    clipBottom_.setGroupID("clipping");
    clipTop_.setGroupID("clipping");
    resetClipPlanes_.setGroupID("clipping");
    setPropertyGroupGuiName("clipping", "Clipping Planes");
    adjustClipPropertiesVisibility();

    oldVolumeDimensions_ = tgt::ivec3(0,0,0);
}

CubeMeshProxyGeometry::~CubeMeshProxyGeometry() {
}

Processor* CubeMeshProxyGeometry::create() const {
    return new CubeMeshProxyGeometry();
}

void CubeMeshProxyGeometry::process() {
    tgtAssert(inport_.getData(), "no input volume");

    // adapt clipping plane properties on volume change
    if (inport_.hasChanged()) {
        adjustClipPropertiesRanges();
    }

    const VolumeHandleBase* inputVolume = inport_.getData();
    tgt::vec3 volumeSize = inputVolume->getCubeSize();

     // vertex and tex coords of bounding box without clipping
    tgt::vec3 coordLlf = inputVolume->getLLF();
    tgt::vec3 coordUrb = inputVolume->getURB();
    const tgt::vec3 noClippingTexLlf(0, 0, 0);
    const tgt::vec3 noClippingTexUrb(1, 1, 1);

    tgt::vec3 texLlf;
    tgt::vec3 texUrb;
    if (enableClipping_.get()) {
        tgt::ivec3 orgDims = inputVolume->getOriginalDimensions();
        // adjust vertex and tex coords to clipping
        texLlf = tgt::vec3(
            clipRight_.get()  / static_cast<float>(orgDims.x),
            clipFront_.get()  / static_cast<float>(orgDims.y),
            clipBottom_.get() / static_cast<float>(orgDims.z));
        texUrb = tgt::vec3(
            (clipLeft_.get()+1.0f) / static_cast<float>(orgDims.x),
            (clipBack_.get()+1.0f) / static_cast<float>(orgDims.y),
            (clipTop_.get()+1.0f)  / static_cast<float>(orgDims.z));

        // calculate original offset and clip volume
        tgt::vec3 orgDimSpac = static_cast<tgt::vec3>(orgDims)*inputVolume->getSpacing();
        tgt::vec3 orgOffset = orgDimSpac / tgt::max(orgDimSpac); 
        tgt::vec3 coordLlfOrg = (texLlf*(2.f*orgOffset))-orgOffset;
        tgt::vec3 coordUrbOrg = (texUrb*(2.f*orgOffset))-orgOffset;
        tgt::vec3 coordSize = coordUrb-coordLlf;

        // calculate correct volume coordinates [-1, 1]
        coordLlf = tgt::min(tgt::max(coordLlfOrg, coordLlf), coordUrb);
        coordUrb = tgt::max(tgt::min(coordUrbOrg, coordUrb), coordLlf);

        // calculate correct texture coordinates [0, 1]
        texLlf = (coordLlf-inputVolume->getLLF())/(coordSize);
        texUrb = tgt::vec3(1.f)-(inputVolume->getURB()-coordUrb)/(coordSize);
    }
    else {
        texLlf = noClippingTexLlf;
        texUrb = noClippingTexUrb;
    }

    // create output mesh
    MeshListGeometry* geometry = new MeshListGeometry();
    geometry->addMesh(MeshGeometry::createCube(coordLlf, coordUrb, texLlf, texUrb, texLlf, texUrb));
    geometry->transform(inputVolume->getPhysicalToWorldMatrix());

    outport_.setData(geometry);
}

void CubeMeshProxyGeometry::onClipRightChange() {
    if (clipRight_.get() > clipLeft_.get())
        clipLeft_.set(clipRight_.get());
}

void CubeMeshProxyGeometry::onClipLeftChange() {
    if (clipRight_.get() > clipLeft_.get())
        clipRight_.set(clipLeft_.get());
}

void CubeMeshProxyGeometry::onClipFrontChange() {
    if (clipFront_.get() > clipBack_.get())
        clipBack_.set(clipFront_.get());
}

void CubeMeshProxyGeometry::onClipBackChange() {
    if (clipFront_.get() > clipBack_.get())
        clipFront_.set(clipBack_.get());
}

void CubeMeshProxyGeometry::onClipBottomChange() {
    if (clipBottom_.get() > clipTop_.get())
        clipTop_.set(clipBottom_.get());
}

void CubeMeshProxyGeometry::onClipTopChange() {
    if (clipBottom_.get() > clipTop_.get())
        clipBottom_.set(clipTop_.get());
}

void CubeMeshProxyGeometry::resetClipPlanes() {

    clipRight_.set(0.0f);
    clipLeft_.set(clipLeft_.getMaxValue());

    clipFront_.set(0.0f);
    clipBack_.set(clipBack_.getMaxValue());

    clipBottom_.set(0.0f);
    clipTop_.set(clipTop_.getMaxValue());
}

void CubeMeshProxyGeometry::adjustClipPropertiesRanges() {
    tgtAssert(inport_.getData() && inport_.getData()->getRepresentation<Volume>(), "No input volume");

    if (oldVolumeDimensions_ == tgt::ivec3(0,0,0))
        oldVolumeDimensions_ = inport_.getData()->getRepresentation<Volume>()->getOriginalDimensions();

    tgt::ivec3 numSlices = inport_.getData()->getRepresentation<Volume>()->getOriginalDimensions();

    // adapt clipping plane properties to volume dimensions
    clipRight_.setMaxValue(numSlices.x-1.0f);
    clipLeft_.setMaxValue(numSlices.x-1.0f);

    clipFront_.setMaxValue(numSlices.y-1.0f);
    clipBack_.setMaxValue(numSlices.y-1.0f);

    clipBottom_.setMaxValue(numSlices.z-1.0f);
    clipTop_.setMaxValue(numSlices.z-1.0f);

    // assign new clipping values while taking care that the right>left validation
    // does not alter the assigned values
    float scaleRight = tgt::clamp(clipRight_.get()/static_cast<float>(oldVolumeDimensions_.x-1), 0.f, 1.f);
    float scaleLeft =  tgt::clamp(clipLeft_.get()/static_cast<float>(oldVolumeDimensions_.x-1), 0.f, 1.f);
    float rightVal = scaleRight * (numSlices.x-1);
    float leftVal = scaleLeft * (numSlices.x-1);
    clipLeft_.set(clipLeft_.getMaxValue());
    clipRight_.set(rightVal);
    clipLeft_.set(leftVal);

    float scaleFront = tgt::clamp(clipFront_.get()/static_cast<float>(oldVolumeDimensions_.y-1), 0.f, 1.f);
    float scaleBack =  tgt::clamp(clipBack_.get()/static_cast<float>(oldVolumeDimensions_.y-1), 0.f, 1.f);
    float frontVal = scaleFront * (numSlices.y-1);
    float backVal = scaleBack * (numSlices.y-1);
    clipBack_.set(clipBack_.getMaxValue());
    clipFront_.set(frontVal);
    clipBack_.set(backVal);

    float scaleBottom = tgt::clamp(clipBottom_.get()/static_cast<float>(oldVolumeDimensions_.z-1), 0.f, 1.f);
    float scaleTop =  tgt::clamp(clipTop_.get()/static_cast<float>(oldVolumeDimensions_.z-1), 0.f, 1.f);
    float bottomVal = scaleBottom * (numSlices.z-1);
    float topVal = scaleTop * (numSlices.z-1);
    clipTop_.set(clipTop_.getMaxValue());
    clipBottom_.set(bottomVal);
    clipTop_.set(topVal);

    if (clipRight_.get() > clipRight_.getMaxValue())
        clipRight_.set(clipRight_.getMaxValue());

    if (clipLeft_.get() > clipLeft_.getMaxValue())
        clipLeft_.set(clipLeft_.getMaxValue());

    if (clipFront_.get() > clipFront_.getMaxValue())
        clipFront_.set(clipFront_.getMaxValue());

    if (clipBack_.get() > clipBack_.getMaxValue())
        clipBack_.set(clipBack_.getMaxValue());

    if (clipBottom_.get() > clipBottom_.getMaxValue())
        clipBottom_.set(clipBottom_.getMaxValue());

    if (clipTop_.get() > clipTop_.getMaxValue())
        clipTop_.set(clipTop_.getMaxValue());

    oldVolumeDimensions_ = numSlices;
}

void CubeMeshProxyGeometry::adjustClipPropertiesVisibility() {
    bool clipEnabled = enableClipping_.get();
    setPropertyGroupVisible("clipping", clipEnabled);
}


} // namespace
