#include <modules/opengl/geometry/meshglconverter.h>

namespace inviwo {

//MeshRAM2GLConverter::MeshRAM2GLConverter()
//: RepresentationConverterType<GeometryGL>()
//{}
//
//MeshRAM2GLConverter::~MeshRAM2GLConverter() {}
//
//DataRepresentation* MeshRAM2GLConverter::createFrom(const DataRepresentation* source) {     
//    const MeshRAM* meshRAM = dynamic_cast<const MeshRAM*>(source);
//    if (meshRAM) {
//        MeshGL* meshGL = new MeshGL(meshRAM->getAttributesInfo());
//        for (size_t i=0; i < meshRAM->getNumberOfAttributes(); ++i) {
//            meshGL->createArrayBuffer(meshRAM->getAttributes(i));
//            LGL_ERROR;
//        }
//        if(meshRAM->getNumberOfIndicies() > 0) {
//            meshGL->createElementBuffer(meshRAM->getIndicies(0), meshRAM->getIndexAttributesInfo(0));
//            LGL_ERROR;
//        }
//        return meshGL;
//    }
//    return NULL;
//}
//void MeshRAM2GLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
//    const MeshRAM* meshSrc = dynamic_cast<const MeshRAM*>(source);
//    MeshGL* meshDst = dynamic_cast<MeshGL*>(destination);
//    if(meshSrc && meshDst) {
//        // FIXME: Implement update operation
//
//    }
//
//}

} // namespace

