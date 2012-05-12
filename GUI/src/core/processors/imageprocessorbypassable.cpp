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

#include "voreen/core/processors/imageprocessorbypassable.h"

#include "tgt/textureunit.h"

using tgt::TextureUnit;

namespace voreen {

ImageProcessorBypassable::ImageProcessorBypassable(const std::string& shaderFilename)
    : ImageProcessor(shaderFilename),
      enableSwitch_("enable", "Enable", true)
{}


void ImageProcessorBypassable::initialize() throw (tgt::Exception) {
    ImageProcessor::initialize(); //call parent's initialize method, initializes first shader program

    program_->deactivate();

    //initialize bypass shader program
    bypassProgram_ = ShdrMgr.loadSeparate("passthrough.vert", "copyimage.frag", generateHeader(), false);
    invalidate(Processor::INVALID_PROGRAM);
    if (getInvalidationLevel() >= Processor::INVALID_PROGRAM)
        bypassProgram_->rebuild();
    bypassProgram_->deactivate();
}

void ImageProcessorBypassable::deinitialize() throw (tgt::Exception) {
    ShdrMgr.dispose(bypassProgram_);
    bypassProgram_ = 0;
    LGL_ERROR;

    ImageProcessor::deinitialize();
}

/*
* Pass the image from inport to outport without changes
*/
void ImageProcessorBypassable::bypass(RenderPort* inport, RenderPort* outport) {
    // activate and clear output render target
    outport->activateTarget();
    outport->clearTarget();

    // bind input rendering to texture units
    TextureUnit colorUnit, depthUnit;
    inport->bindTextures(colorUnit.getEnum(), depthUnit.getEnum());

    // activate shader and set uniforms
    bypassProgram_->activate();
    setGlobalShaderParameters(bypassProgram_);
    inport->setTextureParameters(bypassProgram_, "texParams_");
    bypassProgram_->setUniform("colorTex_", colorUnit.getUnitNumber());
    bypassProgram_->setUniform("depthTex_", depthUnit.getUnitNumber());

    // render screen aligned quad
    renderQuad();

    // cleanup
    bypassProgram_->deactivate();
    outport->deactivateTarget();
    TextureUnit::setZeroUnit();

    // check for OpenGL errors
    LGL_ERROR;
}

} // voreen namespace
