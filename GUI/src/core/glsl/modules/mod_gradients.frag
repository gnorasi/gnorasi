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

/**
 * This module contains all functions which can be used for calculating
 * gradients on the fly within a raycaster.
 * The functions below are referenced by RC_CALC_GRADIENTS which is
 * used in the raycaster fragment shaders.
 */

/**
 * Calculates a correct gradient for clipping and volume boundaries.
 *
 * @param samplePos the sample's position in texture space
 * @param rayDirection the ray direction
 */
vec3 fixClipBorderGradient(vec3 samplePos, vec3 rayDirection, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 v0 = normalize(textureLookup2D(entryPoints, entryParameters, vec2(gl_FragCoord.x+2.0, gl_FragCoord.y) ).rgb - samplePos);
    vec3 v1 = normalize(textureLookup2D(entryPoints, entryParameters, vec2(gl_FragCoord.x, gl_FragCoord.y+2.0) ).rgb - samplePos);
    //FIXME: error handling if v0 or v1 is (0,0,0)
    vec3 gradient = cross(v0, v1);
    gradient = faceforward(gradient, rayDirection, gradient);
    return gradient;
}

/**
 * Calculates a voxel's gradient in volume object space based on the red
 * channel using forward differences.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 * @param t the ray parameter, needed to fix gradients on clipping and
 *          volume borders
 * @param rayDirection the ray direction
 */
vec3 calcGradientRFD(VOLUME_STRUCT volumeStruct, vec3 samplePos, float t, vec3 rayDirection, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 gradient;
    if (t == 0.0 && !onBorder(samplePos, volumeStruct))
        gradient = fixClipBorderGradient(samplePos, rayDirection, entryPoints, entryParameters);
    else {
        vec3 offset = volumeStruct.datasetDimensionsRCP_;
        float v = textureLookup3DUnnormalized(volumeStruct, samplePos).r;
        float v0 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(offset.x, 0.0, 0.0)).r;
        float v1 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, offset.y, 0)).r;
        float v2 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, offset.z)).r;
        gradient = vec3(v - v0, v - v1, v - v2);
        gradient *= volumeStruct.datasetSpacingRCP_;
        gradient *= volumeStruct.bitDepthScale_;
    }
    return gradient;
}

/**
 * Calculates a voxel's gradient in volume object space based on the green
 * channel using forward differences.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 * @param t the ray parameter, needed to fix gradients on clipping and
 *          volume borders
 * @param rayDirection the ray direction
 */
vec3 calcGradientGFD(VOLUME_STRUCT volumeStruct, vec3 samplePos, float t, vec3 rayDirection, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 gradient;
    if (t == 0.0 && !onBorder(samplePos, volumeStruct))
        gradient = fixClipBorderGradient(samplePos, rayDirection, entryPoints, entryParameters);
    else {
        vec3 offset = volumeStruct.datasetDimensionsRCP_;
        float v = textureLookup3DUnnormalized(volumeStruct, samplePos).g;
        float v0 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(offset.x, 0.0, 0.0)).g;
        float v1 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, offset.y, 0)).g;
        float v2 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, offset.z)).g;
        gradient = vec3(v - v0, v - v1, v - v2);
        gradient *= volumeStruct.datasetSpacingRCP_;
        gradient *= volumeStruct.bitDepthScale_;
    }
    return gradient;
}

/**
 * Calculates a voxel's gradient in volume object space based on the blue
 * channel using forward differences.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 * @param t the ray parameter, needed to fix gradients on clipping and
 *          volume borders
 * @param rayDirection the ray direction
 */
vec3 calcGradientBFD(VOLUME_STRUCT volumeStruct, vec3 samplePos, float t, vec3 rayDirection, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 gradient;
    if (t == 0.0 && !onBorder(samplePos, volumeStruct))
        gradient = fixClipBorderGradient(samplePos, rayDirection, entryPoints, entryParameters);
    else {
        vec3 offset = volumeStruct.datasetDimensionsRCP_;
        float v = textureLookup3DUnnormalized(volumeStruct, samplePos).b;
        float v0 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(offset.x, 0.0, 0.0)).b;
        float v1 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, offset.y, 0)).b;
        float v2 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, offset.z)).b;
        gradient = vec3(v - v0, v - v1, v - v2);
        gradient *= volumeStruct.datasetSpacingRCP_;
        gradient *= volumeStruct.bitDepthScale_;
    }
    return gradient;
}

