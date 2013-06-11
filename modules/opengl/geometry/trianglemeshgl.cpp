#include <modules/opengl/geometry/trianglemeshgl.h>

namespace inviwo {

TriangleMeshGL::TriangleMeshGL()
    : MeshGL()
{}

TriangleMeshGL::~TriangleMeshGL() {
    deinitialize();
}

void TriangleMeshGL::initialize() {}

void TriangleMeshGL::deinitialize() {}

void TriangleMeshGL::render(RenderType renderType){

}

TriangleMeshRAM2GLConverter::TriangleMeshRAM2GLConverter()
: RepresentationConverterType<GeometryGL>()
{}

TriangleMeshRAM2GLConverter::~TriangleMeshRAM2GLConverter() {}

DataRepresentation* TriangleMeshRAM2GLConverter::convert(DataRepresentation* source) {     
    TriangleMeshRAM* triangleMeshRAM = dynamic_cast<TriangleMeshRAM*>(source);
    if (triangleMeshRAM) {
        TriangleMeshGL* meshGL = new TriangleMeshGL();

        return meshGL;
    }
    return NULL;
}

} // namespace

