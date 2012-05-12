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

#ifndef VRN_VOLUMETEXTURE_H
#define VRN_VOLUMETEXTURE_H

#include "tgt/matrix.h"
#include "tgt/texturemanager.h"

namespace voreen {

/**
 * This class is basically a tgt::Texture.
  */
class VolumeTexture : public tgt::Texture {
public:
    /**
     * With data and internalformat argument, type_ is set to GL_TEXTURE_3D
     * and \e NO new chunk of data will be allocated on the heap.
     */
    VolumeTexture(const GLubyte* data, const tgt::ivec3& dimensions,
                  GLint format,
                  GLint internalformat,
                  GLenum dataType,
                  tgt::Texture::Filter filter);
};

} // namespace voreen

#endif // VRN_VOLUMETEXTURE_H
