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

#include "tgt/tgt_math.h"
#include <limits>
#include <algorithm>
#include <sstream>

#include "tgt/tgt_gl.h"
#include "tgt/gpucapabilities.h"

#include "tgt/plane.h"
#include "tgt/glmath.h"
#include "tgt/textureunit.h"

#include "slicerendererbase.h"
#include "voreen/core/datastructures/volume/modality.h"
#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/ports/conditions/portconditionvolumetype.h"

using tgt::vec2;
using tgt::vec3;
using tgt::vec4;
using tgt::ivec2;
using tgt::ivec3;
using tgt::TextureUnit;

namespace {
template<typename T>
void copySliceData(const voreen::VolumeAtomic<T>* volume, tgt::Texture* sliceTexture, 
    voreen::SliceRendererBase::SliceAlignment sliceAlign, size_t sliceID, bool flipX, bool flipY);
}

namespace voreen {

const std::string SliceRendererBase::loggerCat_("voreen.base.SliceRendererBase");

SliceRendererBase::SliceRendererBase()
    : VolumeRenderer()
    , outport_(Port::OUTPORT, "image.outport")
    , inport_(Port::INPORT, "volumehandle.volumehandle")
    , transferFunc_("transferFunction", "Transfer Function")
    , texMode_("textureMode", "Texture Mode", Processor::INVALID_PROGRAM)
    , texFilterMode_("textureFilterMode", "Texture Filtering")
    , texClampMode_("textureClampMode_", "Texture Clamp")
    , texBorderIntensity_("textureBorderIntensity", "Texture Border Intensity", 0.f)
    , applyColorModulation_("applyColorModulation", "Apply color modulation", false)
    , sliceShader_(0)
{
    inport_.addCondition(new PortConditionVolumeTypeGL());
    addPort(inport_);
    addPort(outport_);

    addProperty(transferFunc_);
    addProperty(applyColorModulation_);

    // texture mode (2D/3D)
    texMode_.addOption("2d-texture", "2D Textures", TEXTURE_2D);
    texMode_.addOption("3d-texture", "3D Texture", TEXTURE_3D);
    texMode_.selectByKey("3d-texture");
    addProperty(texMode_);

    // texture filtering
    texFilterMode_.addOption("nearest", "Nearest",  GL_NEAREST);
    texFilterMode_.addOption("linear",  "Linear",   GL_LINEAR);
    texFilterMode_.selectByKey("linear");
    addProperty(texFilterMode_);

    // volume texture clamping
    texClampMode_.addOption("clamp",           "Clamp",             GL_CLAMP);
    texClampMode_.addOption("clamp-to-edge",   "Clamp to Edge",     GL_CLAMP_TO_EDGE);
    texClampMode_.addOption("clamp-to-border", "Clamp to Border",   GL_CLAMP_TO_BORDER);
    texClampMode_.selectByKey("clamp-to-edge");
    addProperty(texClampMode_);
    addProperty(texBorderIntensity_);

    // assign texture access properties to property group
    texMode_.setGroupID("textureAccess");
    texFilterMode_.setGroupID("textureAccess");
    texClampMode_.setGroupID("textureAccess");
    texBorderIntensity_.setGroupID("textureAccess");
    setPropertyGroupGuiName("textureAccess", "Texture Access");

    texClampMode_.onChange(CallMemberAction<SliceRendererBase>(this, &SliceRendererBase::adjustPropertyVisibilities));
}

void SliceRendererBase::initialize() throw (tgt::Exception) {
    VolumeRenderer::initialize();

    adjustPropertyVisibilities();
}

void SliceRendererBase::deinitialize() throw (tgt::Exception) {
    ShdrMgr.dispose(sliceShader_);
    sliceShader_ = 0;

    VolumeRenderer::deinitialize();
}

// protected methods
//

void SliceRendererBase::beforeProcess() {
    if (inport_.hasChanged())
        adjustPropertyVisibilities();

    if (invalidationLevel_ >= Processor::INVALID_PROGRAM)
        rebuildShader();
}

bool SliceRendererBase::setupVolumeShader(const VolumeHandleBase* volume, TextureUnit* volUnit, TextureUnit* transferUnit,
    const tgt::Camera* camera, const tgt::vec4& lightPosition) {

    // if we don't have hardware support -> return
    if (!GpuCaps.areShadersSupported())
        return false;

    if (!sliceShader_) {
        try {
            sliceShader_ = ShdrMgr.load("sl_base", buildShaderHeader(), false);
            LGL_ERROR;
        } 
        catch(tgt::Exception e) {
            LERROR("Failed to load shader: " << e.what());
            return false;
        }
    }

    // activate the shader if everything went fine and set the needed uniforms
    if (sliceShader_) {
        sliceShader_->activate();

        if (transferUnit)
            transferFunc_.get()->setUniform(sliceShader_, "transferFunc_", transferUnit->getUnitNumber());
        
        sliceShader_->setUniform("numChannels_", volume->getRepresentation<Volume>()->getNumChannels());
        sliceShader_->setUniform("applyColorModulation_", applyColorModulation_.get());

        LGL_ERROR;

        // bind volume
        if (volume) {
            std::vector<VolumeStruct> volumeTextures;
            volumeTextures.push_back(VolumeStruct(
                volume,
                volUnit,
                "volumeStruct_",
                texClampMode_.getValue(),
                tgt::vec4(texBorderIntensity_.get()),
                texFilterMode_.getValue())
                );
            bindVolumes(sliceShader_, volumeTextures, camera, lightPosition);

            LGL_ERROR;
        }
    }

    return (sliceShader_ != 0);
}

bool SliceRendererBase::setupSliceShader(tgt::TextureUnit* /*texUnit*/, tgt::TextureUnit* transferUnit, float bitScale) {
    // if we don't have hardware support -> return
    if (!GpuCaps.areShadersSupported())
        return false;

    if (!sliceShader_) {
        try {
            sliceShader_ = ShdrMgr.load("sl_base", buildShaderHeader(), false);
            LGL_ERROR;
        } 
        catch(tgt::Exception e) {
            LERROR("Failed to load shader: " << e.what());
            return false;
        }
    }

    // activate the shader if everything went fine and set the needed uniforms
    if (sliceShader_) {
        sliceShader_->activate();

        if (transferUnit)
            transferFunc_.get()->setUniform(sliceShader_, "transferFunc_", transferUnit->getUnitNumber());

        sliceShader_->setUniform("applyColorModulation_", applyColorModulation_.get());

        sliceShader_->setUniform("sliceBitScale", bitScale);

        LGL_ERROR;
    }

    return (sliceShader_ != 0);
}

bool SliceRendererBase::bindSliceTexture(const tgt::Texture* sliceTexture, tgt::TextureUnit* texUnit) {
    tgtAssert(sliceTexture, "null pointer passed");
    tgtAssert(texUnit, "null pointer passed");
    
    // if we don't have hardware support -> return
    if (!GpuCaps.areShadersSupported())
        return false;

    // check state of slice shader
    if (!sliceShader_) {
        LERROR("no slice shader");
        return false;
    }
    if (!sliceShader_->isActivated()) {
        LERROR("slice shader not activated");
        return false;
    }

    // pass number of texture channels to shader
    int numChannels = static_cast<int>(sliceTexture->getNumChannels());
    sliceShader_->setUniform("numChannels_", numChannels);
    
    // bind slicetexture to unit
    texUnit->activate();
    sliceTexture->bind();
    sliceShader_->setUniform("sliceTex_", texUnit->getUnitNumber());

    LGL_ERROR;

    // pass TEXTURE_PARAMETERS struct values to shader
    sliceShader_->setIgnoreUniformLocationError(true);
    sliceShader_->setUniform("sliceTexParameters_.dimensions_", tgt::vec2(sliceTexture->getDimensions().xy()));
    sliceShader_->setUniform("sliceTexParameters_.dimensionsRCP_", tgt::vec2(1.0f) / tgt::vec2(sliceTexture->getDimensions().xy()));
    sliceShader_->setUniform("sliceTexParameters_.matrix_", tgt::mat4::identity);
    sliceShader_->setIgnoreUniformLocationError(false);

    LGL_ERROR;
    
    return true;
}


std::string SliceRendererBase::buildShaderHeader() {
    std::string header = VolumeRenderer::generateHeader();

    if (texMode_.isSelected("2d-texture"))
        header += "#define SLICE_TEXTURE_MODE_2D \n";
    else if (texMode_.isSelected("3d-texture"))
        header += "#define SLICE_TEXTURE_MODE_3D \n";
    else {
        LWARNING("Unknown texture mode: " << texMode_.get());
    }

    header += transferFunc_.get()->getShaderDefines();
    
    return header;
}

bool SliceRendererBase::rebuildShader() {
    // do nothing if there is no shader at the moment
    if (!sliceShader_)
        return false;

    sliceShader_->setHeaders(buildShaderHeader());
    return sliceShader_->rebuild();
}

void SliceRendererBase::deactivateShader() {
    if (sliceShader_ && sliceShader_->isActivated())
        sliceShader_->deactivate();
}

void SliceRendererBase::adjustPropertyVisibilities() {
    texBorderIntensity_.setVisible(!texClampMode_.isSelected("clamp-to-edge"));
    if (! inport_.isReady())
        applyColorModulation_.setVisible(false);
    else {
        const Volume* vol = inport_.getData()->getRepresentation<Volume>();
        applyColorModulation_.setVisible(vol != 0 && vol->getNumChannels() == 3);
    }
}

tgt::Texture* SliceRendererBase::generateAlignedSliceTexture(const VolumeHandleBase* volumeHandle, 
        const tgt::vec3& texLowerLeft, const tgt::vec3& texUpperRight) const {

    // get cpu volume
    if (!volumeHandle) {
        LERROR("generateSliceTexture(): volumeHandle must not be null");
        return 0;
    }
    const Volume* volume = volumeHandle->getRepresentation<Volume>();
    if (!volume) {
        LERROR("generateSliceTexture(): no CPU representation");
        return 0;
    }
    tgt::ivec3 volDim = volume->getDimensions();
    
    // check range of texcoord parameters
    if (!tgt::hand(tgt::greaterThanEqual(texLowerLeft, tgt::vec3(0.f))) ||
        !tgt::hand(tgt::lessThanEqual(texLowerLeft, tgt::vec3(1.f))) ) {
            LERROR("generateSliceTexture(): texLowerLeft outside valid range [0.0;1.0]^3");
            return 0;
    }
    if (!tgt::hand(tgt::greaterThanEqual(texUpperRight, tgt::vec3(0.f))) ||
        !tgt::hand(tgt::lessThanEqual(texUpperRight, tgt::vec3(1.f))) ) {
            LERROR("generateSliceTexture(): texUpperRight outside valid range [0.0;1.0]^3");
            return 0;
    }
    
    // convert float tex coords to voxel indices
    tgt::ivec3 voxelLowerLeft = tgt::iround(texLowerLeft*tgt::vec3(volDim-1)); 
    voxelLowerLeft = tgt::clamp(voxelLowerLeft, tgt::ivec3(0), volDim-1);
    tgt::ivec3 voxelUpperRight = tgt::iround(texUpperRight*tgt::vec3(volDim-1)); 
    voxelUpperRight = tgt::clamp(voxelUpperRight, tgt::ivec3(0), volDim-1);

    // determine slice orientation and position from voxel LF/UR
    // and generate slice texture
    SliceAlignment sliceAlign;
    tgt::ivec3 sliceTexDim;
    size_t sliceID;
    bool flipX = false;
    bool flipY = false;
    if (voxelLowerLeft.x == voxelUpperRight.x){
        // Y-Z-Slice
        sliceAlign = SliceRendererBase::YZ_PLANE;
        sliceTexDim = tgt::ivec3(volDim.yz(), 1);
        sliceID = voxelLowerLeft.x;
        flipX = voxelLowerLeft.y > voxelUpperRight.y;
        flipY = voxelLowerLeft.z > voxelUpperRight.z;
    }
    else if (voxelLowerLeft.y == voxelUpperRight.y) {
        // X-Z-Slice
        sliceAlign = SliceRendererBase::XZ_PLANE;
        sliceTexDim = tgt::ivec3(volDim.x, volDim.z, 1);
        sliceID = voxelLowerLeft.y;
        flipX = voxelLowerLeft.x > voxelUpperRight.x;
        flipY = voxelLowerLeft.z > voxelUpperRight.z;
    }
    else if (voxelLowerLeft.z == voxelUpperRight.z) {
        // X-Y-Slice
        sliceAlign = SliceRendererBase::XY_PLANE;
        sliceTexDim = tgt::ivec3(volDim.xy(), 1);
        sliceID = voxelLowerLeft.z;
        flipX = voxelLowerLeft.x > voxelUpperRight.x;
        flipY = voxelLowerLeft.y > voxelUpperRight.y;
    }
    else {
        LERROR("generateSliceTexture(): slice " << voxelLowerLeft << "," << voxelUpperRight 
            << " is not axis-aligned");
        return 0;
    }

    // create slice texture of the same type as the volume texture, and copy over tex data to slice
    tgt::Texture* sliceTex = 0;
    // scalar
    if (dynamic_cast<const VolumeAtomic<uint8_t>*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_ALPHA, GL_ALPHA8, GL_UNSIGNED_BYTE);
        LGL_ERROR;
        copySliceData<uint8_t>(static_cast<const VolumeAtomic<uint8_t>*>(volume), sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<int8_t>*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_ALPHA, GL_ALPHA8, GL_BYTE);
        LGL_ERROR;
        copySliceData<int8_t>(static_cast<const VolumeAtomic<int8_t>*>(volume), sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<uint16_t>*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_ALPHA, GL_ALPHA16, GL_UNSIGNED_SHORT);
        LGL_ERROR;
        copySliceData<uint16_t>(static_cast<const VolumeAtomic<uint16_t>*>(volume), sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<int16_t>*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_ALPHA, GL_ALPHA16, GL_SHORT);
        LGL_ERROR;
        copySliceData<int16_t>(static_cast<const VolumeAtomic<int16_t>*>(volume), sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<uint32_t>*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_INT);
        LGL_ERROR;
        copySliceData<uint32_t>(static_cast<const VolumeAtomic<uint32_t>*>(volume), sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<int32_t>*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_ALPHA, GL_ALPHA, GL_INT);
        LGL_ERROR;
        copySliceData<int32_t>(static_cast<const VolumeAtomic<int32_t>*>(volume), sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<uint64_t>*>(volume)) {
        LERROR("Texture data type 'uint64' not supported by OpenGL");
    }
    else if (dynamic_cast<const VolumeAtomic<int64_t>*>(volume)) {
        LERROR("Texture data type 'int64' not supported by OpenGL");
    }
    else if (dynamic_cast<const VolumeAtomic<float>*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_ALPHA, GL_ALPHA, GL_FLOAT);
        LGL_ERROR;
        copySliceData<float>(static_cast<const VolumeAtomic<float>*>(volume), sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<double>*>(volume)) {
        LERROR("Texture data type 'double' not supported by OpenGL");
    }
    // vec2
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<uint8_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE);
        LGL_ERROR;
        copySliceData<tgt::Vector2<uint8_t> >(static_cast<const VolumeAtomic<tgt::Vector2<uint8_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<int8_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_BYTE);
        LGL_ERROR;
        copySliceData<tgt::Vector2<int8_t> >(static_cast<const VolumeAtomic<tgt::Vector2<int8_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<uint16_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_UNSIGNED_SHORT);
        LGL_ERROR;
        copySliceData<tgt::Vector2<uint16_t> >(static_cast<const VolumeAtomic<tgt::Vector2<uint16_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<int16_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_SHORT);
        LGL_ERROR;
        copySliceData<tgt::Vector2<int16_t> >(static_cast<const VolumeAtomic<tgt::Vector2<int16_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<uint32_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_UNSIGNED_INT);
        LGL_ERROR;
        copySliceData<tgt::Vector2<uint32_t> >(static_cast<const VolumeAtomic<tgt::Vector2<uint32_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<int32_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_INT);
        LGL_ERROR;
        copySliceData<tgt::Vector2<int32_t> >(static_cast<const VolumeAtomic<tgt::Vector2<int32_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<uint64_t> >*>(volume)) {
        LERROR("Texture data type 'uint64' not supported by OpenGL");
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<int64_t> >*>(volume)) {
        LERROR("Texture data type 'int64' not supported by OpenGL");
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<float> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, GL_FLOAT);
        LGL_ERROR;
        copySliceData<tgt::Vector2<float> >(static_cast<const VolumeAtomic<tgt::Vector2<float> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector2<double> >*>(volume)) {
        LERROR("Texture data type 'double' not supported by OpenGL");
    }
    // vec3
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<uint8_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE);
        LGL_ERROR;
        copySliceData<tgt::Vector3<uint8_t> >(static_cast<const VolumeAtomic<tgt::Vector3<uint8_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<int8_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGB, GL_RGB8, GL_BYTE);
        LGL_ERROR;
        copySliceData<tgt::Vector3<int8_t> >(static_cast<const VolumeAtomic<tgt::Vector3<int8_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<uint16_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGB, GL_RGB16, GL_UNSIGNED_SHORT);
        LGL_ERROR;
        copySliceData<tgt::Vector3<uint16_t> >(static_cast<const VolumeAtomic<tgt::Vector3<uint16_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<int16_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGB, GL_RGB16, GL_SHORT);
        LGL_ERROR;
        copySliceData<tgt::Vector3<int16_t> >(static_cast<const VolumeAtomic<tgt::Vector3<int16_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<uint32_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGB, GL_RGB, GL_UNSIGNED_INT);
        LGL_ERROR;
        copySliceData<tgt::Vector3<uint32_t> >(static_cast<const VolumeAtomic<tgt::Vector3<uint32_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<int32_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGB, GL_RGB, GL_INT);
        LGL_ERROR;
        copySliceData<tgt::Vector3<int32_t> >(static_cast<const VolumeAtomic<tgt::Vector3<int32_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<uint64_t> >*>(volume)) {
        LERROR("Texture data type 'uint64' not supported by OpenGL");
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<int64_t> >*>(volume)) {
        LERROR("Texture data type 'int64' not supported by OpenGL");
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<float> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGB, GL_RGB, GL_FLOAT);
        LGL_ERROR;
        copySliceData<tgt::Vector3<float> >(static_cast<const VolumeAtomic<tgt::Vector3<float> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector3<double> >*>(volume)) {
        LERROR("Texture data type 'double' not supported by OpenGL");
    }
    // vec4
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<uint8_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE);
        LGL_ERROR;
        copySliceData<tgt::Vector4<uint8_t> >(static_cast<const VolumeAtomic<tgt::Vector4<uint8_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<int8_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGBA, GL_RGBA8, GL_BYTE);
        LGL_ERROR;
        copySliceData<tgt::Vector4<int8_t> >(static_cast<const VolumeAtomic<tgt::Vector4<int8_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<uint16_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGBA, GL_RGBA16, GL_UNSIGNED_SHORT);
        LGL_ERROR;
        copySliceData<tgt::Vector4<uint16_t> >(static_cast<const VolumeAtomic<tgt::Vector4<uint16_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<int16_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGBA, GL_RGBA16, GL_SHORT);
        LGL_ERROR;
        copySliceData<tgt::Vector4<int16_t> >(static_cast<const VolumeAtomic<tgt::Vector4<int16_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<uint32_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGBA, GL_RGBA, GL_UNSIGNED_INT);
        LGL_ERROR;
        copySliceData<tgt::Vector4<uint32_t> >(static_cast<const VolumeAtomic<tgt::Vector4<uint32_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<int32_t> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGBA, GL_RGBA, GL_INT);
        LGL_ERROR;
        copySliceData<tgt::Vector4<int32_t> >(static_cast<const VolumeAtomic<tgt::Vector4<int32_t> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<uint64_t> >*>(volume)) {
        LERROR("Texture data type 'uint64' not supported by OpenGL");
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<int64_t> >*>(volume)) {
        LERROR("Texture data type 'int64' not supported by OpenGL");
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<float> >*>(volume)) {
        sliceTex = new tgt::Texture(sliceTexDim, GL_RGBA, GL_RGBA, GL_FLOAT);
        LGL_ERROR;
        copySliceData<tgt::Vector4<float> >(static_cast<const VolumeAtomic<tgt::Vector4<float> >*>(volume), 
            sliceTex, sliceAlign, sliceID, flipX, flipY);
    }
    else if (dynamic_cast<const VolumeAtomic<tgt::Vector4<double> >*>(volume)) {
        LERROR("Texture data type 'double' not supported by OpenGL");
    }
    else {
        LERROR("generateSliceTexture(): unknown or unsupported volume type");
    }

    LGL_ERROR;
    return sliceTex;
}

}   // namespace voreen


