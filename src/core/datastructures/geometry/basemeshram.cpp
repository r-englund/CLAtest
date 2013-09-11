#include <inviwo/core/datastructures/geometry/basemeshram.h>

namespace inviwo {

BaseMeshRAM::BaseMeshRAM(RenderType rt, ConnectivityType ct)
: MeshRAM(rt, ct)
{
}

BaseMeshRAM::~BaseMeshRAM() {
    deinitialize();
}

void BaseMeshRAM::initialize() {
    vertexPositions_ = new Position3dAttributes();
    addAttribute(vertexPositions_);

    vertexTexCoords_ = new TexCoord3dAttributes();
    addAttribute(vertexTexCoords_);

    vertexColors_ = new ColorAttributes();
    addAttribute(vertexColors_);

    indices_ = new IndexAttributes();
    addIndicies(MeshRAM::AttributesInfo(), indices_);
}

void BaseMeshRAM::deinitialize() {}

void BaseMeshRAM::render() const {}

void BaseMeshRAM::addVertex(glm::vec3 pos, glm::vec3 texCoord, glm::vec4 color){
    vertexPositions_->add(pos);
    vertexTexCoords_->add(texCoord);
    vertexColors_->add(color);
}

void BaseMeshRAM::addIndex(unsigned int idx){
    indices_->add(idx);
}

void BaseMeshRAM::setIndicesInfo(RenderType rt, ConnectivityType ct){
    indexAttributes_[0].first = MeshRAM::AttributesInfo(rt, ct);
}

} // namespace

