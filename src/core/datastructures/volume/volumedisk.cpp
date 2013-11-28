#include <inviwo/core/datastructures/volume/volumedisk.h>
#include "inviwo/core/io/datvolumereader.h"
#include "inviwo/core/io/ivfvolumereader.h"

namespace inviwo {

    VolumeDisk::VolumeDisk(uvec3 dimensions, const DataFormatBase* format)
        : VolumeRepresentation(dimensions, format), DiskRepresentation(){
    }

    VolumeDisk::VolumeDisk(std::string srcFile, uvec3 dimensions, const DataFormatBase* format)
        : VolumeRepresentation(dimensions, format), DiskRepresentation(srcFile){
        initialize();
    }

    VolumeDisk* VolumeDisk::clone() const {
        return new VolumeDisk(*this);
    }

    VolumeDisk::~VolumeDisk() {}

    void VolumeDisk::initialize() {}
    void VolumeDisk::deinitialize() {}


} // namespace
