#include "inviwo/core/datastructures/image.h"
#include "inviwo/core/datastructures/imagerepresentation.h"

namespace inviwo {

    Image::Image() : Data() {
        representations_.clear();
    }

    Image::Image(ivec2 dimensions)
        : Data(),
          dimensions_(dimensions) {
        representations_.clear();
    }

    Image::~Image() {
        for (size_t i=0; i<representations_.size(); i++) {
            representations_[i]->deinitialize();
        }
        representations_.clear();
    }

    void Image::resize(ivec2 dimensions) {
        dimensions_ = dimensions;
        for (size_t i=0; i<representations_.size(); i++) {
            ImageRepresentation* imageRepresentation = dynamic_cast<ImageRepresentation*>(representations_[i]) ;
            if ( imageRepresentation ) {
                imageRepresentation->resize(dimensions_);
            }
        }
    }

} // namespace
