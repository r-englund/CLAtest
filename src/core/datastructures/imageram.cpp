#include <inviwo/core/datastructures/imageram.h>

namespace inviwo {

    ImageRAM::ImageRAM()
        : ImageRepresentation()
    {
        ImageRAM::initialize();
    }

    ImageRAM::ImageRAM(uvec2 dimension)
        : ImageRepresentation(dimension)
    {
        ImageRAM::initialize();
    }

    ImageRAM::ImageRAM(uvec2 dimension, DataFormatBase format)
        : ImageRepresentation(dimension, format), data_(NULL)
    {
        ImageRAM::initialize();

    }

    ImageRAM::~ImageRAM() {}  

    void ImageRAM::initialize() {
        
    }
    
    void ImageRAM::deinitialize() {}

    DataRepresentation* ImageRAM::clone() const {
        return new ImageRAM();
    }

} // namespace
