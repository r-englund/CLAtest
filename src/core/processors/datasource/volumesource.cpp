#include "inviwo/core/processors/datasource/volumesource.h"

namespace inviwo {

    VolumeSource::VolumeSource()
        : Processor(),
          volumePort_(Port::OUTPORT, "volume")
    {
        addPort(volumePort_);
        Volume* volume = new Volume();
        volumePort_.setData(volume);
    }

    void VolumeSource::initialize() {
        Processor::initialize();
    }
    
    void VolumeSource::deinitialize() {
        Processor::deinitialize();
    }

    void VolumeSource::process() {
        std::cout << "VolumeSource::process()" << std::endl;
        //TODO: if volume has changed setData
    }

} // namespace
