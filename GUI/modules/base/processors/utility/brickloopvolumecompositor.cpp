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

#include "brickloopvolumecompositor.h"

namespace voreen {

using tgt::ivec3;

const std::string BrickLoopVolumeCompositor::loggerCat_("voreen.experimental.BrickLoopVolumeCompositor");

BrickLoopVolumeCompositor::BrickLoopVolumeCompositor()
    : Processor(),
      inport_(Port::INPORT, "volumebrick.in"),
      outport_(Port::OUTPORT, "volume.out"),
      loopOutport_("loop.out", Port::OUTPORT),
      volume_(0)
{
    loopOutport_.setLoopPort(true);

    addPort(inport_);
    addPort(outport_);
    addPort(loopOutport_);
}

BrickLoopVolumeCompositor::~BrickLoopVolumeCompositor() {
    delete volume_;
}

Processor* BrickLoopVolumeCompositor::create() const {
    return new BrickLoopVolumeCompositor();
}

bool BrickLoopVolumeCompositor::isReady() const {
    return (outport_.isReady());
}

void BrickLoopVolumeCompositor::defineNewOffset(){
    int iter = loopOutport_.getLoopIteration();
    if (iter>0){
        //move offset ahead
        tgt::svec3 volOutSize = inport_.getData()->getOriginalDimensions();
        currentOffset_.z += lastVolumeSize_.z;

        if(currentOffset_.z >= volOutSize.z){
            currentOffset_.y += lastVolumeSize_.y;
            currentOffset_.z = 0;
        }
        if(currentOffset_.y >= volOutSize.y){
            currentOffset_.x += lastVolumeSize_.x;
            currentOffset_.y = 0;
            currentOffset_.z = 0;
        }
    }
    else
        currentOffset_ = tgt::svec3(0,0,0);

    lastVolumeSize_ = inport_.getData()->getDimensions();
}

void BrickLoopVolumeCompositor::process() {

    //if no valid data, clear
    if (!inport_.isReady()) {
        delete volume_;
        volume_ = 0;
        currentOffset_ = tgt::svec3(0,0,0);
        lastVolumeSize_ = tgt::svec3(0,0,0);
        outport_.setData(0);
        return;
    }
    else if(inport_.hasChanged()){
        // first iteration: clear previous volume and create new one
        if (loopOutport_.getLoopIteration() == 0) {
            VolumeHandleBase* volumeHandle = outport_.getWritableData();
            if(volumeHandle) {
                VolumeHandle* temp = dynamic_cast<VolumeHandle*>(volumeHandle);
                temp->deleteAllRepresentations();
                delete temp;
                outport_.setData(0);
            }

            if (inport_.hasData())
                volume_ = inport_.getData()->getRepresentation<Volume>()->createNew(inport_.getData()->getOriginalDimensions(), VolumeRepresentation::VolumeBorders(), true);
        }

        // add volume as part of our volume
        if (inport_.hasData()){
            defineNewOffset();
            volume_->setSubVolume(inport_.getData()->getRepresentation<Volume>(), currentOffset_);
        }

        // last iteration: volume operations are complete => write it to outport
        if (loopOutport_.getLoopIteration() == loopOutport_.getNumLoopIterations()-1) {
            VolumeHandle* vh = new VolumeHandle(volume_, inport_.getData());
            oldVolumePosition(vh); //FIXME
            outport_.setData(vh, false);
            LINFOC("voreen.VolumeBrickLoopFinalizer" , "Brick Looping Completed After " << loopOutport_.getNumLoopIterations() << " Iterations.");
        }
    }

    LGL_ERROR;
}

void BrickLoopVolumeCompositor::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void BrickLoopVolumeCompositor::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

} // voreen namespace
