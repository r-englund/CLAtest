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
    attributes_.push_back(vertexPositions_);

    TexCoordAttributes* vertexTexCoords_ = new TexCoordAttributes();
    attributes_.push_back(vertexTexCoords_);

    ColorAttributes* vertexColors_ = new ColorAttributes();
    attributes_.push_back(vertexColors_);

    indices_ = new IndexAttributes();
}

void TriangleMeshRAM::deinitialize() {}

void TriangleMeshRAM::render(){}

unsigned int TriangleMeshRAM::addVertex(glm::vec3 pos, glm::vec3 texCoord, glm::vec4 color){
    vertexPositions_->add(pos);
    vertexTexCoords_->add(texCoord);
    vertexColors_->add(color);
    return vertexPositions_->getNumberOfAttributes()-1;
}

unsigned int TriangleMeshRAM::addIndex(unsigned int idx){
    indices_->add(idx);
    return indices_->getNumberOfAttributes()-1;
}

} // namespace

