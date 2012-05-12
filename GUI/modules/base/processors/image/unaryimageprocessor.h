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

#ifndef VRN_UNARYIMAGEPROCESSOR_H
#define VRN_UNARYIMAGEPROCESSOR_H

#include "voreen/core/processors/imageprocessorbypassable.h"
#include "voreen/core/properties/shaderproperty.h"

namespace voreen {

class UnaryImageProcessor : public ImageProcessorBypassable {
public:
    UnaryImageProcessor();
    virtual Processor* create() const;

    virtual std::string getCategory() const { return "Image Processing"; }
    virtual std::string getClassName() const { return "UnaryImageProcessor"; }
    virtual CodeState getCodeState() const { return CODE_STATE_STABLE; }

protected:
    void process();
    virtual void initialize() throw (tgt::Exception);

    virtual void loadShader();
    virtual void compileShader();

    ShaderProperty shader_;  ///< the property that controls the used shader

    RenderPort inport_;
    RenderPort outport_;
};


} // namespace voreen

#endif //VRN_UNARYIMAGEPROCESSOR_H
