#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

Geometry::Geometry() : SpatialData<3>() {}

Geometry::Geometry(GeometryRepresentation* rep) : SpatialData<3>() {
    addRepresentation(rep);
}

Geometry* Geometry::clone() const {
    return new Geometry(*this);
}

Geometry::~Geometry() {
}

DataRepresentation* Geometry::createDefaultRepresentation() {
    return NULL;
    //return new Mesh();  //Peter, Mesh is not a DataRepresentation? what to do?
}

} // namespace
