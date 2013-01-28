#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/datastructures/volumetypeclassification.h>

namespace inviwo {

VolumePort::VolumePort(PortDirection direction, std::string identifier)
    : DataPort<Volume>(direction, identifier)
{}

VolumePort::~VolumePort() {
}

void VolumePort::initialize() {
    data_ = new StandardVolume();
}

void VolumePort::deinitialize() {
    delete data_;
}

} // namespace
