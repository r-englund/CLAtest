#include <inviwo/core/datastructures/imagedisk.h>

namespace inviwo {

    ImageDisk::ImageDisk()
        : ImageRepresentation()
    {}

    ImageDisk::~ImageDisk() {}

    void ImageDisk::initialize() {}

    void ImageDisk::deinitialize() {}

    DataRepresentation* ImageDisk::clone() {
        return new ImageDisk();
    }

} // namespace
