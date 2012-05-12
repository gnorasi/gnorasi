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

uniform SAMPLER2D_TYPE originalColorTex_;
uniform SAMPLER2D_TYPE blurredColorTex_;
uniform SAMPLER2D_TYPE depthTex_;
uniform TEXTURE_PARAMETERS textureParameters_;
uniform float amount_;
uniform float threshold_;

void main() {
    vec2 fragCoord = gl_FragCoord.xy;

    vec4 original = textureLookup2Dscreen(originalColorTex_, textureParameters_, fragCoord);
    vec4 blurred = textureLookup2Dscreen(blurredColorTex_, textureParameters_, fragCoord);

    // do the masking: v'=a*v+(1-a)*bv, where a = amount, v = original image, bv = blurred image
    vec4 curColor = amount_*original + (1-amount_)*blurred;
    
    FragData0 = (length(original) - length(blurred) < threshold_) ?  original : curColor;

    // retain depth value from original image
    float depth;
    depth = textureLookup2Dscreen(depthTex_, textureParameters_, fragCoord).z;
    gl_FragDepth = depth;
 }
