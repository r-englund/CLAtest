#include <inviwo/core/datastructures/volumerepresentation.h>

namespace inviwo {

    VolumeRepresentation::VolumeRepresentation(ivec3 dimension, std::string format)
        : DataRepresentation(),
        dimensions_(dimension),
        dataFormat_(format)
    {}

    VolumeRepresentation::~VolumeRepresentation() {}

} // namespace
