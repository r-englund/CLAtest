#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/datastructures/geometry/simplemeshram.h>

namespace inviwo {

// Geometry Inport
GeometryInport::GeometryInport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
: DataInport<Geometry>(identifier, invalidationLevel)
{}

GeometryInport::~GeometryInport() {
}

void GeometryInport::initialize() {}

void GeometryInport::deinitialize() {}


uvec3 GeometryInport::getColorCode() const { 
    return uvec3(188,188,101); 
}

// Geometry Outport
GeometryOutport::GeometryOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
: DataOutport<Geometry>(identifier, invalidationLevel)
{
    data_ = new SimpleMeshRAM();
}

GeometryOutport::~GeometryOutport() {}

void GeometryOutport::initialize() {}

void GeometryOutport::deinitialize() {}

uvec3 GeometryOutport::getColorCode() const { 
    return uvec3(188,188,101); 
}

} // namespace