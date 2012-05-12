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

#include "modules/base/processors/image/texturetotarget.h"

using tgt::TextureUnit;

namespace voreen {

TextureToTarget::TextureToTarget()
    : ImageProcessor("image/texturetotarget")
    , inport_(Port::INPORT, "inport")
    , outport_(Port::OUTPORT, "outport")
{
    // register ports and properties
    addPort(inport_);
    addPort(outport_);
}

void TextureToTarget::process() {
    // activate and clear output render target
    outport_.activateTarget();
    outport_.clearTarget();

    // bind input rendering to texture units
    TextureUnit colorUnit;
    const tgt::Texture* texture = inport_.getData();
    colorUnit.activate();
    texture->bind();

    // activate shader and set uniforms
    program_->activate();
    setGlobalShaderParameters(program_);
    //inport_.setTextureParameters(program_, "textureParameters_");
    program_->setUniform("colorTex_", colorUnit.getUnitNumber());

    // render screen aligned quad
    renderQuad();

    // cleanup
    program_->deactivate();
    outport_.deactivateTarget();
    TextureUnit::setZeroUnit();

    // check for OpenGL errors
    LGL_ERROR;
}

} // namespace
