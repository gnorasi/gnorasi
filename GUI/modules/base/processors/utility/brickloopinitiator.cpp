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

#include "brickloopinitiator.h"
#include <iomanip>
#include "tgt/glmath.h"
#include "tgt/physmem.h"
#include "voreen/core/datastructures/volume/diskrepresentation.h"
#include "voreen/core/datastructures/volume/volumegl.h"

namespace voreen {

const std::string BrickLoopInitiator::loggerCat_("voreen.experimental.BrickLoopInitiator");

BrickLoopInitiator::BrickLoopInitiator()
    : Processor(),
      limitingMemory_("memoryResource", "Limiting Memory Resource"),
      brickingStrategy_("brickingStrategy", "Bricking Strategy"),
      memUsageStrategy_("maxMemCondition", "Memory Usage Strategy"),
      memPercentage_("percentageOfMem", "Percentage of Available Memory", 60, 1, 100),
      brickSizeMem_("brickSizeMem", "Brick Size (MB)", 100, 1, 10000),
      numBricks_("numBricks", "Number of Bricks (per Dim)", tgt::ivec3(1), tgt::ivec3(1), tgt::ivec3(64)),
      brickSizeVoxel_("brickSizeVoxel", "Brick Size (Voxel)"), //TODO: set min and max for this and bricks per dim based on volume dimensions
      borderWidth_("borderWidth", "Border Width", 2, 0, 16),
      camera_("camera", "Camera", tgt::Camera()),
      inport_(Port::INPORT, "volume.in"),
      outport_(Port::OUTPORT, "volumebrick.out"),
      loopInport_("loop.in", Port::INPORT)
{
    limitingMemory_.addOption("CPU", "CPU (Main Memory)", CPU);
    limitingMemory_.addOption("GPU", "GPU (TextureGL)", GPU);
    limitingMemory_.select("GPU");
    addProperty(limitingMemory_);
    limitingMemory_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::invalidateBricking));

    brickingStrategy_.addOption("MIN", "Minimum Number of Bricks", MIN_NUM_BRICKS);
    brickingStrategy_.addOption("DIV", "Specific Number of Bricks", NUM_BRICKS);
    brickingStrategy_.addOption("SIZE", "Fixed Size Bricks", BRICK_SIZE);
    brickingStrategy_.select("DIV");
    addProperty(brickingStrategy_);
    brickingStrategy_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::invalidateBricking));

    memUsageStrategy_.addOption("PERCENTAGE", "Percentage of Available Memory", MEM_PERCENTAGE);
    memUsageStrategy_.addOption("SIZE", "Fixed Memory Size", MEM_FIXED);
    memUsageStrategy_.select("PERCENTAGE");
    addProperty(memUsageStrategy_);
    memUsageStrategy_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::invalidateBricking));

    addProperty(memPercentage_);
    memPercentage_.setTracking(false);
    memPercentage_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::invalidateBricking));
    addProperty(brickSizeMem_);
    brickSizeMem_.setTracking(false);
    brickSizeMem_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::invalidateBricking));
    addProperty(numBricks_);
    numBricks_.setTracking(false);
    numBricks_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::invalidateBricking));

    // TODO: query maximum volume dimensions
    //Add values which have a power of two
    brickSizeVoxel_.addOption("1", "1", 1);
    brickSizeVoxel_.addOption("2", "2", 2);
    brickSizeVoxel_.addOption("4", "4", 4);
    brickSizeVoxel_.addOption("8", "8", 8);
    brickSizeVoxel_.addOption("16", "16", 16);
    brickSizeVoxel_.addOption("32", "32", 32);
    brickSizeVoxel_.addOption("64", "64", 64);
    brickSizeVoxel_.addOption("128", "128", 128);
    brickSizeVoxel_.addOption("256", "256", 256);
    brickSizeVoxel_.addOption("512", "512", 512);
    brickSizeVoxel_.addOption("1024", "1024", 1024);
    brickSizeVoxel_.addOption("2048", "2048", 2048);
    brickSizeVoxel_.select("32");
    addProperty(brickSizeVoxel_);
    brickSizeVoxel_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::invalidateBricking));

    addProperty(borderWidth_);
    borderWidth_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::invalidateBricking));

    addProperty(camera_);
    camera_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::cameraChanged));

    addPort(inport_);
    addPort(outport_);
    addPort(loopInport_);

    loopInport_.setLoopPort(true);
}

