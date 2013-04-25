#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/datastructures/volumetypeclassification.h>

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
{}

VolumeOutport::~VolumeOutport() {
}

void VolumeOutport::initialize() {
    data_ = new StandardVolume();
}

void VolumeOutport::deinitialize() {
    delete data_;
}

uvec3 VolumeOutport::getColorCode() const { 
    return uvec3(188,101,101); 
}

} // namespace