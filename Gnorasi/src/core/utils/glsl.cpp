/***********************************************************************************
 *                                                                                 *
 * Voreen - The Volume Rendering Engine                                            *
 *                                                                                 *
 * Copyright (C) 2005-2012 University of Muenster, Germany.                        *
 * Visualization and Computer Graphics Group <http://viscg.uni-muenster.de>        *
 * For a list of authors please refer to the file "CREDITS.txt".                   *
 *                                                                                 *
 * This file is part of the Voreen software package. Voreen is free software:      *
 * you can redistribute it and/or modify it under the terms of the GNU General     *
 * Public License version 2 as published by the Free Software Foundation.          *
 *                                                                                 *
 * Voreen is distributed in the hope that it will be useful, but WITHOUT ANY       *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR   *
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.      *
 *                                                                                 *
 * You should have received a copy of the GNU General Public License in the file   *
 * "LICENSE.txt" along with this file. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                                 *
 * For non-commercial academic use see the license exception specified in the file *
 * "LICENSE-academic.txt". To get information about commercial licensing please    *
 * contact the authors.                                                            *
 *                                                                                 *
 ***********************************************************************************/

#include "voreen/core/utils/glsl.h"

#include "voreen/core/datastructures/volume/volumegl.h"

namespace voreen {

using tgt::vec3;

void setUniform(tgt::Shader* shader, const std::string& volumeUniform, const std::string& structUniform, const VolumeBase* vh, const tgt::TextureUnit* texUnit, const tgt::Camera* camera, const tgt::vec4& lightPosition) {
    if(texUnit)
        shader->setUniform(volumeUniform, texUnit->getUnitNumber());

    // volume size, i.e. dimensions of the proxy geometry in world coordinates
    shader->setUniform(structUniform + ".datasetDimensions_", tgt::vec3(vh->getDimensions()));
    shader->setUniform(structUniform + ".datasetDimensionsRCP_", vec3(1.f) / tgt::vec3(vh->getDimensions()));

    // volume spacing, i.e. voxel size
    shader->setUniform(structUniform + ".datasetSpacing_", vh->getSpacing());
    shader->setUniform(structUniform + ".datasetSpacingRCP_", vec3(1.f) / vh->getSpacing());

    // volume's size in its physical coordinates
    shader->setUniform(structUniform + ".volumeCubeSize_", vh->getCubeSize());
    shader->setUniform(structUniform + ".volumeCubeSizeRCP_", vec3(1.f) / vh->getCubeSize());

    shader->setUniform(structUniform + ".volumeOffset_", vh->getOffset());

    shader->setUniform(structUniform + ".numChannels_", static_cast<GLint>(vh->getNumChannels()));

    // volume's transformation matrix
    shader->setUniform(structUniform + ".physicalToWorldMatrix_", vh->getPhysicalToWorldMatrix());

    tgt::mat4 invTm = vh->getWorldToPhysicalMatrix();
    shader->setUniform(structUniform + ".worldToPhysicalMatrix_", invTm);

    shader->setUniform(structUniform + ".worldToTextureMatrix_", vh->getWorldToTextureMatrix());
    shader->setUniform(structUniform + ".textureToWorldMatrix_", vh->getTextureToWorldMatrix());

    // camera position in volume object coords
    if (camera)
        shader->setUniform(structUniform + ".cameraPositionPhysical_", invTm*camera->getPosition());

    // light position in volume object coords
    shader->setUniform(structUniform + ".lightPositionPhysical_", (invTm*lightPosition).xyz());

    LGL_ERROR;

    // bit depth of the volume
    shader->setUniform(structUniform + ".bitDepth_", (GLint)(vh->getBytesPerVoxel() * 8));

    // construct shader real-world mapping by combining volume rwm and pixel transfer mapping
    RealWorldMapping rwm = vh->getRealWorldMapping();
    RealWorldMapping transferMapping;
    if (vh->getRepresentation<VolumeGL>())
        transferMapping = vh->getRepresentation<VolumeGL>()->getPixelTransferMapping();
    else
        LWARNINGC("voreen.glsl", "setUniform(): no VolumeGL");
    RealWorldMapping shaderMapping = RealWorldMapping::combine(transferMapping.getInverseMapping(), rwm);
    shader->setUniform(structUniform + ".rwmScale_", shaderMapping.getScale());
    shader->setUniform(structUniform + ".rwmOffset_", shaderMapping.getOffset());
}

bool bindVolumeTexture(const VolumeBase* vh, const tgt::TextureUnit* texUnit, GLint filterMode, GLint wrapMode, tgt::vec4 borderColor) {
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
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrapMode);
    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, static_cast<tgt::Vector4<GLfloat> >(borderColor).elem);
    LGL_ERROR;

    texUnit->setZeroUnit();

    return true;
}

void setUniform(tgt::Shader* shader, const std::string& imageUniform, const std::string& structUniform, const Slice* sl, const tgt::TextureUnit* texUnit) {
    bool ignoreErr = shader->getIgnoreUniformLocationError();
    shader->setIgnoreUniformLocationError(true);

    if(texUnit)
        shader->setUniform(imageUniform, texUnit->getUnitNumber());

    // volume size, i.e. dimensions of the proxy geometry in world coordinates
    shader->setUniform(structUniform + ".datasetDimensions_", tgt::vec3(sl->getTexture()->getDimensions()));
    shader->setUniform(structUniform + ".datasetDimensionsRCP_", vec3(1.f) / tgt::vec3(sl->getTexture()->getDimensions()));

    // volume spacing, i.e. voxel size
    //shader->setUniform(structUniform + ".datasetSpacing_", sl->getSpacing());
    //shader->setUniform(structUniform + ".datasetSpacingRCP_", vec3(1.f) / sl->getSpacing());

    shader->setUniform(structUniform + ".numChannels_", static_cast<GLint>(sl->getTexture()->getNumChannels()));
    //shader->setUniform(structUniform + ".numChannels_", static_cast<GLint>(1));

    // volume's transformation matrix
    //shader->setUniform(structUniform + ".physicalToWorldMatrix_", sl->getPhysicalToWorldMatrix());

    //tgt::mat4 invTm = sl->getWorldToPhysicalMatrix();
    //shader->setUniform(structUniform + ".worldToPhysicalMatrix_", invTm);

    shader->setUniform(structUniform + ".worldToTextureMatrix_", sl->getWorldToTextureMatrix());
    shader->setUniform(structUniform + ".textureToWorldMatrix_", sl->getTextureToWorldMatrix());

    LGL_ERROR;

    // construct shader real-world mapping by combining volume rwm and pixel transfer mapping
    RealWorldMapping rwm = sl->getRealWorldMapping();
    //RealWorldMapping transferMapping;
    //if (sl->getRepresentation<VolumeGL>())
        //transferMapping = sl->getRepresentation<VolumeGL>()->getPixelTransferMapping();
    //else
        //LWARNINGC("voreen.glsl", "setUniform(): no VolumeGL");
    //RealWorldMapping shaderMapping = RealWorldMapping::combine(transferMapping.getInverseMapping(), rwm);
    RealWorldMapping shaderMapping = rwm;
    shader->setUniform(structUniform + ".rwmScale_", shaderMapping.getScale());
    shader->setUniform(structUniform + ".rwmOffset_", shaderMapping.getOffset());

    shader->setIgnoreUniformLocationError(ignoreErr);
}

} // namespace
