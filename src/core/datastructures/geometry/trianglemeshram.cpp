#include <inviwo/core/datastructures/geometry/trianglemeshram.h>

namespace inviwo {

TriangleMeshRAM::TriangleMeshRAM(Connectivity ct)
    : MeshRAM()
{
    indexConnectivity_ = ct;
}

TriangleMeshRAM::~TriangleMeshRAM() {
    deinitialize();
}

void TriangleMeshRAM::initialize() {
    vertexPositions_ = new PositionAttributes();
    attributes_.push_back(vertexPositions_);

    vertexTexCoords_ = new TexCoordAttributes();
    attributes_.push_back(vertexTexCoords_);

    vertexColors_ = new ColorAttributes();
    attributes_.push_back(vertexColors_);

    faceIndices_ = new IndexAttributes();
    indices_.push_back(std::make_pair(indexConnectivity_, faceIndices_));
}

void TriangleMeshRAM::deinitialize() {}

void TriangleMeshRAM::render() const {}

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

