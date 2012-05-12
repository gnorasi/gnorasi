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

#ifndef VRN_DISTANCEMEASURE_H
#define VRN_DISTANCEMEASURE_H

#include "voreen/core/processors/imageprocessor.h"
#include "voreen/core/properties/eventproperty.h"
#include "voreen/core/properties/cameraproperty.h"
#include "voreen/core/properties/stringproperty.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/utils/stringconversion.h"

#include "voreen/core/ports/volumeport.h"

#include "tgt/font.h"
#include "tgt/glmath.h"

namespace voreen {

class DistanceMeasure : public ImageProcessor {
public:
    DistanceMeasure();
    ~DistanceMeasure();
    virtual Processor* create() const;

    virtual std::string getCategory() const  { return "Utility"; }
    virtual std::string getClassName() const { return "DistanceMeasure"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_TESTING; }
    virtual bool isUtility() const           { return true; }

    virtual bool isReady();

    void measure(tgt::MouseEvent* e);

protected:
    void process();

private:
    tgt::ivec2 cropToViewport(tgt::ivec2 mousePos);

    RenderPort imgInport_;
    RenderPort fhpInport_;
    RenderPort outport_;

    EventProperty<DistanceMeasure>* mouseEventProp_;
    CameraProperty camera_;
    StringProperty unit_;
    FloatProperty unitFactor_;
    IntProperty numDigits_;
    BoolProperty renderSpheres_;

    tgt::ivec2 mouseCurPos2D_;
    tgt::vec4 mouseCurPos3D_;
    tgt::ivec2 mouseStartPos2D_;
    tgt::vec4 mouseStartPos3D_;
    bool mouseDown_;

    float distance_;

    tgt::Font* font_;
};

} // namespace

#endif // VRN_DISTANCEMEASURE_H
