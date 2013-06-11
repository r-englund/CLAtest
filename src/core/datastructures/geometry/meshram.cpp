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

AttributesBase* MeshRAM::getVertexAttribute(size_t idx) const{
    return vertexAttributes_[idx];
}

AttributesBase* MeshRAM::getFaceAttribute(size_t idx) const{
    return faceAttributes_[idx];
}

size_t MeshRAM::getNumberOfVertexAttributes() const{
    return vertexAttributes_.size();
}

size_t MeshRAM::getNumberOfFaceAttributes() const{
    return faceAttributes_.size();
}

} // namespace

