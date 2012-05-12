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

#include "voreen/core/datastructures/volume/volumetexture.h"

#include "tgt/plane.h"

using tgt::vec3;
using tgt::ivec3;
using tgt::mat4;
using tgt::plane;

namespace voreen {

VolumeTexture::VolumeTexture(const GLubyte* data, const tgt::ivec3& dimensions,
                             GLint format,
                             GLint internalformat,
                             GLenum dataType,
                             tgt::Texture::Filter filter)
  : tgt::Texture(const_cast<GLubyte*>(data), dimensions, format, internalformat, dataType, filter)
{
    tgtAssert(tgt::hand(tgt::greaterThan(dimensions, ivec3(1))),
        "Invalid volume dimensions: Must be greater one in all directions");

    // Always set to a 3d texture
    setType(GL_TEXTURE_3D);
}

} // namespace voreen
