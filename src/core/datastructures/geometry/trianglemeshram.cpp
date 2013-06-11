#include <inviwo/core/datastructures/geometry/trianglemeshram.h>

namespace inviwo {

TriangleMeshRAM::TriangleMeshRAM()
    : MeshRAM()
{}

TriangleMeshRAM::~TriangleMeshRAM() {
    deinitialize();
}

void TriangleMeshRAM::initialize() {
    PositionAttributes* vertexPositions_ = new PositionAttributes();
    vertexAttributes_.push_back(vertexPositions_);

    TexCoordAttributes* vertexTexCoords_ = new TexCoordAttributes();
    vertexAttributes_.push_back(vertexTexCoords_);

    ColorAttributes* vertexColors_ = new ColorAttributes();
    vertexAttributes_.push_back(vertexColors_);

    IndexAttributes* faceIndices_ = new IndexAttributes();
    faceAttributes_.push_back(faceIndices_);
}

void TriangleMeshRAM::deinitialize() {}

void TriangleMeshRAM::render(RenderType renderType){

}

unsigned int TriangleMeshRAM::addVertex(glm::vec3 pos, glm::vec3 texCoord, glm::vec4 color){
    vertexPositions_->add(pos);
    vertexTexCoords_->add(texCoord);
    vertexColors_->add(color);
    return vertexPositions_->getNumberOfAttributes()-1;
}

unsigned int TriangleMeshRAM::addIndex(unsigned int idx){
    faceIndices_->add(idx);
    return faceIndices_->getNumberOfAttributes()-1;
}

} // namespace

