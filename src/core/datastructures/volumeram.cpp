#include <inviwo/core/datastructures/volumeram.h>

namespace inviwo {

    VolumeRAM::VolumeRAM()
        : data_(0), VolumeRepresentation(ivec3(128,128,128), "UINT8")
    {}

    VolumeRAM::VolumeRAM(ivec3 dimensions)
        : data_(0), VolumeRepresentation(dimensions, "UINT8")
    {}

    VolumeRAM::~VolumeRAM() {}

    void VolumeRAM::initialize() {}

    void VolumeRAM::deinitialize() {}

    void* VolumeRAM::getData() {
        return data_;
    }

} // namespace
