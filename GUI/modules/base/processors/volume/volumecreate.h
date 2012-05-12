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

#ifndef VRN_VOLUMECREATE_H
#define VRN_VOLUMECREATE_H

#include <string>
#include "voreen/core/processors/volumecreatebase.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/boolproperty.h"

namespace voreen {

class VolumeCreate : public VolumeCreateBase {
public:
    VolumeCreate();

    std::string getClassName() const  { return "VolumeCreate";      }
    std::string getCategory() const   { return "Volume Processing"; }
    CodeState getCodeState() const    { return CODE_STATE_STABLE;  }
    Processor* create() const { return new VolumeCreate; }

protected:
    void process();

private:
    void createCornell(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createCube(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createBlobs(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createBlobs2(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createBlobs3(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createSphere(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createDoubleSphere(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createTorus(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createDoubleTorus(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createDoublePartialTorus(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createBumpySphere(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createSphereCoord(const tgt::ivec3& dimensions, Volume4xUInt8* target);
    void createSynth(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createCloud(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createAOTestBox(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createShadowTestVolume(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createAorticArch(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createRandomShapes(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createDECT(const tgt::ivec3& dimensions, VolumeUInt8* target);
    void createStack(const tgt::ivec3& dimensions, VolumeUInt8* target);

    VolumePort outport_;
    unsigned int currentSeed_;

    static const std::string loggerCat_; ///< category used in logging

    StringOptionProperty operation_;
    IntProperty dimension_;
    ButtonProperty regenerate_;
    IntProperty numShapes_;
    BoolProperty keepCurrentShapes_;

    IntProperty numSubdivisions_;
};

}   //namespace

#endif
