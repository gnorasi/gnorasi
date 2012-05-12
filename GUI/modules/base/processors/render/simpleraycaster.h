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

#ifndef VRN_SIMPLERAYCASTER_H
#define VRN_SIMPLERAYCASTER_H

#include "voreen/core/processors/volumeraycaster.h"

#include "voreen/core/properties/cameraproperty.h"
#include "voreen/core/properties/transfuncproperty.h"

namespace voreen {

/**
 * Performs a simple single pass raycasting without lighting.
 */
class SimpleRaycaster : public VolumeRaycaster {
public:
    SimpleRaycaster();
    virtual Processor* create() const;

    virtual std::string getCategory() const   { return "Raycasting";      }
    virtual std::string getClassName() const  { return "SimpleRaycaster"; }
    virtual CodeState getCodeState() const    { return CODE_STATE_STABLE; }

protected:
    virtual void beforeProcess();
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);

    virtual std::string generateHeader();

private:
    VolumePort volumePort_;
    RenderPort entryPort_;
    RenderPort exitPort_;
    RenderPort outport_;

    tgt::Shader* raycastPrg_;         ///< The shader program used by this raycaster.

    TransFuncProperty transferFunc_;  ///< the property that controls the transfer-function
    CameraProperty camera_;           ///< necessary for depth value calculation
};


} // namespace

#endif // VRN_SIMPLERAYCASTER_H
