#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

MeshRAM::MeshRAM()
    : GeometryRAM(), attributesInfo_(AttributesInfo())
{}

MeshRAM::MeshRAM(RenderType rt, ConnectivityType ct)
    : GeometryRAM(), attributesInfo_(AttributesInfo(rt, ct))
{}

MeshRAM::~MeshRAM() {
    deinitialize();
}

void MeshRAM::initialize() {}

void MeshRAM::deinitialize() {
    for (std::vector<AttributesBase*>::iterator it = attributes_.begin() ; it != attributes_.end(); ++it)
        delete (*it);

    for (std::vector<std::pair<AttributesInfo, IndexAttributes*> >::iterator it = indexAttributes_.begin() ; it != indexAttributes_.end(); ++it)
        delete it->second;
}

DataRepresentation* MeshRAM::clone() const {
    MeshRAM* newMeshRAM = new MeshRAM(attributesInfo_.rt, attributesInfo_.ct);

    for(std::vector<AttributesBase*>::const_iterator it = attributes_.begin() ; it != attributes_.end(); ++it)
        newMeshRAM->addAttribute((*it)->clone());

    for(std::vector<std::pair<AttributesInfo, IndexAttributes*> >::const_iterator it = indexAttributes_.begin() ; it != indexAttributes_.end(); ++it)
        newMeshRAM->addIndicies(it->first, static_cast<IndexAttributes*>(it->second->clone()));

    return newMeshRAM;
}

void MeshRAM::addAttribute(AttributesBase* att){
    attributes_.push_back(att);
}

void MeshRAM::addIndicies(AttributesInfo info, IndexAttributes* ind){
    indexAttributes_.push_back(std::make_pair(info, ind));
}

AttributesBase* MeshRAM::getAttributes(size_t idx) const{
    return attributes_[idx];
}

AttributesBase* MeshRAM::getIndicies(size_t idx) const{
    return indexAttributes_[idx].second;
}

MeshRAM::AttributesInfo MeshRAM::getAttributesInfo() const{
    return attributesInfo_;
}

MeshRAM::AttributesInfo MeshRAM::getIndexAttributesInfo(size_t idx) const{
    return indexAttributes_[idx].first;
}

size_t MeshRAM::getNumberOfAttributes() const{
    return attributes_.size();
}

size_t MeshRAM::getNumberOfIndicies() const{
    return indexAttributes_.size();
}

} // namespace

