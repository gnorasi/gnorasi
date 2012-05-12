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

 #define MOD_SAMPLER3D

/*
 * This struct contains a volume and information about it, as
 * its dimensions and spacing. Additionally, the reciprocal
 * values of all parameters are available (suffix RCP) .
 * The values are set automatically by
 * VolumeRenderer::bindVolumes() if necessary.
 */
struct VOLUME_STRUCT {
    sampler3D volume_;              // the actual dataset

    vec3 datasetDimensions_;        // the dataset's resolution, e.g. [ 256.0, 128.0, 128.0]
    vec3 datasetDimensionsRCP_;

    vec3 datasetSpacing_;           // set dataset's voxel size, e.g. [ 2.0, 0.5, 1.0]
    vec3 datasetSpacingRCP_;

    vec3 volumeCubeSize_;          // the volume's size in physical coordinates, e.g. [ 1.0, 0.5, 0.5]
    vec3 volumeCubeSizeRCP_;

    vec3 volumeOffset_;              // see VolumeHandle::getOffset()
    vec3 volumeTextureTranslation_;  // translation for scaling of texture coordinates when performing slicing
    
    vec3 volumeBorderOffset_;    // the offset to add to texCoords based on border of faces connected to [0.0,0.0,0.0]
    vec3 volumeBorderScaling_;  // the scaling to multiply to texCoords based on border of faces connected to [1.0,1.0,1.0] 
    bool volumeHasBorder_;      // true if volume has border

    int bitDepth_;                  // the volume's bit depth
    float bitDepthScale_;           // scaling factor that must be applied for normalizing the fetched texture value.
                                    // currently just used for 12 bit volumes, which actually use only 12 out of 16 bits.

    float rwmScale_;                // RealWorldMapping slope
    float rwmOffset_;               // RealWorldMapping intercept 

    int numChannels_;

    mat4 volumeTransformation_;     // dataset transformation matrix (see Volume)
    mat4 volumeTransformationINV_;  // inverse dataset transformation matrix

    vec3 cameraPositionOBJ_;        // camera position in volume object coordinates (see mod_shading.frag)
    vec3 lightPositionOBJ_;         // light position in volume object coordinates (see mod_shading.frag)
};

/*
 * Function to apply correct texture coordinates when for instance using borders
 */
vec3 getCorrectTextureCoordinate(VOLUME_STRUCT volumeStruct, vec3 texCoords){
/*
 * IGNORE_VOLUME_STRUCT_UNIFORMS_WHEN_SAMPLING is a needed define for the MultiVolumeRaycaster as the 
 * VOLUME_STRUCT uniforms are not valid to use in the mod_sampler3D shader, when multiple volumes are attached.
 * This is a problem only on Nvidia Driver version larger than 295.73, tested to 296.10.
*/
#if defined(IGNORE_VOLUME_STRUCT_UNIFORMS_WHEN_SAMPLING)
    return texCoords;
#else
    return volumeStruct.volumeBorderOffset_ + texCoords*volumeStruct.volumeBorderScaling_;
#endif
}

/*
 * Function for volume texture lookup. In addition to the texture coordinates
 * the corresponding VOLUME_STRUCT has to be passed.
 * Before returning the fetched value it is normalized to the interval [0,1], in order to deal
 * with 12 bit data sets.
 */
vec4 textureLookup3D(VOLUME_STRUCT volumeStruct, vec3 texCoords) {
    vec4 result;
#if defined(GLSL_VERSION_130)
    result = texture(volumeStruct.volume_, getCorrectTextureCoordinate(volumeStruct, texCoords));
#else
    result = texture3D(volumeStruct.volume_, getCorrectTextureCoordinate(volumeStruct, texCoords));
#endif
    result *= volumeStruct.bitDepthScale_;
    result.a *= volumeStruct.rwmScale_;
    result.a += volumeStruct.rwmOffset_;
    return result;
}

/*
 * DEPRECATED
 */
vec4 textureLookup3D(sampler3D volume, VOLUME_STRUCT volumeStruct, vec3 texCoords) {
    return textureLookup3D(volumeStruct, texCoords);
}

/*
 * Function for volume texture lookup. In addition to the texture coordinates
 * the corresponding VOLUME_STRUCT has to be passed .
 * In contrast to textureLookup3D() this function does not normalize the intensity values,
 * in order to deal with 12 bit data sets.
 */
vec4 textureLookup3DUnnormalized(VOLUME_STRUCT volumeStruct, vec3 texCoords) {
#if defined(GLSL_VERSION_130)
    return texture(volumeStruct.volume_, getCorrectTextureCoordinate(volumeStruct, texCoords));
#else
    return texture3D(volumeStruct.volume_, getCorrectTextureCoordinate(volumeStruct, texCoords));
#endif
}