namespace {

// template definitions
template<typename T>
void copySliceData(const voreen::VolumeAtomic<T>* volume, tgt::Texture* sliceTexture,
        voreen::SliceRendererBase::SliceAlignment sliceAlign, size_t sliceID, bool flipX, bool flipY) {

    tgtAssert(volume, "volume is null");
    tgtAssert(sliceTexture, "sliceTexture is null");
    tgtAssert(sliceTexture->getPixelData(), "sliceTexture pixel data is null");
    tgtAssert(sliceTexture->getDimensions().z == 1, "2D texture expected");

    tgt::ivec3 volDim = volume->getDimensions();
    tgt::ivec2 sliceDim = sliceTexture->getDimensions().xy();
    switch (sliceAlign) {
    case voreen::SliceRendererBase::YZ_PLANE: 
        {
            tgtAssert(volDim.yz() == sliceDim, "dimensions mismatch");
            tgtAssert(static_cast<int>(sliceID) < volDim.x, "invalid slice id");
            for (size_t y=0; y<static_cast<size_t>(sliceDim.y); y++) {
                for (size_t x=0; x<static_cast<size_t>(sliceDim.x); x++) {
                    sliceTexture->texel<T>(x, y) = volume->voxel(sliceID, 
                        !flipX ? x : (sliceDim.x-1) - x, !flipY ? y : (sliceDim.y-1) - y);
                }
            }
        }
        break;
    case voreen::SliceRendererBase::XZ_PLANE: 
        {
            tgtAssert(volDim.x == sliceDim.x && volDim.z == sliceDim.y, "dimensions mismatch");
            tgtAssert(static_cast<int>(sliceID) < volDim.y, "invalid slice id");
            for (size_t y=0; y<static_cast<size_t>(sliceDim.y); y++) {
                for (size_t x=0; x<static_cast<size_t>(sliceDim.x); x++) {
                    sliceTexture->texel<T>(x, y) = volume->voxel(!flipX ? x : (sliceDim.x-1) - x, sliceID, 
                        !flipY ? y : (sliceDim.y-1) - y);
                }
            }
        }
        break;
    case voreen::SliceRendererBase::XY_PLANE: 
        {
            tgtAssert(volDim.xy() == sliceDim, "dimensions mismatch");
            tgtAssert(static_cast<int>(sliceID) < volDim.z, "invalid slice id");
            for (size_t y=0; y<static_cast<size_t>(sliceDim.y); y++) {
                for (size_t x=0; x<static_cast<size_t>(sliceDim.x); x++) {
                    sliceTexture->texel<T>(x, y) = volume->voxel(!flipX ? x : (sliceDim.x-1) - x, 
                        !flipY ? y : (sliceDim.y-1) - y, sliceID);
                }
            }
        }
        break;
    default:
        tgtAssert(false, "invalid slice alignment");

    }

}

} // namespace anonymous

