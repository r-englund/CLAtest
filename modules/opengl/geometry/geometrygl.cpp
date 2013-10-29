#include <modules/opengl/geometry/geometrygl.h>

namespace inviwo {

GeometryGL::GeometryGL()
    : GeometryRepresentation()
{}

GeometryGL::~GeometryGL() {
    deinitialize();
}

void GeometryGL::initialize() {}

void GeometryGL::deinitialize() {}

} // namespace

