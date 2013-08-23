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
      volumeFileName_("volumeFileName", "Volume file name", IVW_DIR+"data/volumes/hydrogenatom.dat")
{
    addPort(volumePort_);
    addProperty(volumeFileName_);
}

VolumeSource::~VolumeSource() {}

void VolumeSource::initialize() {
    Processor::initialize();
    // Load default volume
    loadVolume(volumeFileName_.get());


}

void VolumeSource::deinitialize() {
    Processor::deinitialize();
}

void VolumeSource::process() {
    loadVolume(volumeFileName_.get());
}

void VolumeSource::loadVolume( const std::string& filePath )
{
    // TODO: We need to remove resources at some point, how?
    TemplateResource<Volume>* volumeResource = ResourceManager::instance()->getResourceAs<TemplateResource<Volume>>(filePath);
    if(volumeResource) {
        volumePort_.setData(volumeResource->getData(), false);
    } else {
        Volume* volume = new UniformRectiLinearVolume();
        VolumeDisk* vd = new VolumeDisk(filePath);
        volume->setDimension(vd->getDimensions());
        volume->addRepresentation(vd);
        ResourceManager::instance()->addResource(new TemplateResource<Volume>(filePath, volume));
        volumePort_.setData(volume, false);
    }      
}

} // namespace
