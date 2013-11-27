#include "volumesubset.h"
#include <inviwo/core/datastructures/volume/volumeramsubset.h>

namespace inviwo {

ProcessorClassName(VolumeSubset, "VolumeSubset"); 
ProcessorCategory(VolumeSubset, "Volume Operation");
ProcessorCodeState(VolumeSubset, CODE_STATE_STABLE);

VolumeSubset::VolumeSubset()
    : Processor(),
    inport_("volume.inport"),
    outport_("volume.outport"),
    enabled_("enabled", "Enable Operation", true),
    rangeX_("rangeX", "X Slices", 0, 256, 0, 256),
    rangeY_("rangeY", "Y Slices", 0, 256, 0, 256),
    rangeZ_("rangeZ", "Z Slices", 0, 256, 0, 256)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(enabled_);
    addProperty(rangeX_);
    addProperty(rangeY_);
    addProperty(rangeZ_);

    dims_ = uvec3(1,1,1);
}

VolumeSubset::~VolumeSubset() {}

void VolumeSubset::initialize() {
    Processor::initialize();
}

void VolumeSubset::deinitialize() {
    Processor::deinitialize();
}

void VolumeSubset::process(){
    if(dims_ != inport_.getData()->getDimension()){
        dims_ = inport_.getData()->getDimension();
        rangeX_.setRangeMax(static_cast<int>(dims_.x));
        rangeY_.setRangeMax(static_cast<int>(dims_.y));
        rangeZ_.setRangeMax(static_cast<int>(dims_.z));
    }

    if(enabled_.get()){
         const VolumeRAM* vol = inport_.getData()->getRepresentation<VolumeRAM>();

         uvec3 dim = uvec3(static_cast<unsigned int>(rangeX_.get().y), static_cast<unsigned int>(rangeY_.get().y), static_cast<unsigned int>(rangeZ_.get().y));
         uvec3 offset = uvec3(static_cast<unsigned int>(rangeX_.get().x), static_cast<unsigned int>(rangeY_.get().x), static_cast<unsigned int>(rangeZ_.get().x));
         dim -= offset;

         if(dim == dims_)
             outport_.setConstData(inport_.getData());
         else{
             Volume* volume = new Volume(VolumeRAMSubSet::apply(vol, dim, offset));
             outport_.setData(volume);
         }
    }
    else
        outport_.setConstData(inport_.getData());
}

} // inviwo namespace
