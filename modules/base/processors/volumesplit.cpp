#include "volumesplit.h"
#include <inviwo/core/datastructures/volumeramsubset.h>
#include <inviwo/core/inviwocore.h>
#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/util/vectoroperations.h>
#include <modules/opengl/openglmodule.h>

namespace inviwo {

VolumeSplit::VolumeSplit()
    : Processor(),
      limitingMemory_("memoryResource", "Limiting Memory Resource", "GPU (TextureGL)"),
      memUsageStrategy_("maxMemCondition", "Memory Usage Strategy", "Percentage of Available Memory"),
      memPercentage_("percentageOfMem", "Percentage of Available Memory", 60, 1, 100),
      brickSizeMem_("brickSizeMem", "Brick Size (MB)", 100, 1, 10000),
      numBricks_("numBricks", "Number of Bricks (per Dim)", ivec3(1), ivec3(1), ivec3(64)),
      brickSizeVoxel_("brickSizeVoxel", "Brick Size (Voxel)", "32"), //TODO: set min and max for this and bricks per dim based on volume dimensions
      borderWidth_("borderWidth", "Border Width", 2, 0, 16),
      camera_("camera", "Camera", vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 1.0f, 0.0f)),
      inport_(Port::INPORT, "volume.in"),
      outport_(Port::OUTPORT, "volumebrick.out")
{
    limitingMemory_.addOption("CPU (Main Memory)", CPU);
    limitingMemory_.addOption("GPU (TextureGL)", GPU);
    //limitingMemory_.setSelectedOption("GPU (TextureGL)");
    addProperty(limitingMemory_);
    //limitingMemory_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::invalidateBricking));

    memUsageStrategy_.addOption("Percentage of Available Memory", MEM_PERCENTAGE);
    memUsageStrategy_.addOption("Fixed Memory Size", MEM_FIXED);
    //memUsageStrategy_.setSelectedOption("Percentage of Available Memory");
    addProperty(memUsageStrategy_);
    //memUsageStrategy_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::invalidateBricking));

    addProperty(memPercentage_);
    //memPercentage_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::invalidateBricking));
    
    addProperty(brickSizeMem_);
    //brickSizeMem_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::invalidateBricking));
    
    addProperty(numBricks_);
    //numBricks_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::invalidateBricking));

    // TODO: query maximum volume dimensions
    //Add values which have a power of two
    brickSizeVoxel_.addOption("1", 1);
    brickSizeVoxel_.addOption("2", 2);
    brickSizeVoxel_.addOption("4", 4);
    brickSizeVoxel_.addOption("8", 8);
    brickSizeVoxel_.addOption("16", 16);
    brickSizeVoxel_.addOption("32", 32);
    brickSizeVoxel_.addOption("64", 64);
    brickSizeVoxel_.addOption("128", 128);
    brickSizeVoxel_.addOption("256", 256);
    brickSizeVoxel_.addOption("512", 512);
    brickSizeVoxel_.addOption("1024", 1024);
    brickSizeVoxel_.addOption("2048", 2048);
    addProperty(brickSizeVoxel_);
    //brickSizeVoxel_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::invalidateBricking));

    addProperty(borderWidth_);
    //borderWidth_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::invalidateBricking));

    addProperty(camera_);
    //camera_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::cameraChanged));

    addPort(inport_);
    addPort(outport_);

    openGLInfoRef_ = NULL;
    systemInfoRef_ = NULL;
}

Processor* VolumeSplit::create() const {
    return new VolumeSplit();
}

uint64_t VolumeSplit::getVolumeMemorySizeInBytes(ivec3 dims, int bits){
    return static_cast<uint64_t>(static_cast<float>(dims.x*dims.y*dims.z*bits)/8.0f);
}

bool VolumeSplit::dataFitsIntoMemory(MaximumMemoryCondition memCond, uint64_t availableMemSize, uint64_t currentBrickMemSize){
    // TODO: introduce member variable dedicatedMemory_ which is updated when the according properties have been changed
    if (memCond == MEM_PERCENTAGE)
        return (availableMemSize*(static_cast<double>(memPercentage_.get())/100.0) >= currentBrickMemSize);
    else
        return static_cast<double>(brickSizeMem_.get()*1024*1024) >= currentBrickMemSize && availableMemSize >= currentBrickMemSize;
}