/*
 * DEPRECATED
 */
vec4 textureLookup3DUnnormalized(sampler3D volume, VOLUME_STRUCT volumeStruct, vec3 texCoords) {
    return textureLookup3DUnnormalized(volumeStruct, texCoords);
}


/*
 * This function should be called by all raycasters in order to get the intensity from the volume.
 * In cases where volumeStruct indicates, that gradients are stored in the volume, these are
 * also fetched from the volume. Therefore, in addition to the volume and the texture coordinates
 * the corresponding VOLUME_STRUCT has to be passed.
 * Before returning the intensity value it is normalized to the interval [0,1], in order to deal
 * with 12 bit data sets.
 *
 * @return The result vec4 stores the intensity in the a channel and the gradient (if available)
 *         in the rgb channels.
 *
 */
vec4 getVoxel(VOLUME_STRUCT volumeStruct, vec3 sampleOrg) {
    return textureLookup3D(volumeStruct.volume_, volumeStruct, sampleOrg);
}

/*
 * DEPRECATED
 */
vec4 getVoxel(sampler3D volume, VOLUME_STRUCT volumeStruct, vec3 sampleOrg) {
    return getVoxel(volumeStruct, sampleOrg);
}


/*
 * This function should be called by all raycasters in order to get the intensity from the volume.
 * In cases where volumeStruct indicates, that gradients are stored in the volume, these are
 * also fetched from the volume. Therefore, in addition to the volume and the texture coordinates
 * the corresponding VOLUME_STRUCT has to be passed.
 * In contrast to getVoxel() this function does not normalize the intensity values, in order to deal
 * with 12 bit data sets.
 *
 * @return The result vec4 stores the intensity in the a channel and the gradient (if available)
 *         in the rgb channels.
 */
vec4 getVoxelUnnormalized(VOLUME_STRUCT volumeStruct, vec3 sampleOrg) {
    return textureLookup3DUnnormalized(volumeStruct.volume_, volumeStruct, sampleOrg);
}

/*
 * DEPRECATED
 */
vec4 getVoxelUnnormalized(sampler3D volume, VOLUME_STRUCT volumeStruct, vec3 sampleOrg) {
    return getVoxelUnnormalized(volumeStruct, sampleOrg);
}

vec3 texToPhysical(vec3 samplePos, VOLUME_STRUCT volumeParams) {
    return ((samplePos*volumeParams.datasetDimensions_)*volumeParams.datasetSpacing_)+volumeParams.volumeOffset_;
}

vec3 physicalToTex(vec3 samplePos, VOLUME_STRUCT volumeStruct) {
    return ((samplePos - volumeStruct.volumeOffset_) * volumeStruct.datasetSpacingRCP_) * volumeStruct.datasetDimensionsRCP_;
}

vec3 worldToTex(vec3 worldSamplePos, VOLUME_STRUCT volumeStruct) {
    //From world space to physical space:
    vec3 samplePos = (volumeStruct.volumeTransformationINV_ * vec4(worldSamplePos, 1.0)).xyz;

    return physicalToTex(samplePos, volumeStruct);
}

vec3 texToWorld(vec3 texSamplePos, VOLUME_STRUCT volumeStruct) {
    vec3 samplePos = texToPhysical(texSamplePos, volumeStruct);

    //From physical space to world space:
    return (volumeStruct.volumeTransformation_ * vec4(samplePos, 1.0)).xyz;
}

bool inUnitCube(vec3 sample_val) {
    if(sample_val.x > 1.0 || sample_val.x < 0.0 || sample_val.y > 1.0 || sample_val.y < 0.0 || sample_val.z > 1.0 || sample_val.z < 0.0)
        return false;
    return true;
}

bool onBorder(vec3 samplePos, VOLUME_STRUCT volumeStruct) {
    if(volumeStruct.volumeHasBorder_){
        if(samplePos.x == 1.0 || samplePos.x == 0.0 || samplePos.y == 1.0 || samplePos.y == 0.0 || samplePos.z == 1.0 || samplePos.z == 0.0){
            vec3 texCoord = getCorrectTextureCoordinate(volumeStruct, samplePos);
            if(texCoord.x == 1.0 || texCoord.x == 0.0 || texCoord.y == 1.0 || texCoord.y == 0.0 || texCoord.z == 1.0 || texCoord.z == 0.0)
                return true;
        }
    }
    return false;
}
