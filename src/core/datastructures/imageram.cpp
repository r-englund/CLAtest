#include "inviwo/core/datastructures/imageram.h"

namespace inviwo {

    ImageRAM::ImageRAM()
        : ImageRepresentation()
    {}

    ImageRAM::~ImageRAM() {}  

    void ImageRAM::initialize() {}

    void ImageRAM::deinitialize() {}

    DataRepresentation* ImageRAM::clone() {
        return new ImageRAM();
    }

} // namespace
