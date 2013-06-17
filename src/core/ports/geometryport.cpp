#include <inviwo/core/ports/geometryport.h>

namespace inviwo {

// Geometry Inport
GeometryInport::GeometryInport(std::string identifier)
: DataInport<Geometry>(identifier)
{}

GeometryInport::~GeometryInport() {
}

void GeometryInport::initialize() {}

void GeometryInport::deinitialize() {}


uvec3 GeometryInport::getColorCode() const { 
    return uvec3(188,188,101); 
}

// Geometry Outport
GeometryOutport::GeometryOutport(std::string identifier)
: DataOutport<Geometry>(identifier)
{
    data_ = new Geometry();
}

GeometryOutport::~GeometryOutport() {}

void GeometryOutport::initialize() {}

void GeometryOutport::deinitialize() {}

uvec3 GeometryOutport::getColorCode() const { 
    return uvec3(188,188,101); 
}

} // namespace