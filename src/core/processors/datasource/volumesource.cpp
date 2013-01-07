#include "inviwo/core/processors/datasource/volumesource.h"
#include "inviwo/core/datastructures/volumedisk.h"

namespace inviwo {

VolumeSource::VolumeSource()
    : Processor(),
      volumePort_(Port::OUTPORT, "volume"),
      volumeFileName_("volumeFileName", "Volume file name", IVW_DIR+"data/volumes/hydrogenatom.raw")
{
    addPort(volumePort_);
    addProperty(volumeFileName_);
}

VolumeSource::~VolumeSource() {}

Processor* VolumeSource::create() const {
    return new VolumeSource();
}

void VolumeSource::initialize() {
    Processor::initialize();
    Volume* volume = volumePort_.getData();
    if (volume)
        volume->addRepresentation(new VolumeDisk(volumeFileName_.get()) );
}

void VolumeSource::deinitialize() {
    Processor::deinitialize();
}

void VolumeSource::process() {
    //TODO: if volume has changed setData
}

} // namespace
