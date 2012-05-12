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

#include "modules/mod_sampler2d.frag"

uniform SAMPLER2D_TYPE colorTex0_;
uniform SAMPLER2D_TYPE depthTex0_;
uniform TEXTURE_PARAMETERS textureParameters0_;
uniform SAMPLER2D_TYPE colorTex1_;
uniform SAMPLER2D_TYPE depthTex1_;
uniform TEXTURE_PARAMETERS textureParameters1_;

void main() {
    vec2 fragCoord = gl_FragCoord.xy;

    // fetch input textures
    vec4 color0 = textureLookup2Dscreen(colorTex0_, textureParameters0_, fragCoord);
    float depth0 = textureLookup2Dscreen(depthTex0_, textureParameters0_, fragCoord).z;
    vec4 color1 = textureLookup2Dscreen(colorTex1_, textureParameters1_, fragCoord);
    float depth1 = textureLookup2Dscreen(depthTex1_, textureParameters1_, fragCoord).z;

    vec4 fragColor = vec4(0.0);

    fragColor.rgb = color0.rgb + (1.0 - color0.a) * color1.rgb;
    fragColor.a = color0.a + (1.0 -color0.a) * color1.a;

    float fragDepth = min(depth0, depth1);

    FragData0 = fragColor;
    gl_FragDepth = fragDepth;
}