//Always divide largest dimension to get least number of bricks, which are as cubed as possible
ivec3 VolumeSplit::calculateBestBrickDimension(MaximumMemoryCondition memCond, uint64_t availableMemSize, ivec3 volDims, int volBits){
    ivec3 currentBrickDims = volDims;
    while (!dataFitsIntoMemory(memCond, availableMemSize, getVolumeMemorySizeInBytes(currentBrickDims, volBits))) {
        int theMaxDim = (currentBrickDims.x > currentBrickDims.y ? (currentBrickDims.x > currentBrickDims.z ? 0 : 2) : (currentBrickDims.y > currentBrickDims.z ? 1 : 2));
        if (currentBrickDims[theMaxDim] % 2 != 0)
            currentBrickDims[theMaxDim]++; //Make the dim we are dividing even
        currentBrickDims[theMaxDim] /= 2;
    }
    if (limitingMemory_.getSelectedValue() == GPU) {
        // adapt brick size according to maximum texture dimension
        int maxGPUTextureDim = (openGLInfoRef_ ? openGLInfoRef_->getMax3DTexSize() : 1);
        while (currentBrickDims.x>maxGPUTextureDim || currentBrickDims.y>maxGPUTextureDim || currentBrickDims.z>maxGPUTextureDim) {
            int theMaxDim = (currentBrickDims.x > currentBrickDims.y ? (currentBrickDims.x > currentBrickDims.z ? 0 : 2) : (currentBrickDims.y > currentBrickDims.z ? 1 : 2));
            if (currentBrickDims[theMaxDim] % 2 != 0)
                currentBrickDims[theMaxDim]++; //Make the dim we are dividing even
            currentBrickDims[theMaxDim] /= 2;
        }
    }
    return currentBrickDims;
}

Volume* VolumeSplit::createNewVolume(VolumeSplit::Brick brick){
    Volume* volume;
    if (inport_.getData()->hasRepresentation<VolumeRepresentation>()){
        VolumeRepresentation* vol = inport_.getData()->getRepresentation<VolumeRepresentation>();
        volume = new Volume(VolumeRAMSubSet::apply(vol, brick.dim, brick.offset, VolumeRepresentation::VolumeBorders(brick.borderLlf, brick.borderUrb)), inport_.getData());
    }

    vec3 offsetTexCoords = (static_cast<vec3>(brick.offset)/static_cast<vec3>(inport_.getData()->getDimension()));//*inport_.getData()->getCubeSize();
    offsetTexCoords += inport_.getData()->getOffset();
    volume->setMetaData<Vec3MetaData>("Offset", offsetTexCoords);

    return volume;
}

// perform operations based on camera changes
void VolumeSplit::cameraChanged() {
    startSettings_ = true;
}

// perform operations based on camera changes
void VolumeSplit::startSettings() {
    if (connectedToImageCompositor()){
        //calculateCameraBrickDistance(bricks_, camera_..getPosition(), inport_.getData()->getDimensions(), inport_.getData()->getTextureToWorldMatrix());
        std::sort(bricks_.begin(), bricks_.end());
    }
    startSettings_ = false;
}

// check if connected to image compositor
bool VolumeSplit::connectedToImageCompositor(){
    /*const std::vector<const Port*> loopConnectedPorts = loopInport_.getConnected();
    size_t i = 0;
    while (i<loopConnectedPorts.size()){
        if (loopConnectedPorts[i]->getProcessor()->getName() == "BrickLoopImageCompositor"){
            return true;
        }
        i++;
    }*/

    return false;
}

void VolumeSplit::calculateCameraBrickDistance(std::vector<VolumeSplit::Brick>& bricks, vec3 cameraposition, uvec3 dimension, mat4 toWorld) {
    for (std::vector<VolumeSplit::Brick>::iterator it = bricks.begin(); it!=bricks.end(); ++it) {
        vec4 brickCenterTexCoords = vec4((*it).getCenterPosNormalized(dimension), 1.f);
        vec4 brickCenterWorldCoords = toWorld*brickCenterTexCoords;
        vec3 brickCenterWorldCoordsWYZ = brickCenterWorldCoords.xyz();
        (*it).distanceFromCamera = glm::length(cameraposition - brickCenterWorldCoordsWYZ);
    }
}

