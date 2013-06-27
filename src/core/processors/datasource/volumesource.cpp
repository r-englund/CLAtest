#include <inviwo/core/processors/datasource/volumesource.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>

namespace inviwo {

ProcessorClassName(VolumeSource, "VolumeSource"); 
ProcessorCategory(VolumeSource, "Data Input");
ProcessorCodeState(VolumeSource, CODE_STATE_EXPERIMENTAL);

VolumeSource::VolumeSource()
    : Processor(),
      volumePort_("volume"),
      volumeFileName_("volumeFileName", "Volume file name", IVW_DIR+"data/volumes/hydrogenatom.dat")
{
    addPort(volumePort_);
    addProperty(volumeFileName_);
}

VolumeSource::~VolumeSource() {}

void VolumeSource::initialize() {
    Processor::initialize();
    Volume* volume = volumePort_.getData();
    if (volume){
        VolumeDisk* vd = new VolumeDisk(volumeFileName_.get());
        //Clear all existing representations if volumeDisk representation is to be removed
        volume->clearRepresentations();
        volume->addRepresentation(vd);
        volume->setDimension(vd->getDimensions());
        vd = volume->getEditableRepresentation<VolumeDisk>();
    }
}

void VolumeSource::deinitialize() {
    Processor::deinitialize();
}

void VolumeSource::process() {
    //TODO: if volume has changed setData
    Volume* volume = volumePort_.getData();
    if (volume) {
        VolumeDisk* volumeDisk = volume->getEditableRepresentation<VolumeDisk>();
        if (volumeDisk && (volumeDisk->getSourceFile()!=volumeFileName_.get())) {
            //Clear all existing representations if volumeDisk representation is to be removed
            volume->clearRepresentations();
            VolumeDisk* vd = new VolumeDisk(volumeFileName_.get());
            volume->addRepresentation(vd);
            volume->setDimension(vd->getDimensions());
        }
        volumeDisk = volume->getEditableRepresentation<VolumeDisk>();
    }
}

} // namespace