Processor* BrickLoopInitiator::create() const {
    return new BrickLoopInitiator();
}

bool BrickLoopInitiator::isReady() const {
    return (inport_.isReady() && outport_.isReady());
}

unsigned int BrickLoopInitiator::getVolumeMemorySizeInBytes(tgt::svec3 dims, int bits){
    return static_cast<unsigned int>(static_cast<float>(dims.x*dims.y*dims.z*bits)/8.0f);
}

bool BrickLoopInitiator::dataFitsIntoMemory(MaximumMemoryCondition memCond, unsigned int availableMemSize, unsigned int currentBrickMemSize){
    // TODO: introduce member variable dedicatedMemory_ which is updated when the according properties have been changed
    if (memCond == MEM_PERCENTAGE)
        return (availableMemSize*(static_cast<double>(memPercentage_.get())/100.0) >= currentBrickMemSize);
    else
        return static_cast<double>(brickSizeMem_.get()*1024*1024) >= currentBrickMemSize && availableMemSize >= currentBrickMemSize;
}

//Always divide largest dimension to get least number of bricks, which are as cubed as possible
tgt::svec3 BrickLoopInitiator::calculateBestBrickDimension(MaximumMemoryCondition memCond, unsigned int availableMemSize, tgt::svec3 volDims, int volBits){
    tgt::svec3 currentBrickDims = volDims;
    while (!dataFitsIntoMemory(memCond, availableMemSize, getVolumeMemorySizeInBytes(currentBrickDims, volBits))) {
        int theMaxDim = (currentBrickDims.x > currentBrickDims.y ? (currentBrickDims.x > currentBrickDims.z ? 0 : 2) : (currentBrickDims.y > currentBrickDims.z ? 1 : 2));
        if (currentBrickDims[theMaxDim] % 2 != 0)
            currentBrickDims[theMaxDim]++; //Make the dim we are dividing even
        currentBrickDims[theMaxDim] /= 2;
    }
    if (limitingMemory_.getValue() == GPU) {
        // adapt brick size according to maximum texture dimension
        unsigned int maxGPUTextureDim = static_cast<unsigned int>(GpuCaps.getMax3DTextureSize());
        while (currentBrickDims.x>maxGPUTextureDim || currentBrickDims.y>maxGPUTextureDim || currentBrickDims.z>maxGPUTextureDim) {
            int theMaxDim = (currentBrickDims.x > currentBrickDims.y ? (currentBrickDims.x > currentBrickDims.z ? 0 : 2) : (currentBrickDims.y > currentBrickDims.z ? 1 : 2));
            if (currentBrickDims[theMaxDim] % 2 != 0)
                currentBrickDims[theMaxDim]++; //Make the dim we are dividing even
            currentBrickDims[theMaxDim] /= 2;
        }
    }
    return currentBrickDims;
}

VolumeHandle* BrickLoopInitiator::createNewVolume(BrickLoopInitiator::Brick brick){
    VolumeHandle* volumeHandle;
    if (inport_.getData()->hasRepresentation<Volume>()){
        const Volume* vol = inport_.getData()->getRepresentation<Volume>();
        volumeHandle = new VolumeHandle(vol->getSubVolume(brick.dim, brick.offset, VolumeRepresentation::VolumeBorders(brick.borderLlf, brick.borderUrb)), inport_.getData());
    }
    else{
        const DiskRepresentation* disk = inport_.getData()->getRepresentation<DiskRepresentation>();
        volumeHandle = new VolumeHandle(disk->getSubVolume(brick.dim, brick.offset, VolumeRepresentation::VolumeBorders(brick.borderLlf, brick.borderUrb)), inport_.getData());
    }

    tgt::vec3 offsetTexCoords = (static_cast<tgt::vec3>(brick.offset)/static_cast<tgt::vec3>(inport_.getData()->getDimensions()))*inport_.getData()->getCubeSize();
    offsetTexCoords += inport_.getData()->getOffset();
    volumeHandle->setMetaDataValue<Vec3MetaData>("Offset", offsetTexCoords);

    return volumeHandle;
}

