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

#ifndef VRN_BRICKLOOPINITIATOR_H
#define VRN_BRICKLOOPINITIATOR_H

#include "voreen/core/ports/volumeport.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/vectorproperty.h"
#include "voreen/core/properties/buttonproperty.h"
#include "voreen/core/properties/boolproperty.h"
#include "voreen/core/properties/cameraproperty.h"

namespace voreen {

/**
 * Create sub volumes to process through scene
 *
 * NOTICE: Presumes that largest allocation size is larger 
 * on main memory than GPU memory.
 */
class BrickLoopInitiator : public Processor {
    /// Determines on which type of memory the volume should be stored.
    enum VolumeLocation {
        CPU = 0,
        GPU = 1
    };
    /// Determines which property should calculate how large the sub volumes are.
    enum AllocationDirective {
        MIN_NUM_BRICKS = 0,
        NUM_BRICKS = 1,
        BRICK_SIZE = 2
    };

    /// Determines which condition that should be used when trying to fit as large volume as possible
    enum MaximumMemoryCondition {
        MEM_PERCENTAGE = 0,
        MEM_FIXED = 1
    };

public:
    BrickLoopInitiator();
    virtual Processor* create() const;

    virtual std::string getCategory() const  { return "Utility"; }
    virtual std::string getClassName() const { return "BrickLoopInitiator"; }
    virtual CodeState getCodeState() const   { return Processor::CODE_STATE_TESTING; }
    virtual bool isUtility() const           { return true; }

    virtual bool isReady() const;

protected:
    // struct Brick.
    struct Brick
    {
        tgt::svec3 dim;
        tgt::svec3 offset;
        tgt::svec3 borderLlf;
        tgt::svec3 borderUrb;
        float distanceFromCamera;

        Brick() : dim(tgt::svec3(1,1,1)), offset(tgt::svec3(0,0,0)), borderLlf(tgt::svec3(0,0,0)), borderUrb(tgt::svec3(0,0,0)), distanceFromCamera(0.f){}
        Brick(tgt::svec3 size, tgt::svec3 off, tgt::svec3 llf, tgt::svec3 urb, float dist = 0.f) : dim(size), offset(off), borderLlf(llf), borderUrb(urb), distanceFromCamera(dist){}

        tgt::vec3 getCenterPosNormalized(tgt::svec3 dims){
            tgt::vec3 center = static_cast<tgt::vec3>(offset) + (static_cast<tgt::vec3>(dim)/2.f);
            return center/static_cast<tgt::vec3>(dims);
        }

        bool operator<(const struct Brick& b) const { 
            return distanceFromCamera > b.distanceFromCamera; 
        }
    };
    
    virtual void process();
    virtual void initialize() throw (tgt::Exception);
    virtual void deinitialize() throw (tgt::Exception);
    virtual void performBricking();
    virtual void invalidateBricking();
    virtual void showAndHideProperties();

    /// Property defining where the output volume is to be stored.
    OptionProperty<VolumeLocation> limitingMemory_;

    /// Property defining how bricks should be created.
    OptionProperty<AllocationDirective> brickingStrategy_;

    /// Property defining maximum memory condition.
    OptionProperty<MaximumMemoryCondition> memUsageStrategy_;

    IntProperty memPercentage_;
    IntProperty brickSizeMem_;
    IntVec3Property numBricks_;

    /// Power of two brick sizes.
    OptionProperty<int> brickSizeVoxel_;

    /// Define the width of the border used for border handling.
    IntProperty borderWidth_;

    /// Camera property.
    CameraProperty camera_;

    VolumePort inport_;
    VolumePort outport_;
    Port loopInport_;

private:
    bool updateBricks_;
    bool startSettings_;
    bool volFillCPURequirements_;
    bool volFillGPURequirements_;
    std::vector<BrickLoopInitiator::Brick> bricks_;
    static const std::string loggerCat_; ///< category used in logging


    // calculate and return the size needed to store the volume 
    unsigned int getVolumeMemorySizeInBytes(tgt::svec3 dims, int bits); 

    // check if data can be allocated according to the conditions specified
    bool dataFitsIntoMemory(MaximumMemoryCondition memCond, unsigned int memSize, unsigned int volMemSize);

    // return bricks size based on user defined and hardware-based limitations
    tgt::svec3 calculateBestBrickDimension(MaximumMemoryCondition memCond, unsigned int availableMemSize, tgt::svec3 volDims, int volBits);

    // create volume handle if necessary (take spacing and transformation from incoming volume)
    VolumeHandle* createNewVolume(BrickLoopInitiator::Brick brick);

    // check if connected to BrickLoopImageCompositor
    bool connectedToImageCompositor();

    // perform first iteration settings
    void startSettings();

    // perform operations based on camera changes
    void cameraChanged();

    // update all distances of the bricks
    void calculateCameraBrickDistance(std::vector<BrickLoopInitiator::Brick>& bricks, tgt::vec3 cameraposition, tgt::svec3 dimension, tgt::mat4 toWorld);
};

}

#endif //VRN_BRICKLOOPINITIATOR_H
