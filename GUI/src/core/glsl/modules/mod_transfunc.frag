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
 * This module contains all functions which can be used for performing
 * classifications of a voxel within a raycaster.
 * The functions below are referenced by RC_APPLY_CLASSIFICATION which
 * is used in the raycaster fragment shaders.
 */
 
#ifndef DEPRECATED_TRANSFUNC_ONLY

struct TransFunc1D {
    sampler1D texture_;

    float domainLower_;
    float domainUpper_;
};

struct TransFunc2D {
    sampler2D texture_;

    vec2 domainLower_;
    vec2 domainUpper_;
};

float realWorldToTexture(TransFunc1D tf, float v) {
    if(v <= tf.domainLower_)
       return 0.0;
    else if(v >= tf.domainUpper_)
       return 1.0; 
    else
        return (v - tf.domainLower_) / (tf.domainUpper_ - tf.domainLower_);
}

vec4 applyTF(TransFunc1D transfunc, float intensity) {
    intensity = realWorldToTexture(transfunc, intensity);
    #if defined(GLSL_VERSION_130)
        return texture(transfunc.texture_, intensity);
    #else
        return texture1D(transfunc.texture_, intensity);
    #endif
}

vec4 applyTF(TransFunc1D transfunc, vec4 intensity) {
    intensity.a = realWorldToTexture(transfunc, intensity.a);
    #if defined(GLSL_VERSION_130)
        return texture(transfunc.texture_, intensity.a);
    #else
        return texture1D(transfunc.texture_, intensity.a);
    #endif
}

vec4 applyTF(TransFunc2D transfunc, float intensity, float gradientMagnitude) {
    #if defined(GLSL_VERSION_130)
        return texture(transfunc.texture_, vec2(intensity, gradientMagnitude));
    #else
        return texture2D(transfunc.texture_, vec2(intensity, gradientMagnitude));
    #endif
}

vec4 applyTF(TransFunc2D transfunc, vec4 intensityGradient) {
    #if defined(GLSL_VERSION_130)
        return texture(transfunc.texture_, vec2(intensityGradient.a, length(intensityGradient.rgb)));
    #else
        return texture2D(transfunc.texture_, vec2(intensityGradient.a, length(intensityGradient.rgb)));
    #endif
}

#endif

// Deprecated:

#if defined(TF_SAMPLER_TYPE)
    uniform TF_SAMPLER_TYPE transferFunc_;
#endif

vec4 applyTF(sampler1D transfunc, float intensity) {
    #if defined(GLSL_VERSION_130)
        return texture(transfunc, intensity);
    #else
        return texture1D(transfunc, intensity);
    #endif
}

vec4 applyTF(sampler1D transfunc, vec4 intensity) {
    #if defined(GLSL_VERSION_130)
        return texture(transfunc, intensity.a);
    #else
        return texture1D(transfunc, intensity.a);
    #endif
}

vec4 applyTF(sampler2D transfunc, float intensity, float gradientMagnitude) {
    #if defined(GLSL_VERSION_130)
        return texture(transfunc, vec2(intensity, gradientMagnitude));
    #else
        return texture2D(transfunc, vec2(intensity, gradientMagnitude));
    #endif
}

vec4 applyTF(sampler2D transfunc, vec4 intensityGradient) {
    #if defined(GLSL_VERSION_130)
        return texture(transfunc, vec2(intensityGradient.a, length(intensityGradient.rgb)));
    #else
        return texture2D(transfunc, vec2(intensityGradient.a, length(intensityGradient.rgb)));
    #endif
}


