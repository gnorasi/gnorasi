/***********************************************************************************
 *                                                                                 *
 * Voreen - The Volume Rendering Engine                                            *
 *                                                                                 *
 * Copyright (C) 2005-2012 University of Muenster, Germany.                        *
 * Visualization and Computer Graphics Group <http://viscg.uni-muenster.de>        *
 * For a list of authors please refer to the file "CREDITS.txt".                   *
 *                                                                                 *
 * This file is part of the Voreen software package. Voreen is free software:      *
 * you can redistribute it and/or modify it under the terms of the GNU General     *
 * Public License version 2 as published by the Free Software Foundation.          *
 *                                                                                 *
 * Voreen is distributed in the hope that it will be useful, but WITHOUT ANY       *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR   *
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.      *
 *                                                                                 *
 * You should have received a copy of the GNU General Public License in the file   *
 * "LICENSE.txt" along with this file. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                                 *
 * For non-commercial academic use see the license exception specified in the file *
 * "LICENSE-academic.txt". To get information about commercial licensing please    *
 * contact the authors.                                                            *
 *                                                                                 *
 ***********************************************************************************/

#include "edgedetect.h"

#include "tgt/textureunit.h"

using tgt::TextureUnit;

namespace voreen {

EdgeDetect::EdgeDetect()
    : ImageProcessorBypassable("image/edgedetect"),
      edgeThreshold_("edgeThreshold", "Edge threshold", 0.04f, 0.001f, 1.f),
      backgroundColor_("backgroundColor", "Background color", tgt::Color(0.0f, 0.0f, 0.0f, 0.0f)),
      showImage_("showImage", "Show image", true),
      blendMode_("blendMode", "Blend mode", Processor::INVALID_PROGRAM),
      edgeStyle_("edgeStyle", "Edge style", Processor::INVALID_PROGRAM),
      edgeColor_("edgeColor", "Edge color", tgt::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
      inport_(Port::INPORT, "image.inport"),
      outport_(Port::OUTPORT, "image.outport")
{
    edgeColor_.setViews(Property::COLOR);
    backgroundColor_.setViews(Property::COLOR);
    backgroundColor_.set(tgt::vec4(0.0f,0.0f,0.0f,1.0f));

    edgeThreshold_.setNumDecimals(3);
    blendMode_.addOption("replace", "Replace", 0);
    blendMode_.addOption("pseudo-chromadepth", "Pseudo chromadepth", 1);
    blendMode_.addOption("blend", "Blend", 2);
    blendMode_.addOption("gradient output", "Gradient output", 3);
    edgeStyle_.addOption("contour", "Contour", 0);
    edgeStyle_.addOption("silhouette", "Silhouette", 1);
    edgeStyle_.addOption("contour-depth-based", "Contour (depth-based thickness)", 2);
    addProperty(edgeStyle_);
    addProperty(edgeThreshold_);
    addProperty(blendMode_);
    addProperty(edgeColor_);
    addProperty(backgroundColor_);
    addProperty(showImage_);

    addPort(inport_);
    addPort(outport_);
}

EdgeDetect::~EdgeDetect() {}

Processor* EdgeDetect::create() const {
    return new EdgeDetect();
}

void EdgeDetect::process() {

    if (!enableSwitch_.get()) {
        bypass(&inport_, &outport_);
        return;
    }

    outport_.activateTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //compute Depth Range
    tgt::vec2 depthRange = computeDepthRange(&inport_);

    TextureUnit colorUnit, depthUnit;
    inport_.bindTextures(colorUnit.getEnum(), depthUnit.getEnum());
    LGL_ERROR;

    // initialize shader
    program_->activate();
    setGlobalShaderParameters(program_);
    inport_.setTextureParameters(program_, "texParams_");
    program_->setUniform("colorTex_", colorUnit.getUnitNumber());
    program_->setUniform("depthTex_", depthUnit.getUnitNumber());
    program_->setUniform("minDepth_", depthRange.x);
    program_->setUniform("maxDepth_", depthRange.y);
    program_->setUniform("edgeColor_", edgeColor_.get());
    program_->setUniform("backgroundColor_", backgroundColor_.get());
    program_->setUniform("edgeThreshold_", edgeThreshold_.get());
    program_->setUniform("showImage_", showImage_.get());
    program_->setUniform("blendMode_", blendMode_.getValue());
    program_->setUniform("edgeStyle_", edgeStyle_.getValue());

    renderQuad();

    program_->deactivate();
    outport_.deactivateTarget();
    LGL_ERROR;
}

} // voreen namespace
