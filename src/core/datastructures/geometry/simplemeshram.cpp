#include <inviwo/core/datastructures/geometry/simplemeshram.h>

namespace inviwo {

SimpleMeshRAM::SimpleMeshRAM(RenderType rt, ConnectivityType ct)
: Mesh(rt, ct)
{
}

SimpleMeshRAM::~SimpleMeshRAM() {
    deinitialize();
}

void SimpleMeshRAM::initialize() {
    vertexPositions_ = new Position3dBuffer();
    addAttribute(vertexPositions_);

    vertexTexCoords_ = new TexCoord3dBuffer();
    addAttribute(vertexTexCoords_);

    vertexColors_ = new ColorBuffer();
    addAttribute(vertexColors_);

    indices_ = new IndexBuffer();
    addIndicies(Mesh::AttributesInfo(), indices_);
}

void SimpleMeshRAM::deinitialize() {}

void SimpleMeshRAM::addVertex(vec3 pos, vec3 texCoord, vec4 color){
    vertexPositions_->getEditableRepresentation<Position3dBufferRAM>()->add(pos);
    vertexTexCoords_->getEditableRepresentation<TexCoord3dBufferRAM>()->add(texCoord);
    vertexColors_->getEditableRepresentation<ColorBufferRAM>()->add(color);
}

void SimpleMeshRAM::addIndex(unsigned int idx){
    indices_->getEditableRepresentation<IndexBufferRAM>()->add(idx);
}

void SimpleMeshRAM::setIndicesInfo(RenderType rt, ConnectivityType ct){
    indexAttributes_[0].first = Mesh::AttributesInfo(rt, ct);
}

const Position3dBuffer* SimpleMeshRAM::getVertexList() const {
	return this->vertexPositions_;
}

const IndexBuffer* SimpleMeshRAM::getIndexList() const {
	return this->indices_;
}

} // namespace

