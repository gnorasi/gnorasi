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

#include "brickloopimagecompositor.h"
#include "tgt/textureunit.h"

namespace voreen {

using tgt::TextureUnit;

const std::string BrickLoopImageCompositor::loggerCat_("voreen.experimental.BrickLoopImageCompositor");

BrickLoopImageCompositor::BrickLoopImageCompositor()
    : ImageProcessor("image/brickcompositor"),
      inport_(Port::INPORT, "image.in"),
      outport_(Port::OUTPORT, "image.out"),
      loopOutport_("loop.out", Port::OUTPORT),
      privatePort_(Port::OUTPORT, "image.blend")
{
    loopOutport_.setLoopPort(true);

    addPort(inport_);
    addPort(outport_);
    addPort(loopOutport_);
    addPrivateRenderPort(&privatePort_);

    pingPongMod = 0;
}

BrickLoopImageCompositor::~BrickLoopImageCompositor() {
}

Processor* BrickLoopImageCompositor::create() const {
    return new BrickLoopImageCompositor();
}

bool BrickLoopImageCompositor::isReady() const {
    return (outport_.isReady());
}

void BrickLoopImageCompositor::process() {
    //if no valid data, do nothing
    if (!inport_.isReady()) {
        return;
    }
    else if(inport_.hasChanged()){
        if (getInvalidationLevel() >= Processor::INVALID_PROGRAM)
            compile();

        // first iteration: clear previous targets and create new one
        if (loopOutport_.getLoopIteration() == 0) {
            outport_.activateTarget();
            outport_.clearTarget();
            privatePort_.activateTarget();
            privatePort_.clearTarget();

            pingPongMod = (loopOutport_.getNumLoopIterations() % 2 ? 0 : 1);
        }

        // Ping pong
        TextureUnit colorUnit0, depthUnit0, colorUnit1, depthUnit1;
        if ((loopOutport_.getLoopIteration() % 2) == pingPongMod) {
            // outport <-- inport + private
            outport_.activateTarget();
            inport_.bindTextures(colorUnit0.getEnum(), depthUnit0.getEnum());
            privatePort_.bindTextures(colorUnit1.getEnum(), depthUnit1.getEnum());
        }
        else {
            // private <-- inport + outport
            privatePort_.activateTarget();
            inport_.bindTextures(colorUnit0.getEnum(), depthUnit0.getEnum());
            outport_.bindTextures(colorUnit1.getEnum(), depthUnit1.getEnum());
        }

        // initialize shader
        program_->activate();
        setGlobalShaderParameters(program_);

        program_->setUniform("colorTex0_", colorUnit0.getUnitNumber());
        program_->setUniform("depthTex0_", depthUnit0.getUnitNumber());
        outport_.setTextureParameters(program_, "textureParameters0_");

        program_->setUniform("colorTex1_", colorUnit1.getUnitNumber());
        program_->setUniform("depthTex1_", depthUnit1.getUnitNumber());
        outport_.setTextureParameters(program_, "textureParameters1_");

        renderQuad();

        program_->deactivate();
        outport_.deactivateTarget();
        privatePort_.deactivateTarget();
        TextureUnit::setZeroUnit();
    }

    LGL_ERROR;
}

void BrickLoopImageCompositor::compile() {
    if (program_)
        program_->setHeaders(generateHeader());
    ImageProcessor::compile();
}

void BrickLoopImageCompositor::initialize() throw (tgt::Exception) {
    ImageProcessor::initialize();
}

void BrickLoopImageCompositor::deinitialize() throw (tgt::Exception) {
    ImageProcessor::deinitialize();
}

} // voreen namespace
