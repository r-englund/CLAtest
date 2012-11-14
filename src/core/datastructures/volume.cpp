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

} // namespace
