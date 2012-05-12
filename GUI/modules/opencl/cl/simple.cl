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

__constant sampler_t smpNorm = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

// Simple OpenCL kernel.  Extend it's arguments to dynamically generate ports and properties.
__kernel void standardKernel(__global read_only image2d_t inCol, //$ color inport
                             __global write_only image2d_t outCol, //$ color outport
	                         float test //$ float property to test
                            )
{
    //output image pixel coordinates
    int2 target = (int2)(get_global_id(0), get_global_id(1));
    float2 targetNorm = convert_float2(target) / convert_float2((int2)(get_global_size(0), get_global_size(1)));

    float4 color = read_imagef(inCol, smpNorm, targetNorm);
    write_imagef(outCol, target, color * test);
}


