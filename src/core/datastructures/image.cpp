#include "inviwo/core/datastructures/image.h"

namespace inviwo {

    Image::Image() : Data() {
        representations_.clear();
    }

    Image::Image(ivec2 dimensions)
        : Data(),
          dimensions_(dimensions) {
        representations_.clear();
    }

    Image::~Image() {}

} // namespace
