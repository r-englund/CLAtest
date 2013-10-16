#include <inviwo/core/datastructures/geometry/simplemeshram.h>

namespace inviwo {

SimpleMeshRAM::SimpleMeshRAM(RenderType rt, ConnectivityType ct)
: MeshRAM(rt, ct)
{
}

SimpleMeshRAM::~SimpleMeshRAM() {
    deinitialize();
}

void SimpleMeshRAM::initialize() {
    vertexPositions_ = new Position3dAttributes();
    addAttribute(vertexPositions_);

    vertexTexCoords_ = new TexCoord3dAttributes();
    addAttribute(vertexTexCoords_);

    vertexColors_ = new ColorAttributes();
    addAttribute(vertexColors_);

    indices_ = new IndexAttributes();
    addIndicies(MeshRAM::AttributesInfo(), indices_);
}

void SimpleMeshRAM::deinitialize() {}

void SimpleMeshRAM::render() const {}

void SimpleMeshRAM::addVertex(vec3 pos, vec3 texCoord, vec4 color){
    vertexPositions_->add(pos);
    vertexTexCoords_->add(texCoord);
    vertexColors_->add(color);
}

void SimpleMeshRAM::addIndex(unsigned int idx){
    indices_->add(idx);
}

void SimpleMeshRAM::setIndicesInfo(RenderType rt, ConnectivityType ct){
    indexAttributes_[0].first = MeshRAM::AttributesInfo(rt, ct);
}

Position3dAttributes* SimpleMeshRAM::getVertexList() {
	return this->vertexPositions_;
}

IndexAttributes* SimpleMeshRAM::getIndexList() {
	return this->indices_;
}

} // namespace

