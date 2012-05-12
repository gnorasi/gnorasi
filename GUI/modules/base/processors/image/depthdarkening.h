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

#ifndef VRN_DEPTHDARKENING_H
#define VRN_DEPTHDARKENING_H

#include "voreen/core/processors/imageprocessorbypassable.h"
#include "voreen/core/properties/floatproperty.h"

namespace voreen {

/**
 * Performs depth darkening as
 * presented by Luft et al. in 2006.
 */
class DepthDarkening : public ImageProcessorBypassable {
public:
    DepthDarkening();
    virtual std::string getCategory() const { return "Image Processing"; }
    virtual std::string getClassName() const { return "DepthDarkening"; }
    virtual Processor::CodeState getCodeState() const { return CODE_STATE_STABLE; }
    virtual Processor* create() const;
    void process();

protected:
    FloatProperty sigma_; ///< Standard deviation of the used Gaussian filter
    FloatProperty lambda_; ///< Controls the strength of the depth masking effect

    RenderPort inport_;
    RenderPort outport_;
    RenderPort privatePort_;
};


} // namespace voreen

#endif //VRN_DEPTHDARKENING_H