/**
 * Calculates a voxel's gradient in volume object space based on the alpha
 * channel using forward differences.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 * @param t the ray parameter, needed to fix gradients on clipping and
 *          volume borders
 * @param rayDirection the ray direction
 */
vec3 calcGradientAFD(VOLUME_STRUCT volumeStruct, vec3 samplePos, float t, vec3 rayDirection, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 gradient;
    if (t == 0.0 && !onBorder(samplePos, volumeStruct))
        gradient = fixClipBorderGradient(samplePos, rayDirection, entryPoints, entryParameters);
    else {
        vec3 offset = volumeStruct.datasetDimensionsRCP_;
        float v = textureLookup3DUnnormalized(volumeStruct, samplePos).a;
        float v0 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(offset.x, 0.0, 0.0)).a;
        float v1 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, offset.y, 0)).a;
        float v2 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, offset.z)).a;
        gradient = vec3(v - v0, v - v1, v - v2);
        gradient *= volumeStruct.datasetSpacingRCP_;
        gradient *= volumeStruct.bitDepthScale_;
    }
    return gradient;
}

/**
 * Calculates a voxel's gradient in volume object space based on the red
 * channel using central differences.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 * @param t the ray parameter, needed to fix gradients on clipping and
 *          volume borders
 * @param rayDirection the ray direction
 */
vec3 calcGradientR(VOLUME_STRUCT volumeStruct, vec3 samplePos, float t, vec3 rayDirection, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 gradient;
    if (t == 0.0 && !onBorder(samplePos, volumeStruct))
        gradient = fixClipBorderGradient(samplePos, rayDirection, entryPoints, entryParameters);
    else {
        vec3 offset = volumeStruct.datasetDimensionsRCP_;
        float v0 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(offset.x, 0.0, 0.0)).r;
        float v1 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, offset.y, 0)).r;
        float v2 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, offset.z)).r;
        float v3 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(-offset.x, 0, 0)).r;
        float v4 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, -offset.y, 0)).r;
        float v5 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, -offset.z)).r;
        gradient = vec3(v3 - v0, v4 - v1, v5 - v2);
        gradient *= (volumeStruct.datasetSpacingRCP_ * 0.5);
        gradient *= volumeStruct.bitDepthScale_;
    }
    return gradient;
}

/**
 * Calculates a voxel's gradient in volume object space based on the green
 * channel using central differences.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 * @param t the ray parameter, needed to fix gradients on clipping and
 *          volume borders
 * @param rayDirection the ray direction
 */
vec3 calcGradientG(VOLUME_STRUCT volumeStruct, vec3 samplePos, float t, vec3 rayDirection, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 gradient;
    if (t == 0.0 && !onBorder(samplePos, volumeStruct))
        gradient = fixClipBorderGradient(samplePos, rayDirection, entryPoints, entryParameters);
    else {
        vec3 offset = volumeStruct.datasetDimensionsRCP_;
        float v0 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(offset.x, 0.0, 0.0)).g;
        float v1 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, offset.y, 0)).g;
        float v2 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, offset.z)).g;
        float v3 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(-offset.x, 0, 0)).g;
        float v4 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, -offset.y, 0)).g;
        float v5 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, -offset.z)).g;
        gradient = vec3(v3 - v0, v4 - v1, v5 - v2);
        gradient *= (volumeStruct.datasetSpacingRCP_ * 0.5);
        gradient *= volumeStruct.bitDepthScale_;
    }
    return gradient;
}

/**
 * Calculates a voxel's gradient in volume object space based on the blue
 * channel using central differences.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 * @param t the ray parameter, needed to fix gradients on clipping and
 *          volume borders
 * @param rayDirection the ray direction
 */
vec3 calcGradientB(VOLUME_STRUCT volumeStruct, vec3 samplePos, float t, vec3 rayDirection, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 gradient;
    if (t == 0.0 && !onBorder(samplePos, volumeStruct))
        gradient = fixClipBorderGradient(samplePos, rayDirection, entryPoints, entryParameters);
    else {
        vec3 offset = volumeStruct.datasetDimensionsRCP_;
        float v0 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(offset.x, 0.0, 0.0)).b;
        float v1 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, offset.y, 0)).b;
        float v2 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, offset.z)).b;
        float v3 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(-offset.x, 0, 0)).b;
        float v4 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, -offset.y, 0)).b;
        float v5 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, -offset.z)).b;
        gradient = vec3(v3 - v0, v4 - v1, v5 - v2);
        gradient *= (volumeStruct.datasetSpacingRCP_ * 0.5);
        gradient *= volumeStruct.bitDepthScale_;
    }
    return gradient;
}

