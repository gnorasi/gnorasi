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

#ifndef VRN_IMAGESEQUENCELOOPINITIATOR_H
#define VRN_IMAGESEQUENCELOOPINITIATOR_H

#include "voreen/core/processors/renderprocessor.h"

namespace voreen {

/**
 * Defines an image-processing loop in combination with ImageSequenceLoopFinalizer.
 * All images of the input sequence are processed by the loop.
 */
class ImageSequenceLoopInitiator : public RenderProcessor {
public:
    ImageSequenceLoopInitiator();
    virtual Processor* create() const;

    virtual std::string getCategory() const  { return "Utility"; }
    virtual std::string getClassName() const { return "ImageSequenceLoopInitiator"; }
    virtual CodeState getCodeState() const   { return Processor::CODE_STATE_STABLE; }
    virtual bool isUtility() const           { return true; }

    virtual bool isReady() const;

protected:
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual void invalidate(int inv = INVALID_RESULT);

    /// Shader program used for rendering the current image to the outport.
    tgt::Shader* shader_;

    ImageSequencePort inport_;
    RenderPort outport_;
    Port loopInport_;

    static const std::string loggerCat_; ///< category used in logging
};

}

#endif //VRN_IMAGESEQUENCELOOPINITIATOR_H
