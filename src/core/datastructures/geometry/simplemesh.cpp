#include <inviwo/core/datastructures/geometry/simplemesh.h>

namespace inviwo {

SimpleMesh::SimpleMesh(RenderType rt, ConnectivityType ct)
: Mesh(rt, ct)
{
}

SimpleMesh::~SimpleMesh() {
    deinitialize();
}

void SimpleMesh::initialize() {
    vertexPositions_ = new Position3dBuffer();
    addAttribute(vertexPositions_);

    vertexTexCoords_ = new TexCoord3dBuffer();
    addAttribute(vertexTexCoords_);

    vertexColors_ = new ColorBuffer();
    addAttribute(vertexColors_);

    indices_ = new IndexBuffer();
    addIndicies(Mesh::AttributesInfo(), indices_);
}

void SimpleMesh::deinitialize() {}

void SimpleMesh::addVertex(vec3 pos, vec3 texCoord, vec4 color){
    vertexPositions_->getEditableRepresentation<Position3dBufferRAM>()->add(pos);
    vertexTexCoords_->getEditableRepresentation<TexCoord3dBufferRAM>()->add(texCoord);
    vertexColors_->getEditableRepresentation<ColorBufferRAM>()->add(color);
}

void SimpleMesh::addIndex(unsigned int idx){
    indices_->getEditableRepresentation<IndexBufferRAM>()->add(idx);
}

void SimpleMesh::setIndicesInfo(RenderType rt, ConnectivityType ct){
    indexAttributes_[0].first = Mesh::AttributesInfo(rt, ct);
}

const Position3dBuffer* SimpleMesh::getVertexList() const {
	return this->vertexPositions_;
}

const TexCoord3dBuffer* SimpleMesh::getTexCoordList() const {
    return this->vertexTexCoords_;
}

const ColorBuffer* SimpleMesh::getColorList() const {
    return this->vertexColors_;
}

const IndexBuffer* SimpleMesh::getIndexList() const {
	return this->indices_;
}

} // namespace

