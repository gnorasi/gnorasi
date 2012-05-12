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

#ifndef VRN_EEPGEOMETRYINTEGRATOR_H
#define VRN_EEPGEOMETRYINTEGRATOR_H

#include "voreen/core/processors/imageprocessor.h"

#include "voreen/core/ports/volumeport.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/cameraproperty.h"

namespace voreen {

class EEPGeometryIntegrator : public ImageProcessor {

public:
    EEPGeometryIntegrator();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "EEPGeometryIntegrator"; }
    virtual std::string getCategory() const  { return "Image Processing";      }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; }
    virtual bool isReady() const;

protected:
    virtual void beforeProcess();
    virtual void process();

    RenderPort inport0_;
    RenderPort inport1_;
    RenderPort geometryPort_;
    VolumePort volumeInport_;

    RenderPort entryPort_;
    RenderPort exitPort_;
    RenderPort tmpPort_;

    BoolProperty useFloatRenderTargets_;
    CameraProperty camera_;

private:
    /// category used in logging
    static const std::string loggerCat_;
};


} // namespace voreen

#endif
