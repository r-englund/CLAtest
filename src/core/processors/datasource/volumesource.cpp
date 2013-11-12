#include <inviwo/core/processors/datasource/volumesource.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/resources/templateresource.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
namespace inviwo {

ProcessorClassName(VolumeSource, "VolumeSource"); 
ProcessorCategory(VolumeSource, "Data Input");
ProcessorCodeState(VolumeSource, CODE_STATE_EXPERIMENTAL);

VolumeSource::VolumeSource()
    : Processor(),
      volumePort_("volume"),
	  //FIXME: rename identifier to volumeFile (needs workspace conversion first)
      volumeFile_("volumeFileName", "Volume file")
{
    addPort(volumePort_);

	volumeFile_.onChange(this, &VolumeSource::loadVolume);
	addProperty(volumeFile_);
}

VolumeSource::~VolumeSource() {}

void VolumeSource::loadVolume() {
    // TODO: We need to remove resources at some point, how?
    TemplateResource<Volume>* volumeResource = ResourceManager::instance()->getResourceAs<TemplateResource<Volume> >(volumeFile_.get());
    if (volumeResource) {
        volumePort_.setData(volumeResource->getData(), false);
    } else {
        Volume* volume = new UniformRectiLinearVolume();
        VolumeDisk* vd = new VolumeDisk(volumeFile_.get());
        volume->setDimension(vd->getDimensions());
        volume->addRepresentation(vd);
        ResourceManager::instance()->addResource(new TemplateResource<Volume>(volumeFile_.get(), volume));
        volumePort_.setData(volume, false);
    }      
}

} // namespace
