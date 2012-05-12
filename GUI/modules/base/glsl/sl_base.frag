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
#include "modules/mod_sampler3d.frag"
#include "modules/mod_transfunc.frag"

#ifdef SLICE_TEXTURE_MODE_2D
    uniform SAMPLER2D_TYPE sliceTex_;               // slice texture
    uniform TEXTURE_PARAMETERS sliceTexParameters_; // slice texture parameters
    uniform float sliceBitScale;    //< used for 12bit volumes
#else defined(SLICE_TEXTURE_MODE_3D)
    uniform VOLUME_STRUCT volumeStruct_;    // volume data set
#else
-- should not get here --    
#endif

uniform int numChannels_;                       // number of channels in volume
uniform bool applyColorModulation_;             // 

vec3 rgb2hsv(vec3 colorRGB) {
    float minComponent = min(colorRGB.r, min(colorRGB.g, colorRGB.b));
    float maxComponent = max(colorRGB.r, max(colorRGB.g, colorRGB.b));
    float delta = maxComponent - minComponent;

    vec3 result;
    result.b = maxComponent;
    if (maxComponent != 0.0) result.g = delta / maxComponent;
    else result.g = 0.0;
    if (result.g == 0.0) result.r = 0.0; // no hue
    else {
        if (colorRGB.r == maxComponent) result.r = (colorRGB.g - colorRGB.b) / delta;
        else if (colorRGB.g == maxComponent) result.r = 2 + (colorRGB.b - colorRGB.r) / delta;
        else if (colorRGB.b == maxComponent) result.r = 4 + (colorRGB.r - colorRGB.g) / delta;
        result.r *= 60.0;
        if (result.r < 0.0) result.r += 360.0;
        result.r /= 360.0;
    }
    return result;
}

void main() {
    // fetch intensity
#ifdef SLICE_TEXTURE_MODE_2D
    vec4 intensity = textureLookup2Dnormalized(sliceTex_, sliceTexParameters_, gl_TexCoord[0].xy) * sliceBitScale;
#else if defined(SLICE_TEXTURE_MODE_3D)
    vec4 intensity = getVoxel(volumeStruct_, gl_TexCoord[0].xyz);
#else
    -- should not get here --    
#endif


    vec4 mat;

    if (numChannels_ == 1) {
        // Assuming Intensity only volume - get value via the transfer function.
        mat = applyTF(transferFunc_, intensity.a);
    }
    else if (numChannels_ == 3) {
        // Assuming RGB volume - convert RGB to HSV and apply transfer function to hue.
        vec4 tfColor = applyTF(transferFunc_, rgb2hsv(intensity.rgb).r);
        if (applyColorModulation_)
            intensity.rgb *= tfColor.rgb;
        mat = vec4(intensity.rgb, tfColor.a);
    }
    else if (numChannels_ == 4) {
        // Assuming RGBA volume - no transfer function lookup necessary
        mat = intensity;
    }
    else {
        // something bad happened
        mat = vec4(1.0);
    }

    vec4 fragColor = mat;
    FragData0 = fragColor;

    if (mat.a > 0.0)
        gl_FragDepth = gl_FragCoord.z;
    else
        gl_FragDepth = 1.0;
}

