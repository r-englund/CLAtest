#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

Geometry::Geometry() : SpatialData<3>() {}

Geometry::Geometry(GeometryRepresentation* rep) : SpatialData<3>() {
    addRepresentation(rep);
}

//Data* Geometry::clone() const {
//    Geometry* newGeometry = new Geometry();
//    copyRepresentations(newGeometry);
//    return newGeometry;
//}

Geometry::~Geometry() {

}

void Geometry::createDefaultRepresentation() {

    // FIXME: Should Geometry be a data object?
    //representations_.push_back(new Mesh());
}

} // namespace
