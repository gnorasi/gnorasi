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

#ifndef VRN_IMAGEPROCESSORBYPASSABLE_H
#define VRN_IMAGEPROCESSORBYPASSABLE_H

#include "voreen/core/processors/imageprocessor.h"

#include "voreen/core/properties/boolproperty.h"

namespace voreen {

/**
 * Base class for post processors that have a boolProperty (enableSwitch_) to toggle
 * between enable and bypass.
 * In render method, this switch can be queried and bypass method can be used to
 * pass the image from inport to outport without changes.
 * enableSwitch_ has to be added in constructor to be visible and bypass-method has to
 * be explicitly called.
 */
class ImageProcessorBypassable : public ImageProcessor {
public:
    /**
     * Constructor.
     *
     * @param shaderFilename The filename of the shader that will be used.
     */
    ImageProcessorBypassable(const std::string& shaderFilename = "");

    virtual std::string getCategory() const { return "Image Processing"; }
    virtual std::string getClassName() const { return "ImageProcessorBypassable"; }

protected:
    virtual void bypass(RenderPort *inport, RenderPort *outport); ///< Passes the image from inport to outport without changes.
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    BoolProperty enableSwitch_; ///< Should be used to control if bypass or actual image processing is applied

private:
    tgt::Shader* bypassProgram_; ///< shader program that is used for bypass
};

} // namespace voreen

#endif //VRN_IMAGEPROCESSORBYPASSABLE_H
