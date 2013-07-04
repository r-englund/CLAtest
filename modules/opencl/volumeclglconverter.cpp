#include <modules/opencl/volumeclconverter.h>
#include <modules/opencl/volumeclglconverter.h>
#include <modules/opencl/syncclgl.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

VolumeRAM2CLGLConverter::VolumeRAM2CLGLConverter()
    : RepresentationConverterPackage<VolumeCLGL>()
{
    addConverter(new VolumeRAM2GLConverter());
    addConverter(new VolumeGL2CLGLConverter());
}

VolumeCLGL2RAMConverter::VolumeCLGL2RAMConverter()
    : RepresentationConverterType<VolumeRAM>()
{
}


DataRepresentation* VolumeCLGL2RAMConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const VolumeCLGL* volumeCLGL = dynamic_cast<const VolumeCLGL*>(source);
    if (volumeCLGL) {
        uvec3 dimension = volumeCLGL->getDimensions();
        destination = createVolumeRAM(dimension, volumeCLGL->getDataFormat()); 
        const Texture3D* texture = volumeCLGL->getTexture();
        if (destination) {
            VolumeRAM* volumeRAM = static_cast<VolumeRAM*>(destination);
            texture->download(volumeRAM->getData());
            //const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
            //queue.enqueueReadVolume(volumeCL->getVolume(), true, glm::svec3(0), glm::svec3(dimension, 1), 0, 0, volumeRAM->getData());
        } else {
            LogError("Invalid conversion or not implemented");
        }
    }        
    return destination;
}

void VolumeCLGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeCLGL* volumeSrc = dynamic_cast<const VolumeCLGL*>(source);
    VolumeRAM* volumeDst = dynamic_cast<VolumeRAM*>(destination);
    if(volumeSrc && volumeDst) {
        volumeSrc->getTexture()->download(volumeDst->getData());
    }
}


DataRepresentation* VolumeGL2CLGLConverter::createFrom(const DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    const VolumeGL* volumeGL = dynamic_cast<const VolumeGL*>(source);
    if (volumeGL) {
        const Texture3D* data = volumeGL->getTexture();
        destination = new VolumeCLGL(volumeGL->getDimensions(), volumeGL->getDataFormat(), data);
    }        
    return destination;
}

void VolumeGL2CLGLConverter::update(const DataRepresentation*, DataRepresentation*) {
     // Do nothing since they are sharing data

}


DataRepresentation* VolumeCLGL2CLConverter::createFrom(const DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    const VolumeCLGL* volumeCLGL = dynamic_cast<const VolumeCLGL*>(source);
    if (volumeCLGL) {
        uvec3 dimension = volumeCLGL->getDimensions();;
        destination = new VolumeCL(dimension, volumeCLGL->getDataFormat());
        SyncCLGL glSync;
        volumeCLGL->aquireGLObject(glSync.getGLSyncEvent());
        OpenCL::getInstance()->getQueue().enqueueCopyImage(volumeCLGL->getVolume(), static_cast<VolumeCL*>(destination)->getVolume(), glm::svec3(0), glm::svec3(0), glm::svec3(dimension));
        volumeCLGL->releaseGLObject(glSync.getGLSyncEvent());


    }        
    return destination;
}
void VolumeCLGL2CLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeCLGL* volumeSrc = dynamic_cast<const VolumeCLGL*>(source);
    VolumeCL* volumeDst = dynamic_cast<VolumeCL*>(destination);
    if(volumeSrc && volumeDst) {
        SyncCLGL glSync;
        volumeSrc->aquireGLObject(glSync.getGLSyncEvent());
        OpenCL::getInstance()->getQueue().enqueueCopyImage(volumeSrc->getVolume(), volumeDst->getVolume(), glm::svec3(0), glm::svec3(0), glm::svec3(volumeSrc->getDimensions()));
        volumeSrc->releaseGLObject(glSync.getGLSyncEvent());
    }

}



VolumeCL2CLGLConverter::VolumeCL2CLGLConverter() : RepresentationConverterPackage<VolumeCLGL>()
{
    addConverter(new VolumeCL2RAMConverter());
    addConverter(new VolumeRAM2GLConverter());
    addConverter(new VolumeGL2CLGLConverter());
}

} // namespace
