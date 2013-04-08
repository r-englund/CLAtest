#include <inviwo/core/datastructures/volumeram.h>
#include <inviwo/core/util/typetostring.h>

namespace inviwo {

    VolumeRAM::VolumeRAM(uvec3 dimensions, VolumeBorders border, DataFormatBase format)
        : VolumeRepresentation(dimensions, format, border), data_(0)
    {}

    VolumeRAM::~VolumeRAM() {}

    void VolumeRAM::initialize() {}

    void VolumeRAM::deinitialize() {}

    void* VolumeRAM::getData() {
        return data_;
    }

    const void* VolumeRAM::getData() const{
        return const_cast<void*>(data_);
    }

} // namespace
