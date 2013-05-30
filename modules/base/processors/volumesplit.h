#ifndef IVW_VOLUMESPLIT_H
#define IVW_VOLUMESPLIT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/util/systeminfo.h>
#include <modules/base/basemoduledefine.h>
#include <modules/opengl/openglinfo.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSplit : public Processor {
    /// Determines on which type of memory the volume should be stored.
    enum VolumeLocation {
        CPU = 0,
        GPU = 1
    };

    /// Determines which condition that should be used when trying to fit as large volume as possible
    enum MaximumMemoryCondition {
        MEM_PERCENTAGE = 0,
        MEM_FIXED = 1
    };

public:
    VolumeSplit();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    // struct Brick.
    struct Brick
    {
        uvec3 dim;
        uvec3 offset;
        uvec3 borderLlf;
        uvec3 borderUrb;
        float distanceFromCamera;

        Brick() : dim(uvec3(1,1,1)), offset(uvec3(0,0,0)), borderLlf(uvec3(0,0,0)), borderUrb(uvec3(0,0,0)), distanceFromCamera(0.f){}
        Brick(uvec3 size, uvec3 off, uvec3 llf, uvec3 urb, float dist = 0.f) : dim(size), offset(off), borderLlf(llf), borderUrb(urb), distanceFromCamera(dist){}

        vec3 getCenterPosNormalized(uvec3 dims){
            vec3 center = static_cast<vec3>(offset) + (static_cast<vec3>(dim)/2.f);
            return center/static_cast<vec3>(dims);
        }

        bool operator<(const struct Brick& b) const { 
            return distanceFromCamera > b.distanceFromCamera; 
        }
    };
    
    virtual void process();

    virtual void performBricking();
    virtual void invalidateBricking();
    virtual void showAndHideProperties();

    /// Property defining where the output volume is to be stored.
    TemplateOptionProperty<VolumeLocation> limitingMemory_;

    /// Property defining maximum memory condition.
    TemplateOptionProperty<MaximumMemoryCondition> memUsageStrategy_;

    IntProperty memPercentage_;
    IntProperty brickSizeMem_;
    IntVec3Property numBricks_;

    /// Power of two brick sizes.
    OptionPropertyInt brickSizeVoxel_;

    /// Define the width of the border used for border handling.
    IntProperty borderWidth_;

    /// Camera property.
    CameraProperty camera_;

    VolumeInport inport_;
    VolumeOutport outport_;

private:
    bool updateBricks_;
    bool startSettings_;
    bool volFillCPURequirements_;
    bool volFillGPURequirements_;
    std::vector<VolumeSplit::Brick> bricks_;
    static const std::string loggerCat_; ///< category used in logging

    OpenGLInfo* openGLInfoRef_;
    SystemInfo* systemInfoRef_;

    // calculate and return the size needed to store the volume 
    uint64_t getVolumeMemorySizeInBytes(ivec3 dims, size_t bits); 

    // check if data can be allocated according to the conditions specified
    bool dataFitsIntoMemory(MaximumMemoryCondition memCond, uint64_t memSize, uint64_t volMemSize);

    // return bricks size based on user defined and hardware-based limitations
    ivec3 calculateBestBrickDimension(MaximumMemoryCondition memCond, uint64_t availableMemSize, ivec3 volDims, size_t volBits);

    // create volume handle if necessary (take spacing and transformation from incoming volume)
    Volume* createNewVolume(VolumeSplit::Brick brick);

    // check if connected to BrickLoopImageCompositor
    bool connectedToImageCompositor();

    // perform first iteration settings
    void startSettings();

    // perform operations based on camera changes
    void cameraChanged();

    // update all distances of the bricks
    void calculateCameraBrickDistance(std::vector<VolumeSplit::Brick>& bricks, vec3 cameraposition, uvec3 dimension, mat4 toWorld);
};

}

#endif //IVW_VOLUMESPLIT_H
