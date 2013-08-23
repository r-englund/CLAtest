#include <inviwo/core/ports/volumeport.h>


namespace inviwo {

// Volume Inport
VolumeInport::VolumeInport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
: DataInport<Volume>(identifier, invalidationLevel)
{}

VolumeInport::~VolumeInport() {
}

void VolumeInport::initialize() {}

void VolumeInport::deinitialize() {}


uvec3 VolumeInport::getColorCode() const { 
    return uvec3(188,101,101); 
}

// Volume Outport
VolumeOutport::VolumeOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
: DataOutport<Volume>(identifier, invalidationLevel)
{
}

VolumeOutport::~VolumeOutport() {}

void VolumeOutport::initialize() {}

void VolumeOutport::deinitialize() {}

uvec3 VolumeOutport::getColorCode() const { 
    return uvec3(188,101,101); 
}

} // namespace