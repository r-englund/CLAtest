#include <inviwo/core/datastructures/volumerepresentation.h>

namespace inviwo {

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension)
        : DataRepresentation(),
        dimensions_(dimension),
        borders_(VolumeBorders())
    {}

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension, DataFormatBase format)
        : DataRepresentation(format),
        dimensions_(dimension),
        borders_(VolumeBorders())
    {}

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension, DataFormatBase format, VolumeBorders border)
        : DataRepresentation(format),
        dimensions_(dimension),
        borders_(border)

    {}

    VolumeRepresentation::~VolumeRepresentation() {}

} // namespace
