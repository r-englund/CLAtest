#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/datastructures/geometry/meshram.h>

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
    representations_.push_back(new MeshRAM());
}

} // namespace
