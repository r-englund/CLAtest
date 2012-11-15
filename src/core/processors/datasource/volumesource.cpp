#include "inviwo/core/processors/datasource/volumesource.h"

namespace inviwo {

VolumeSource::VolumeSource()
    : Processor(),
      volumePort_(Port::OUTPORT, "volume"),
      volumeFileName_("volumeFileName", "Volume file name", IVW_DIR+"data/volumes/hydrogenatom.raw")
{
    addPort(volumePort_);
    Volume* volume = new Volume();
    volumePort_.setData(volume);
    addProperty(volumeFileName_);
}

VolumeSource::~VolumeSource() {}

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
