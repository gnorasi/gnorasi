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

#include "voreen/core/processors/imageprocessor.h"

namespace voreen {

const std::string ImageProcessor::loggerCat_("voreen.ImageProcessor");

ImageProcessor::ImageProcessor(const std::string& shaderFilename)
    : RenderProcessor()
    , program_(0)
    , shaderFilename_(shaderFilename)
{}

ImageProcessor::~ImageProcessor() {
}

void ImageProcessor::initialize() throw (tgt::Exception) {
    RenderProcessor::initialize();

    if (!shaderFilename_.empty()) {
        program_ = ShdrMgr.loadSeparate("passthrough.vert", shaderFilename_ + ".frag", generateHeader(), false);
        invalidate(Processor::INVALID_PROGRAM);
        if (getInvalidationLevel() >= Processor::INVALID_PROGRAM)
            compile();
        program_->deactivate();
    }
    else
        program_ = 0;
}

void ImageProcessor::deinitialize() throw (tgt::Exception) {
    ShdrMgr.dispose(program_);
    program_ = 0;
    LGL_ERROR;

    RenderProcessor::deinitialize();
}

void ImageProcessor::compile() {
    if (program_)
        program_->rebuild();
}

/**
* Read back depth buffer and determine min and max depth value.
*
* @param port the port to analyze the depth buffer
* @return tgt::vec2 with x = minDepth, y = maxDepth
*/
tgt::vec2 ImageProcessor::computeDepthRange(RenderPort* port) {
    port->getDepthTexture()->downloadTexture();
    float* pixels = (float*)port->getDepthTexture()->getPixelData();
    float curDepth = *(pixels);
    float minDepth = curDepth;
    float maxDepth = curDepth;
    int numPixels = tgt::hmul(port->getSize());
    for (int i = 0; i < numPixels; i++) {
        curDepth = *(pixels++);
        minDepth = std::min(minDepth, curDepth);
        maxDepth = std::max(maxDepth, curDepth);
    }
    return tgt::vec2(minDepth, maxDepth);
}

} // voreen namespace
