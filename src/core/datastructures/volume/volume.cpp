#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>

namespace inviwo {

Volume::Volume(uvec3 dimensions, DataFormatBase format) : Data3D(dimensions, format) {
    representations_.clear();
    metaData_.removeAll();
}

Volume::Volume(VolumeRepresentation* in) : Data3D(in->getDimensions(), in->getDataFormat()) {
    representations_.clear();
    representations_.push_back(in);
    metaData_.removeAll();
}

Volume::Volume(VolumeRepresentation* in, const Volume* src) : Data3D(in->getDimensions(), in->getDataFormat()) {
    representations_.clear();
    representations_.push_back(in);
    metaData_.removeAll();
    src->copyMetaData(this);
}

Volume::~Volume() {}

Data* Volume::clone() const {
    //Volume* newVolume = new Volume();
    //copyRepresentations(newVolume);
    //return newVolume;
    return 0;
}

void Volume::setOffset(ivec3 offset) {
    setMetaData<IVec3MetaData>("offset", offset);
}

ivec3 Volume::getOffset() const{
    return getMetaData<IVec3MetaData>("offset", ivec3(0,0,0));
}

void Volume::createDefaultRepresentation() const{
    representations_.push_back(new VolumeDisk(getDimension(), getDataFormat()));
}


} // namespace