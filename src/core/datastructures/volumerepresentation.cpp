#include <inviwo/core/datastructures/volumerepresentation.h>

namespace inviwo {

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension, std::string format)
        : DataRepresentation(),
        dimensions_(dimension),
        dataFormat_(format),
        borders_(VolumeBorders())
    {}

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension, std::string format, const VolumeBorders& border)
        : DataRepresentation(),
        dimensions_(dimension),
        dataFormat_(format),
        borders_(border)

    {}

    VolumeRepresentation::~VolumeRepresentation() {}

} // namespace
