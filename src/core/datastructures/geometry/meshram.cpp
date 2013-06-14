#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

MeshRAM::MeshRAM()
    : GeometryRAM()
{
}

MeshRAM::~MeshRAM() {
    deinitialize();
}

void MeshRAM::initialize() {}

void MeshRAM::deinitialize() {
    for (std::vector<AttributesBase*>::iterator it = attributes_.begin() ; it != attributes_.end(); ++it)
        delete (*it);

    for (std::vector<std::pair<Connectivity, IndexAttributes*>>::iterator it = indices_.begin() ; it != indices_.end(); ++it)
        delete it->second;
}

AttributesBase* MeshRAM::getAttributes(size_t idx) const{
    return attributes_[idx];
}

AttributesBase* MeshRAM::getIndicies(size_t idx) const{
    return indices_[idx].second;
}

GeometryRepresentation::Connectivity MeshRAM::getIndexConnectivity(size_t idx) const{
    return indices_[idx].first;
}

size_t MeshRAM::getNumberOfAttributes() const{
    return attributes_.size();
}

size_t MeshRAM::getNumberOfIndicies() const{
    return indices_.size();
}

} // namespace

