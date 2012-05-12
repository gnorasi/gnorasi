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

#ifndef VRN_SLICEPOSITIONRENDERER_H
#define VRN_SLICEPOSITIONRENDERER_H

#include "voreen/core/processors/geometryrendererbase.h"

#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/vectorproperty.h"

#include "voreen/core/ports/volumeport.h"

namespace voreen {

class SlicePositionRenderer : public GeometryRendererBase {
public:
    SlicePositionRenderer();

    Processor* create() const { return new SlicePositionRenderer; }
    std::string getCategory() const  { return "Geometry"; }
    std::string getClassName() const { return "SlicePositionRenderer"; }
    CodeState getCodeState() const   { return CODE_STATE_STABLE; }

    void render();
    void invalidate(int inv = INVALID_RESULT);

protected:
    void process();

private:
    void togglePropertyVisibility();

    BoolProperty renderXSlice_;
    FloatVec4Property xColor_;
    IntProperty xSliceIndexProp_;

    BoolProperty renderYSlice_;
    FloatVec4Property yColor_;
    IntProperty ySliceIndexProp_;

    BoolProperty renderZSlice_;
    FloatVec4Property zColor_;
    IntProperty zSliceIndexProp_;

    FloatProperty width_;
    IntProperty stippleFactor_;
    IntProperty stipplePattern_;
    BoolProperty renderPlanes_;
    FloatProperty planeAlphaFactor_;
    VolumePort inport_;
};

}

#endif

