#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

Mesh::Mesh()
    : Geometry(), attributesInfo_(AttributesInfo())
{}

Mesh::Mesh(RenderType rt, ConnectivityType ct)
    : Geometry(), attributesInfo_(AttributesInfo(rt, ct))
{}

Mesh::~Mesh() {
    deinitialize();
}

void Mesh::initialize() {}

void Mesh::deinitialize() {
    for (std::vector<Buffer*>::iterator it = attributes_.begin() ; it != attributes_.end(); ++it)
        delete (*it);

    for (std::vector<std::pair<AttributesInfo, IndexBuffer*> >::iterator it = indexAttributes_.begin() ; it != indexAttributes_.end(); ++it)
        delete it->second;
}

Data* Mesh::clone() const {
    Mesh* newMeshRAM = new Mesh(attributesInfo_.rt, attributesInfo_.ct);

    for(std::vector<Buffer*>::const_iterator it = attributes_.begin() ; it != attributes_.end(); ++it)
        newMeshRAM->addAttribute(static_cast<Buffer*>((*it)->clone()));

    for(std::vector<std::pair<AttributesInfo, IndexBuffer*> >::const_iterator it = indexAttributes_.begin() ; it != indexAttributes_.end(); ++it)
        newMeshRAM->addIndicies(it->first, static_cast<IndexBuffer*>(it->second->clone()));

    return newMeshRAM;
}

void Mesh::addAttribute(Buffer* att){
    attributes_.push_back(att);
}

void Mesh::addIndicies(AttributesInfo info, IndexBuffer* ind){
    indexAttributes_.push_back(std::make_pair(info, ind));
}

Buffer* Mesh::getAttributes(size_t idx) const{
    return attributes_[idx];
}

Buffer* Mesh::getIndicies(size_t idx) const{
    return indexAttributes_[idx].second;
}

Mesh::AttributesInfo Mesh::getAttributesInfo() const{
    return attributesInfo_;
}

Mesh::AttributesInfo Mesh::getIndexAttributesInfo(size_t idx) const{
    return indexAttributes_[idx].first;
}

size_t Mesh::getNumberOfAttributes() const{
    return attributes_.size();
}

size_t Mesh::getNumberOfIndicies() const{
    return indexAttributes_.size();
}

} // namespace

