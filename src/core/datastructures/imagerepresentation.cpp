#include <inviwo/core/datastructures/imagerepresentation.h>

namespace inviwo {

    ImageRepresentation::ImageRepresentation()
        : DataRepresentation()
    {}

    ImageRepresentation::ImageRepresentation(uvec2 dimensions)
        : DataRepresentation(), dimensions_(dimensions)
    {

    }
    ImageRepresentation::ImageRepresentation(uvec2 dimensions, DataFormatBase format)
        : DataRepresentation(format), dimensions_(dimensions)
    {

    }

    ImageRepresentation::~ImageRepresentation() {}

    void ImageRepresentation::resize(uvec2 dimensions){
        dimensions_ = dimensions;
    }    


} // namespace
