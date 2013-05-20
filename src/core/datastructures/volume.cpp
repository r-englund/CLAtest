#include <inviwo/core/datastructures/volume.h>
#include <inviwo/core/datastructures/volumedisk.h>

namespace inviwo {

Volume::Volume() : Data3D() {
    representations_.clear();
    metaData_.removeAll();
}

Volume::Volume(VolumeRepresentation* in) : Data3D() {
    representations_.clear();
    representations_.push_back(in);
    metaData_.removeAll();
}

Volume::Volume(VolumeRepresentation* in, const Volume* src) : Data3D() {
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

DataFormatBase Volume::getDataFormat() const{
    DataFormatBase format = DataUINT8();
    if (representations_[0]){
        VolumeRepresentation* volRep = dynamic_cast<VolumeRepresentation*>(representations_[0]);
        if (volRep)
            format = volRep->getDataFormat();
    }
    return format;
}

void Volume::createDefaultRepresentation() const{
    representations_.push_back(new VolumeDisk());
}


} // namespace