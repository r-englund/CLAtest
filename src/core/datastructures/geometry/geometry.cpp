#include <inviwo/core/datastructures/geometry/geometry.h>

namespace inviwo {

Geometry::Geometry() : Data() {}

Geometry::Geometry(GeometryRepresentation* rep) : Data() {
    addRepresentation(rep);
}

Data* Geometry::clone() const {
    return NULL;
}

Geometry::~Geometry() {
}

void Geometry::createDefaultRepresentation() const{
}

} // namespace
