#include "volumeexport.h"
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

ProcessorClassName(VolumeExport, "VolumeExport"); 
ProcessorCategory(VolumeExport, "Data Output");
ProcessorCodeState(VolumeExport, CODE_STATE_EXPERIMENTAL);

VolumeExport::VolumeExport()
    : Processor(),
      volumePort_("volume"),
      volumeFileName_("volumeFileName", "Volume file name", IVW_DIR+"data/volumes/newvolume.dat"),
      exportVolumeButton_("snapshot", "Export Volume", PropertyOwner::VALID)
{
    addPort(volumePort_);
    addProperty(volumeFileName_);
    exportVolumeButton_.registerClassMemberFunction(this, &VolumeExport::exportVolume);
    addProperty(exportVolumeButton_);
}

VolumeExport::~VolumeExport() {}

void VolumeExport::initialize() {
    Processor::initialize();    
}

void VolumeExport::deinitialize() {
    Processor::deinitialize();
}

void VolumeExport::exportVolume() {
    //TODO: if volume has changed setData
    const Volume* volume = volumePort_.getData();
    if (volume) {
        const VolumeRAM* volumeRAM = volume->getRepresentation<VolumeRAM>();
        if (volumeRAM && !volumeFileName_.get().empty() ) {
            volumeRAM->saveData(volumeFileName_.get());
        }
    }
}

void VolumeExport::process() {}

} // namespace
