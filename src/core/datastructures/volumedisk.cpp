#include "inviwo/core/datastructures/volumedisk.h"

namespace inviwo {

    VolumeDisk::VolumeDisk(std::string url)
        : VolumeRepresentation(ivec3(128,128,128), "UINT8")
    {
        url_ = url;
        initialize();
    }

    VolumeDisk::~VolumeDisk() {}

    void VolumeDisk::initialize() {
        // From url
        //1. read file name 
        //2. read format
        //3. read dimension

        fileName_ = url_;
        dataFormat_ = "UINT8";
        dimensions_ = ivec3(128,128,128);        
    }

    void VolumeDisk::deinitialize() {

    }

    DataRepresentation* VolumeDisk::clone() {
        return new VolumeDisk(url_);
    }



} // namespace
