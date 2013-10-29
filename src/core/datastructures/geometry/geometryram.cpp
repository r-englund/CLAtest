#include <inviwo/core/datastructures/geometry/geometryram.h>

namespace inviwo {

GeometryRAM::GeometryRAM()
    : GeometryRepresentation()
{}

GeometryRAM::~GeometryRAM() {
    deinitialize();
}

void GeometryRAM::initialize() {}

void GeometryRAM::deinitialize() {}

} // namespace

