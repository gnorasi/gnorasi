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

#ifndef VRN_CURVATURERAYCASTER_H
#define VRN_CURVATURERAYCASTER_H

#include "voreen/core/processors/volumeraycaster.h"

#include "voreen/core/properties/transfuncproperty.h"
#include "voreen/core/properties/cameraproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/floatproperty.h"

#include "voreen/core/ports/volumeport.h"

namespace voreen {

class CurvatureRaycaster : public VolumeRaycaster {
public:
    CurvatureRaycaster();
    virtual Processor* create() const;

    virtual std::string getClassName() const  { return "CurvatureRaycaster"; }
    virtual std::string getCategory() const   { return "Raycasting"; }
    virtual CodeState getCodeState() const    { return CODE_STATE_EXPERIMENTAL; }

    /**
     * All inports and at least one outport need to be connected.
     */
    virtual bool isReady() const;

protected:
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual std::string generateHeader();
    virtual void compile();

private:
    VolumePort volumeInport_;
    VolumePort gradientInport_;
    RenderPort entryPort_;
    RenderPort exitPort_;

    RenderPort outport_;
    RenderPort outport1_;
    RenderPort outport2_;
    PortGroup portGroup_;

    tgt::Shader* raycastPrg_;          ///< The shader program used by this raycaster.

    TransFuncProperty transferFunc_;   ///< the property that controls the transfer-function
    CameraProperty camera_;            ///< the camera used for lighting calculations

    StringOptionProperty compositingMode1_;   ///< What compositing mode should be applied for second outport
    StringOptionProperty compositingMode2_;   ///< What compositing mode should be applied for third outport

    StringOptionProperty curvatureType_;
    FloatProperty curvatureFactor_;
    FloatProperty silhouetteWidth_;
    FloatProperty minGradientLength_;
};


} // namespace voreen

#endif // VRN_CURVATURERAYCASTER_H
