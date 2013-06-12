#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

MeshRAM::MeshRAM()
    : GeometryRAM()
{
    indices_ = NULL;
}

MeshRAM::~MeshRAM() {
    deinitialize();
}

void MeshRAM::initialize() {}

void MeshRAM::deinitialize() {
    for (std::vector<AttributesBase*>::iterator it = attributes_.begin() ; it != attributes_.end(); ++it)
        delete (*it);

    delete indices_;
}

AttributesBase* MeshRAM::getAttribute(size_t idx) const{
    return attributes_[idx];
}

AttributesBase* MeshRAM::getIndicies() const{
    return indices_;
}

size_t MeshRAM::getNumberOfAttributes() const{
    return attributes_.size();
}

} // namespace

