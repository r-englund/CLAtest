#include <inviwo/core/datastructures/geometry/geometryrepresentation.h>

namespace inviwo {

GeometryRepresentation::GeometryRepresentation()
    : DataRepresentation()
{}

GeometryRepresentation::~GeometryRepresentation(){}

void GeometryRepresentation::performOperation(DataOperation*) const {}

std::string GeometryRepresentation::getClassName() const { 
    return "GeometryRepresentation"; 
}


} // namespace
