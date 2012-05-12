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

#ifndef VRN_IMAGEMASKING_H
#define VRN_IMAGEMASKING_H

#include "voreen/core/processors/imageprocessorbypassable.h"
#include "voreen/core/properties/vectorproperty.h"

namespace voreen {

/**
 * Masks the input by a mask color.
 */
class ImageMasking : public ImageProcessorBypassable {
public:
    ImageMasking();
    virtual Processor* create() const;

    virtual std::string getClassName() const  { return "ImageMasking"; }
    virtual std::string getCategory() const   { return "Image Processing"; }
    virtual CodeState getCodeState() const    { return CODE_STATE_STABLE; }

protected:
    void process();

    RenderPort inport_;
    RenderPort inportMask_;
    RenderPort outport_;

    FloatVec4Property maskColor_;
};

} // namespace voreen

#endif //VRN_IMAGEMASKING_H
