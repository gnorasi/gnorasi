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
uniform float dPlaneIncr_;

vec4 renderSlice() {
    vec3 texCoord = gl_TexCoord[0].xyz;
    
    // texture coordinate adaption for non-uniform volumes
    texCoord -= vec3(0.5);
    texCoord *= volumeStruct_.volumeCubeSizeRCP_*2.0;
    texCoord += volumeStruct_.volumeTextureTranslation_;

    if (inUnitCube(texCoord)) {
        float intensity = getVoxel(volumeStruct_, texCoord).a;
        vec4 color = applyTF(transferFunc_, intensity);
        color.a = 1.0 - pow(1.0 - color.a, dPlaneIncr_ * SAMPLING_BASE_INTERVAL_SLICE_RCP);
        color.rgb *= color.a;
        return color;
    } else
        return vec4(0.0);
}

/***
 * The main method.
 ***/
void main() {
    FragData0 = renderSlice();
}
