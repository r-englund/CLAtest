#include <inviwo/core/datastructures/volumerepresentation.h>

namespace inviwo {

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension)
        : DataRepresentation(),
        dimensions_(dimension),
        dataFormatBase_(DataUINT8()),
        borders_(VolumeBorders())
    {}

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension, DataFormatBase format)
        : DataRepresentation(),
        dimensions_(dimension),
        dataFormatBase_(format),
        borders_(VolumeBorders())
    {}

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension, DataFormatBase format, const VolumeBorders& border)
        : DataRepresentation(),
        dimensions_(dimension),
        dataFormatBase_(format),
        borders_(border)

    {}

    VolumeRepresentation::~VolumeRepresentation() {}

} // namespace