// perform operations based on camera changes
void BrickLoopInitiator::cameraChanged() {
    startSettings_ = true;
}

// perform operations based on camera changes
void BrickLoopInitiator::startSettings() {
    if (connectedToImageCompositor()){
        calculateCameraBrickDistance(bricks_, camera_.get().getPosition(), inport_.getData()->getDimensions(), inport_.getData()->getTextureToWorldMatrix());
        std::sort(bricks_.begin(), bricks_.end());
    }
    startSettings_ = false;
}

// check if connected to BrickLoopImageCompositor
bool BrickLoopInitiator::connectedToImageCompositor(){
    const std::vector<const Port*> loopConnectedPorts = loopInport_.getConnected();
    size_t i = 0;
    while (i<loopConnectedPorts.size()){
        if (loopConnectedPorts[i]->getProcessor()->getName() == "BrickLoopImageCompositor"){
            return true;
        }
        i++;
    }

    return false;
}

void BrickLoopInitiator::calculateCameraBrickDistance(std::vector<BrickLoopInitiator::Brick>& bricks, tgt::vec3 cameraposition, tgt::svec3 dimension, tgt::mat4 toWorld) {
    for (std::vector<BrickLoopInitiator::Brick>::iterator it = bricks.begin(); it!=bricks.end(); ++it) {
        tgt::vec4 brickCenterTexCoords = tgt::vec4((*it).getCenterPosNormalized(dimension), 1.f);
        tgt::vec4 brickCenterWorldCoords = toWorld*brickCenterTexCoords;        
        (*it).distanceFromCamera = tgt::length(cameraposition - brickCenterWorldCoords.xyz());
    }
}

