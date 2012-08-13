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

    Processor* VolumeSource::create() const {
        return new VolumeSource();
    }

    void VolumeSource::initialize() {
        Processor::initialize();
    }
    
    void VolumeSource::deinitialize() {
        Processor::deinitialize();
    }

    void VolumeSource::process() {
        //TODO: if volume has changed setData
    }

} // namespace
