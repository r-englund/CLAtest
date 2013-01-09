#include "inviwo/core/datastructures/volume.h"

namespace inviwo {

    Volume::Volume() : Data() {
        representations_.clear();
    }

    Volume::~Volume() {}

    Data* Volume::clone() {
        Volume* newVolume = new Volume();
        copyRepresentations(newVolume);
        return newVolume;
    }

    void Volume::setDimension(ivec3 dim) {
        setMetaData<IVec3MetaData>("dimension", dim);
    }

    void Volume::setFormat(std::string format) {
        setMetaData<StringMetaData>("format", format);
    }

    ivec3 Volume::getDimension() {
        return getMetaData<IVec3MetaData>("dimension", ivec3(0));
    }

    std::string Volume::getFormat() {
        return getMetaData<StringMetaData>("format", std::string(""));
    }

} // namespace
