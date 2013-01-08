#include "inviwo/core/datastructures/imageram.h"

namespace inviwo {

    ImageRAM::ImageRAM()
        : ImageRepresentation()
    {
        ImageRAM::initialize();
    }

    ImageRAM::ImageRAM(ivec2 dimension)
        : ImageRepresentation(dimension)

    {
        ImageRAM::initialize();
    }

    ImageRAM::~ImageRAM() {}  

    void ImageRAM::initialize() {
        data_ = new uint8_t[dimensions_.x*dimensions_.y];
    }
    
    void ImageRAM::deinitialize() {}

    DataRepresentation* ImageRAM::clone() {
        return new ImageRAM();
    }

} // namespace
