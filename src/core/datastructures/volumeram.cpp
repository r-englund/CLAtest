#include <inviwo/core/datastructures/volumeram.h>
#include <inviwo/core/util/typetostring.h>

namespace inviwo {

    VolumeRAM::VolumeRAM()
        : VolumeRepresentation(uvec3(128,128,128), "UINT8"), data_(0), bytesPerVoxel_(0)
    {}

    VolumeRAM::VolumeRAM(uvec3 dimensions)
        : VolumeRepresentation(dimensions, "UINT8"), data_(0), bytesPerVoxel_(0)
    {}

    VolumeRAM::VolumeRAM(uvec3 dimensions, const VolumeBorders& border)
        : VolumeRepresentation(dimensions, "UINT8", border), data_(0), bytesPerVoxel_(0)
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

    size_t VolumeRAM::getBitsPerVoxel() const{
        return getBytesPerVoxel()*8;
    }

    size_t VolumeRAM::getBytesPerVoxel() const{
        return bytesPerVoxel_;
    }

} // namespace
