#include "volumesource.h"
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/resources/templateresource.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/io/datareaderfactory.h>

namespace inviwo {

ProcessorClassName(VolumeSource, "VolumeSource"); 
ProcessorCategory(VolumeSource, "Data Input");
ProcessorCodeState(VolumeSource, CODE_STATE_EXPERIMENTAL);

VolumeSource::VolumeSource()
    : Processor(),
      volumePort_("volume"),
	  volumeFile_("volumeFileName", "Volume file")
{
    addPort(volumePort_);

	volumeFile_.onChange(this, &VolumeSource::loadVolume);
    
    std::vector<FileExtension> ext = DataReaderFactory::getRef().getExtensionsForType<Volume>();
    for(std::vector<FileExtension>::const_iterator it = ext.begin();
        it != ext.end(); ++it){
            std::stringstream ss;
            ss << it->description_ << " (*." << it->extension_ << ")";
            volumeFile_.addNameFilter(ss.str());
    }

	addProperty(volumeFile_);
}

VolumeSource::~VolumeSource() {}

void VolumeSource::loadVolume() {
    TemplateResource<Volume>* volumeResource = ResourceManager::instance()->getResourceAs<TemplateResource<Volume> >(volumeFile_.get());
    if (volumeResource) {
        volumePort_.setData(volumeResource->getData(), false);
    } else {
        std::string fileExtension = URLParser::getFileExtension(volumeFile_.get());
        DataReaderType<Volume>* reader = DataReaderFactory::getRef().getReaderForTypeAndExtension<Volume>(fileExtension);
        if(reader){
            Volume* volume = reader->readMetaData(volumeFile_.get());
            ResourceManager::instance()->addResource(new TemplateResource<Volume>(volumeFile_.get(), volume));
            volumePort_.setData(volume, false);
        }else{
            LogInfo("Could not load volume: " << volumeFile_.get());
        }
    }      
}

} // namespace
