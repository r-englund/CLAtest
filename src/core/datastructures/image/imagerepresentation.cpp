#include <inviwo/core/datastructures/image/imagerepresentation.h>

namespace inviwo {

ImageRepresentation::ImageRepresentation(uvec2 dimensions, ImageType type, const DataFormatBase* format)
    : DataRepresentation(format), imageType_(type), dimensions_(dimensions)
{

}

ImageRepresentation::~ImageRepresentation() {}

void ImageRepresentation::resize(uvec2 dimensions){
    dimensions_ = dimensions;
}    


} // namespace
