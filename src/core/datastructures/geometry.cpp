#include <inviwo/core/datastructures/geometry.h>

namespace inviwo {

Geometry::Geometry() : Data() {}

Data* Geometry::clone() const {
    return NULL;
}

Geometry::~Geometry() {
}

void Geometry::createDefaultRepresentation() const{
}

} // namespace
