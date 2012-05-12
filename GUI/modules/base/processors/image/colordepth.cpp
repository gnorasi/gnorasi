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

#include "colordepth.h"

#include "tgt/texturemanager.h"
#include "tgt/textureunit.h"

using tgt::TextureUnit;

namespace voreen {

ColorDepth::ColorDepth()
    : ImageProcessor("image/colordepth")
    , chromaDepthTex_(0)
    , colorMode_("mode", "Choose mode", INVALID_PROGRAM)
    , factor_("factor", "Factor", 1.0f, 0.0f, 10.0f)
    , inport_(Port::INPORT, "image.inport")
    , outport_(Port::OUTPORT, "image.outport")
{
    colorMode_.addOption("light-dark-replace", "Light-dark (replace)", 0);
    colorMode_.addOption("light-dark-modulate", "Light-dark (modulate)", 1);
    colorMode_.addOption("chromadepth", "Chromadepth", 2);
    colorMode_.addOption("pseudo-chromadepth", "Pseudo chromadepth", 3);
    addProperty(colorMode_);

    addProperty(factor_);

    addPort(inport_);
    addPort(outport_);
}

ColorDepth::~ColorDepth() {
    if (chromaDepthTex_)
        TexMgr.dispose(chromaDepthTex_);
}

void ColorDepth::initialize() throw (tgt::Exception) {
    chromaDepthTex_ = TexMgr.load(VoreenApplication::app()->getTransFuncPath("chromadepthspectrum.bmp"));
    ImageProcessor::initialize();
}

void ColorDepth::process() {
    if (chromaDepthTex_ == 0)
        return;

    //compute Depth Range
    tgt::vec2 depthRange = computeDepthRange(&inport_);

    outport_.activateTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    TextureUnit colorUnit, depthUnit;
    inport_.bindTextures(colorUnit.getEnum(), depthUnit.getEnum());

    // bind chroma depth texture
    TextureUnit chromaDepthUnit;
    chromaDepthUnit.activate();
    //chromaDepthTex_ is 0 here
    chromaDepthTex_->bind();
    LGL_ERROR;

    // initialize shader
    program_->activate();
    setGlobalShaderParameters(program_);
    program_->setUniform("colorTex_", colorUnit.getUnitNumber());
    program_->setUniform("depthTex_", depthUnit.getUnitNumber());
    inport_.setTextureParameters(program_, "texParams_");
    program_->setUniform("chromadepthTex_", chromaDepthUnit.getUnitNumber());
    program_->setUniform("minDepth_", depthRange.x);
    program_->setUniform("maxDepth_", depthRange.y);
    program_->setUniform("colorMode_", colorMode_.getValue());
    program_->setUniform("colorDepthFactor_", factor_.get());

    renderQuad();

    program_->deactivate();
    TextureUnit::setZeroUnit();
    outport_.deactivateTarget();
    LGL_ERROR;
}

} // voreen namespace
