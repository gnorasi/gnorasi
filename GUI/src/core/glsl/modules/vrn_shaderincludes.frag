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

// Enable extension GL_ARB_draw_buffers only if it is not yet directly supported
// by this GLSL version and the extension was detected by GLEW.
#if __VERSION__ < 120 && defined(VRN_GLEW_ARB_draw_buffers)
#extension GL_ARB_draw_buffers : enable
#endif

// include required shader modules
#include "modules/mod_sampler2d.frag"
#include "modules/mod_sampler3d.frag"

#include "modules/mod_curvature.frag"
#include "modules/mod_raysetup.frag"
#include "modules/mod_transfunc.frag"
#include "modules/mod_gradients.frag"
#include "modules/mod_shading.frag"
#include "modules/mod_compositing.frag"
#include "modules/mod_depth.frag"
