#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/resources/templateresource.h>

#ifndef IVW_VOLUME_TEST_H
#define IVW_VOLUME_TEST_H


TEST(VolumeTest,SimpleLoadVolumeTest){
    EXPECT_TRUE(true);
    
    std::string file = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_VOLUMES) + "/hydrogenatom.dat";

    std::string fileExtension = URLParser::getFileExtension(file);
    DataReaderType<Volume>* reader = DataReaderFactory::getRef().getReaderForTypeAndExtension<Volume>(fileExtension);
    if(reader){
        Volume* volume = reader->readMetaData(file);
        ResourceManager::instance()->addResource(new TemplateResource<Volume>(file, volume));
        const VolumeRAM* volumeRAM = volume->getRepresentation<VolumeRAM>();
    }

    ResourceManager::instance()->clearAllResources();
}







#endif