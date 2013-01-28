#include <inviwo/core/datastructures/volume.h>

namespace inviwo {

    Volume::Volume() : Data3D() {
        representations_.clear();
        metaData_.removeAll();
    }

    Volume::~Volume() {}

    Data* Volume::clone() {
        //Volume* newVolume = new Volume();
        //copyRepresentations(newVolume);
        //return newVolume;
        return 0;
    }

    void Volume::setFormat(std::string format) {
        setMetaData<StringMetaData>("format", format);
    }
    
    std::string Volume::getFormat() {
        return getMetaData<StringMetaData>("format", std::string(""));
    }


} // namespace
