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

/*******************/
/* Declare Inports */
/*******************/
//$ @name = "volumehandle", @type = "volumeport"
uniform VOLUME_STRUCT volume_;

//$ @name = "inport", @depthtex = "inputTexDepth_", @params = "inputTexParams_"
uniform sampler2D inputTex_;
uniform sampler2D inputTexDepth_;
uniform TEXTURE_PARAMETERS inputTexParams_;


/********************/
/* Declare Outports */
/********************/

//$ @name = "outport0"
out vec4 FragData;

//$ @name = "outport1"
out vec4 FragData1;


/**********************/
/* Declare Properties */
/**********************/
//$ @name = "test", @label = "Test property"
uniform float test_;

/***
 * The main method.
 ***/
void main() {
    vec2 p = gl_FragCoord.xy * screenDimRCP_;
    FragData = vec4(0.0, test_, 0.0, 1.0);
    FragData1 = vec4(1.0, 1.0, 0.0, 1.0)*texture2D(inputTex_, p);
    gl_FragDepth = texture2D(inputTexDepth_, p).z;
}