// calculates the bricking division and initializes the bricks_ data structure
void VolumeSplit::performBricking() {
    // determine brick size based on property values
    ivec3 originalVolDim = inport_.getData()->getDimension();
    ivec3 brickDim = originalVolDim;

    if (limitingMemory_.getSelectedValue() == GPU) {
        uint64_t gpuMemInBytes = (openGLInfoRef_ ? static_cast<uint64_t>(openGLInfoRef_->getCurrentAvailableTextureMem()) : 0);
        brickDim = calculateBestBrickDimension(memUsageStrategy_.getSelectedValue(), gpuMemInBytes, brickDim, inport_.getData()->getDataFormat().getBitsStored());
    }
    else{
        uint64_t physMemInBytes = (systemInfoRef_ ? systemInfoRef_->getAvailableMemory() : 0);
        brickDim = calculateBestBrickDimension(memUsageStrategy_.getSelectedValue(), physMemInBytes, brickDim, inport_.getData()->getDataFormat().getBitsStored());
    }

    // bound brick size by maximal volume size
    brickDim = glm::min(brickDim, originalVolDim);

    // compute brick layout if bricks are smaller than original volume
    bricks_.clear();
    if (brickDim != originalVolDim){
      
        // calculate number of bricks per dimension
        uvec3 bricksPerDim = uvec3(glm::ceil(static_cast<vec3>(originalVolDim)/static_cast<vec3>(brickDim)));

        // set number of iterations based on number of bricks
        size_t numBricksTotal = bricksPerDim.x*bricksPerDim.y*bricksPerDim.z;

        // calculate all other offsets and brick sizes in advance.
        for (size_t brickX=0; brickX<bricksPerDim.x; brickX++) {
            for (size_t brickY=0; brickY<bricksPerDim.y; brickY++) {
                for (size_t brickZ=0; brickZ<bricksPerDim.z; brickZ++) {
                    //calculate brick offset
                    ivec3 brickOffset = ivec3(static_cast<int>(brickX*brickDim.x), static_cast<int>(brickY*brickDim.y), static_cast<int>(brickZ*brickDim.z));
                    //calculate valid brick dimensions
                    uvec3 actualBrickDim = uvec3(brickDim - glm::max((brickOffset + brickDim) - originalVolDim, ivec3(0,0,0)));
                    //define valid borders
                    uvec3 borderLLF = uvec3(brickOffset - ivec3(glm::max(ivec3(0), brickOffset - ivec3(borderWidth_.get()))));
                    uvec3 borderURB = uvec3(glm::min(originalVolDim - (brickOffset + ivec3(actualBrickDim)), ivec3(borderWidth_.get())));
                    // push brick into data structure
                    bricks_.push_back(VolumeSplit::Brick(actualBrickDim, uvec3(brickOffset), borderLLF, borderURB));
                }
            }
        }

        // sort brick sizes and offsets based on distance to camera if we are connect to BrickLoopImageCompositor
        startSettings();
    }

    //Set hasChanged to false
    //inport_.setValid();
    updateBricks_ = false;
}

void VolumeSplit::invalidateBricking() {
    updateBricks_ = true;
    invalidate();
}

void VolumeSplit::process(){ 
    Volume* volHandle = NULL;
    /*if (inport_.hasData()) {
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
        outport_.setData(const_cast<VolumeBase*>(inport_.getData()), false); //Don't take ownership or we can't use it again*/
}

void VolumeSplit::initialize() {
    Processor::initialize();

    if(!openGLInfoRef_){
        OpenGLModule* openGLModule = getTypeFromVector<OpenGLModule>(InviwoApplication::getRef().getModules());
        if(openGLModule)
            openGLInfoRef_ = getTypeFromVector<OpenGLInfo>(openGLModule->getResourceInfos());
    }

    if(!systemInfoRef_){
        InviwoCore* inviwoCoreModule = getTypeFromVector<InviwoCore>(InviwoApplication::getRef().getModules());
        if(inviwoCoreModule)
            systemInfoRef_ = getTypeFromVector<SystemInfo>(inviwoCoreModule->getResourceInfos());
    }

    //brickingStrategy_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::showAndHideProperties));
    //memUsageStrategy_.onChange(CallMemberAction<VolumeSplit>(this, &VolumeSplit::showAndHideProperties));

    showAndHideProperties();
}

void VolumeSplit::deinitialize() {
    Processor::deinitialize();
}

void VolumeSplit::showAndHideProperties(){
    /*memUsageStrategy_.setVisible(brickingStrategy_.getValue() == MIN_NUM_BRICKS);
    memPercentage_.setVisible(memUsageStrategy_.isVisible() && memUsageStrategy_.getValue() == MEM_PERCENTAGE);
    brickSizeMem_.setVisible(memUsageStrategy_.isVisible() && memUsageStrategy_.getValue() == MEM_FIXED);

    numBricks_.setVisible(brickingStrategy_.getValue() == NUM_BRICKS);
    brickSizeVoxel_.setVisible(brickingStrategy_.getValue() == BRICK_SIZE);*/
}

} // inviwo namespace