/**
 * Calculates a voxel's gradient in volume object space based on the alpha
 * channel using central differences.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 * @param t the ray parameter, needed to fix gradients on clipping and
 *          volume borders
 * @param rayDirection the ray direction
 */
vec3 calcGradientA(VOLUME_STRUCT volumeStruct, vec3 samplePos, float t, vec3 rayDirection, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 gradient;
    if (t == 0.0 && !onBorder(samplePos, volumeStruct))
        gradient = fixClipBorderGradient(samplePos, rayDirection, entryPoints, entryParameters);
    else {
        vec3 offset = volumeStruct.datasetDimensionsRCP_;
        float v0 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(offset.x, 0.0, 0.0)).a;
        float v1 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, offset.y, 0)).a;
        float v2 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, offset.z)).a;
        float v3 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(-offset.x, 0, 0)).a;
        float v4 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, -offset.y, 0)).a;
        float v5 = textureLookup3DUnnormalized(volumeStruct, samplePos + vec3(0, 0, -offset.z)).a;
        gradient = vec3(v3 - v0, v4 - v1, v5 - v2);
        gradient *= (volumeStruct.datasetSpacingRCP_ * 0.5);
        gradient *= volumeStruct.bitDepthScale_;
    }
    return gradient;
}

/**
 * Calculates a voxel's gradient in volume object space by calculating the
 * gradients for all eight neighbours based on the alpha channel using
 * central differences and filtering between the reuslts.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 */
vec3 calcGradientFiltered(VOLUME_STRUCT volumeStruct, vec3 samplePos, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    vec3 delta = volumeStruct.datasetDimensionsRCP_;
    vec3 g0 = calcGradientA(volumeStruct, samplePos, 0.5, vec3(0.0), entryPoints, entryParameters);
    vec3 g1 = calcGradientA(volumeStruct, samplePos+vec3(-delta.x, -delta.y, -delta.z), 0.5, vec3(0.0), entryPoints, entryParameters);
    vec3 g2 = calcGradientA(volumeStruct, samplePos+vec3( delta.x,  delta.y,  delta.z), 0.5, vec3(0.0), entryPoints, entryParameters);
    vec3 g3 = calcGradientA(volumeStruct, samplePos+vec3(-delta.x,  delta.y, -delta.z), 0.5, vec3(0.0), entryPoints, entryParameters);
    vec3 g4 = calcGradientA(volumeStruct, samplePos+vec3( delta.x, -delta.y,  delta.z), 0.5, vec3(0.0), entryPoints, entryParameters);
    vec3 g5 = calcGradientA(volumeStruct, samplePos+vec3(-delta.x, -delta.y,  delta.z), 0.5, vec3(0.0), entryPoints, entryParameters);
    vec3 g6 = calcGradientA(volumeStruct, samplePos+vec3( delta.x,  delta.y, -delta.z), 0.5, vec3(0.0), entryPoints, entryParameters);
    vec3 g7 = calcGradientA(volumeStruct, samplePos+vec3(-delta.x,  delta.y,  delta.z), 0.5, vec3(0.0), entryPoints, entryParameters);
    vec3 g8 = calcGradientA(volumeStruct, samplePos+vec3( delta.x, -delta.y, -delta.z), 0.5, vec3(0.0), entryPoints, entryParameters);
    vec3 mix0 = mix(mix(g1, g2, 0.5), mix(g3, g4, 0.5), 0.5);
    vec3 mix1 = mix(mix(g5, g6, 0.5), mix(g7, g8, 0.5), 0.5);
    return mix(g0, mix(mix0, mix1, 0.5), 0.75);
}

/**
 * Calculates a voxel's gradient in volume object space based on the alpha
 * channel using central differences.
 *
 * @param volume the voxel's volume
 * @param volumeStruct additional information about the passed volume
 * @param samplePos the sample's position in texture space
 */
vec3 calcGradient(VOLUME_STRUCT volumeStruct, vec3 samplePos, SAMPLER2D_TYPE entryPoints, TEXTURE_PARAMETERS entryParameters) {
    return calcGradientA(volumeStruct, samplePos, 0.5, vec3(0.0), entryPoints, entryParameters);
}