// calculates the bricking division and initializes the bricks_ data structure
void BrickLoopInitiator::performBricking() {
    // determine brick size based on property values
    tgt::svec3 originalVolDim = inport_.getData()->getDimensions();
    tgt::svec3 brickDim = originalVolDim;

    if (brickingStrategy_.getValue() == MIN_NUM_BRICKS) {
        if (limitingMemory_.getValue() == GPU) {
            int gpuMemInBytes = static_cast<int>(static_cast<float>(GpuCaps.getCurrentAvailableTextureMem()) * 1024.0f);
            brickDim = calculateBestBrickDimension(memUsageStrategy_.getValue(), gpuMemInBytes, brickDim, inport_.getData()->getBitsStored());
        }
        else{
            int physMemInBytes = tgt::ifloor(tgt::getAvailablePhysicalMemory());
            brickDim = calculateBestBrickDimension(memUsageStrategy_.getValue(), physMemInBytes, brickDim, inport_.getData()->getBitsStored());
        }  
    }
    else if (brickingStrategy_.getValue() == NUM_BRICKS)
        brickDim = tgt::svec3(tgt::iceil(tgt::vec3(originalVolDim) / tgt::vec3(numBricks_.get())));
    else if (brickingStrategy_.getValue() == BRICK_SIZE)
        brickDim = tgt::svec3(brickSizeVoxel_.getValue());

    // bound brick size by maximal volume size
    brickDim = tgt::min(brickDim, originalVolDim);

    // compute brick layout if bricks are smaller than original volume
    bricks_.clear();
    if (brickDim != originalVolDim){
      
        // calculate number of bricks per dimension
        tgt::svec3 bricksPerDim = tgt::svec3(tgt::iceil(static_cast<tgt::vec3>(originalVolDim)/static_cast<tgt::vec3>(brickDim)));

        // set number of iterations based on number of bricks
        size_t numBricksTotal = bricksPerDim.x*bricksPerDim.y*bricksPerDim.z;
        loopInport_.setNumLoopIterations(static_cast<int>(numBricksTotal));

        // calculate all other offsets and brick sizes in advance.
        for (size_t brickX=0; brickX<bricksPerDim.x; brickX++) {
            for (size_t brickY=0; brickY<bricksPerDim.y; brickY++) {
                for (size_t brickZ=0; brickZ<bricksPerDim.z; brickZ++) {
                    //calculate brick offset
                    tgt::ivec3 brickOffset = tgt::ivec3(static_cast<int>(brickX*brickDim.x), static_cast<int>(brickY*brickDim.y), static_cast<int>(brickZ*brickDim.z));
                    //calculate valid brick dimensions
                    tgt::svec3 actualBrickDim = brickDim - tgt::svec3(tgt::max((brickOffset + tgt::ivec3(brickDim)) - tgt::ivec3(originalVolDim), tgt::ivec3::zero));
                    //define valid borders
                    tgt::svec3 borderLLF = brickOffset - tgt::max(tgt::ivec3(0), brickOffset - tgt::ivec3(borderWidth_.get()));
                    tgt::svec3 borderURB = tgt::svec3(tgt::min(tgt::ivec3(originalVolDim) - (brickOffset + tgt::ivec3(actualBrickDim)), tgt::ivec3(borderWidth_.get())));
                    // push brick into data structure
                    bricks_.push_back(BrickLoopInitiator::Brick(actualBrickDim, brickOffset, borderLLF, borderURB));
                }
            }
        }

        // sort brick sizes and offsets based on distance to camera if we are connect to BrickLoopImageCompositor
        startSettings();
    }
    else
        loopInport_.setNumLoopIterations(0);

    //Set hasChanged to false
    inport_.setValid();
    updateBricks_ = false;
}

void BrickLoopInitiator::invalidateBricking() {
    updateBricks_ = true;
    invalidate();
}

void BrickLoopInitiator::process(){ 
    LGL_ERROR;

    if (!inport_.isReady() || loopInport_.getNumConnections() < 1) {
        outport_.setData(0);
        return;
    }

    VolumeHandle* volHandle = NULL;
    if (inport_.hasData()) {
        if (inport_.hasChanged() || updateBricks_)
            performBricking();
        if (startSettings_)
            startSettings();
        if (!bricks_.empty())
            volHandle = createNewVolume(bricks_[loopInport_.getLoopIteration()]);
    }

    // put out result volume
    if (volHandle)
        outport_.setData(volHandle);
    else
        outport_.setData(const_cast<VolumeHandleBase*>(inport_.getData()), false); //Don't take ownership or we can't use it again
}

void BrickLoopInitiator::initialize() throw (tgt::Exception) {
    Processor::initialize();

    brickingStrategy_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::showAndHideProperties));
    memUsageStrategy_.onChange(CallMemberAction<BrickLoopInitiator>(this, &BrickLoopInitiator::showAndHideProperties));

    showAndHideProperties();
}

void BrickLoopInitiator::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

void BrickLoopInitiator::showAndHideProperties(){
    memUsageStrategy_.setVisible(brickingStrategy_.getValue() == MIN_NUM_BRICKS);
    memPercentage_.setVisible(memUsageStrategy_.isVisible() && memUsageStrategy_.getValue() == MEM_PERCENTAGE);
    brickSizeMem_.setVisible(memUsageStrategy_.isVisible() && memUsageStrategy_.getValue() == MEM_FIXED);

    numBricks_.setVisible(brickingStrategy_.getValue() == NUM_BRICKS);
    brickSizeVoxel_.setVisible(brickingStrategy_.getValue() == BRICK_SIZE);
}

} // voreen namespace
