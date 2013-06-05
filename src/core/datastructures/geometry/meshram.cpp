#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

MeshRAM::MeshRAM()
    : GeometryRAM()
{}

MeshRAM::~MeshRAM() {
    deinitialize();
}

void MeshRAM::initialize() {}

void MeshRAM::deinitialize() {
    for (std::vector<AttributesBase*>::iterator it = vertexAttributes_.begin() ; it != vertexAttributes_.end(); ++it)
        delete (*it);

    for (std::vector<AttributesBase*>::iterator it = faceAttributes_.begin() ; it != faceAttributes_.end(); ++it)
        delete (*it);
}

unsigned int MeshRAM::addVertex(const glm::vec3& pos, const glm::vec3& texCoord, const glm::vec3& color){
    return 0;
}

} // namespace

