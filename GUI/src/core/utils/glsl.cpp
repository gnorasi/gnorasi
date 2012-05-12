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

#include "voreen/core/utils/glsl.h"

namespace voreen {

using tgt::vec3;

void setUniform(tgt::Shader* shader, const std::string& uniform, const VolumeHandleBase* vh, const tgt::TextureUnit* texUnit, const tgt::Camera* camera, const tgt::vec4& lightPosition) {
    if(texUnit)
        shader->setUniform(uniform + ".volume_", texUnit->getUnitNumber());

    // volume size, i.e. dimensions of the proxy geometry in world coordinates
    tgt::vec3 dims = tgt::vec3(vh->getDimensions());
    shader->setUniform(uniform + ".datasetDimensions_", dims);
    shader->setUniform(uniform + ".datasetDimensionsRCP_", tgt::vec3(1.f) / dims);

    // volume spacing, i.e. voxel size
    tgt::vec3 spacs = vh->getSpacing();
    shader->setUniform(uniform + ".datasetSpacing_", spacs);
    shader->setUniform(uniform + ".datasetSpacingRCP_", tgt::vec3(1.f) / spacs);

    // volume's size in its physical coordinates
    tgt::vec3 cubeSize = vh->getCubeSize();
    shader->setUniform(uniform + ".volumeCubeSize_", cubeSize);
    shader->setUniform(uniform + ".volumeCubeSizeRCP_", tgt::vec3(1.f) / cubeSize);

    // volume offset and texture translation for slicing
    tgt::vec3 offset = vh->getOffset();
    shader->setUniform(uniform + ".volumeOffset_", offset);
    tgt::vec3 orgDims = static_cast<tgt::vec3>(vh->getOriginalDimensions());
    tgt::vec3 dimSpac = orgDims*spacs;
    tgt::vec3 originOffset = -(dimSpac / tgt::max(dimSpac));
    originOffset *= (dims/orgDims);
    tgt::vec3 textureTranslation = ((originOffset-offset)/cubeSize) + tgt::vec3(0.5f);
    shader->setUniform(uniform + ".volumeTextureTranslation_", textureTranslation);

    // volume borders
    tgt::vec3 llf = tgt::vec3(vh->getBorderLLF());
    tgt::vec3 urb = tgt::vec3(vh->getBorderURB());
    tgt::vec3 borderOffset = llf / (dims + llf + urb);
    tgt::vec3 borderScaling = dims / (dims + llf + urb);
    shader->setUniform(uniform + ".volumeBorderOffset_", borderOffset);
    shader->setUniform(uniform + ".volumeBorderScaling_", borderScaling);
    shader->setUniform(uniform + ".volumeHasBorder_", vh->hasBorder());

    shader->setUniform(uniform + ".numChannels_", static_cast<GLint>(vh->getNumChannels()));

    // volume's transformation matrix
    shader->setUniform(uniform + ".volumeTransformation_", vh->getPhysicalToWorldMatrix());
    tgt::mat4 invTm = vh->getWorldToPhysicalMatrix();
    shader->setUniform(uniform + ".volumeTransformationINV_", invTm);

    // camera position in volume object coords
    if (camera)
        shader->setUniform(uniform + ".cameraPositionOBJ_", invTm*camera->getPosition());

    // light position in volume object coords
    shader->setUniform(uniform + ".lightPositionOBJ_", (invTm*lightPosition).xyz());

    LGL_ERROR;

    // bit depth of the volume
    shader->setUniform(uniform + ".bitDepth_", vh->getBitsStored());

    // is the volume a 12 bit volume => fetched texel values have to be normalized in the shader
    if (vh->getBitsStored() == 12)
        shader->setUniform(uniform + ".bitDepthScale_", 16.0f);
    else
        shader->setUniform(uniform + ".bitDepthScale_", 1.0f);

    RealWorldMapping rwm = vh->getRealWorldMapping();
    shader->setUniform(uniform + ".rwmScale_", rwm.getScale());
    shader->setUniform(uniform + ".rwmOffset_", rwm.getOffset());
}

bool bindVolumeTexture(const VolumeHandleBase* vh, const tgt::TextureUnit* texUnit, GLint filterMode, GLenum wrapMode, tgt::vec4 borderColor) {
    const VolumeGL* volumeGL = vh->getRepresentation<VolumeGL>();
    if (!volumeGL || !volumeGL->getTexture()) {
        LWARNINGC("voreen.glsl", "No volume texture while binding volumes");
        return false;
    }
    const VolumeTexture* volumeTex = volumeGL->getTexture();

    texUnit->activate();

    volumeTex->bind();

    // Returns the residence status of the target texture. If the value returned in params is
    // GL_TRUE, the texture is resident in texture memory
    GLint resident;
    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_RESIDENT, &resident);

    if (resident != GL_TRUE)
        LWARNINGC("voreen.glsl", "texture not resident: " /*<< volume->meta().getFileName()*/);

    LGL_ERROR;

    // texture filtering
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filterMode);
    LGL_ERROR;

    // texture wrapping
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapMode));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapMode));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, static_cast<GLint>(wrapMode));
    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, borderColor.elem);
    LGL_ERROR;

    return true;
}

} // namespace
