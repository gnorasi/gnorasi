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

#include "modules/vrn_shaderincludes.frag"

// declare volume
uniform VOLUME_STRUCT volumeStruct_;    // volume data set

uniform SAMPLER2D_TYPE lightBuf_;
uniform TEXTURE_PARAMETERS lightBufParameters_;
uniform mat4 lightMat_;
uniform float dPlaneIncr_;
uniform bool secondPass_;

vec4 renderSlice() {
    vec3 texCoord = gl_TexCoord[0].xyz;
    vec3 worldCoord = 2.0*texCoord - 1.0;

    // texture coordinate adaption for non-uniform volumes
    texCoord -= vec3(0.5);
    texCoord *= volumeStruct_.volumeCubeSizeRCP_*2.0;
    texCoord += volumeStruct_.volumeTextureTranslation_;

    if (inUnitCube(texCoord)) {
        float intensity = getVoxel(volumeStruct_, texCoord).a;
        vec4 color = applyTF(transferFunc_, intensity);
        color.a = 1.0 - pow(1.0 - color.a, dPlaneIncr_ * SAMPLING_BASE_INTERVAL_SLICE_RCP);
        color.rgb *= color.a;

        if(!secondPass_) {
            // calculate the pixel location of this voxel on the previously rendered light texture
            vec4 lightPoint = (gl_ProjectionMatrix * lightMat_ * volumeStruct_.volumeTransformation_ * vec4(worldCoord, 1.0));
            lightPoint.xyz /= lightPoint.w;
            vec2 p = 0.5*lightPoint.xy + 0.5;
            vec4 lightcol = textureLookup2Dnormalized(lightBuf_, lightBufParameters_, p);
            color.rgb *= lightcol.rgb;
        } else
            color.rgb = vec3(0.0);

        return color;
    } else
        return vec4(0.0);
}

/***
 * The main method.
 ***/
void main() {

    vec4 currentRes = renderSlice();
    FragData0 = currentRes;
}
