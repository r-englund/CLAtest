#include <inviwo/core/datastructures/geometry/geometrydisk2ramconverter.h>
#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

GeometryDisk2RAMConverter::GeometryDisk2RAMConverter()
    : RepresentationConverterType<GeometryRAM>() {}

GeometryDisk2RAMConverter::~GeometryDisk2RAMConverter() {}

DataRepresentation* GeometryDisk2RAMConverter::createFrom(const DataRepresentation* source) {
    const MeshDisk* meshdisk = dynamic_cast<const MeshDisk*>(source);

    if (meshdisk) {
        return static_cast<MeshRAM*>(meshdisk->readData());
    }

    return NULL;
}

void GeometryDisk2RAMConverter::update(const DataRepresentation* source,
                                       DataRepresentation* destination) {
    const MeshDisk* meshdisk = dynamic_cast<const MeshDisk*>(source);
    MeshRAM* meshram = dynamic_cast<MeshRAM*>(destination);

    if (meshdisk && meshram) {
        meshdisk->readDataInto(meshram);
    }
}

}  // namespace
