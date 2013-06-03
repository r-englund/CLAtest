#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>

namespace inviwo {

// Volume Inport
VolumeInport::VolumeInport(std::string identifier)
: DataInport<Volume>(identifier)
{}

VolumeInport::~VolumeInport() {
}

void VolumeInport::initialize() {}

void VolumeInport::deinitialize() {}


uvec3 VolumeInport::getColorCode() const { 
    return uvec3(188,101,101); 
}

// Volume Outport
VolumeOutport::VolumeOutport(std::string identifier)
: DataOutport<Volume>(identifier)
{
    data_ = new StandardVolume();
}

VolumeOutport::~VolumeOutport() {
    delete data_;
}

void VolumeOutport::initialize() {}

void VolumeOutport::deinitialize() {}

uvec3 VolumeOutport::getColorCode() const { 
    return uvec3(188,101,101); 
}

} // namespace