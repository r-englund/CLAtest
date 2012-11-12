#include "inviwo/core/ports/volumeport.h"

namespace inviwo {

    VolumePort::VolumePort(PortDirection direction, std::string identifier)
        : DataPort<Volume>(direction, identifier)
    {}

    VolumePort::~VolumePort() {
    }

    void VolumePort::initialize() {
        data_ = new Volume();
    }

    void VolumePort::deinitialize() {
        delete data_;
    }

    Volume* VolumePort::scaledData(Volume*, Processor*) {
        return getData();
    }

} // namespace
