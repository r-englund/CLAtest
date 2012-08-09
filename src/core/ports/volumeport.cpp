#include "inviwo/core/ports/volumeport.h"

namespace inviwo {

    VolumePort::VolumePort(PortDirection direction, std::string identifier)
        : DataPort<Volume>(direction, identifier)
    {}

    VolumePort::~VolumePort() {
    }

    void VolumePort::initialize() {
        std::cout << "initializing port " << identifier_ << std::endl;
        data_ = new Volume();
    }

    void VolumePort::deinitialize() {
        delete data_;
    }

} // namespace
