#include <modules/opencl/volumeclconverter.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

    VolumeRAM2CLConverter::VolumeRAM2CLConverter()
        : RepresentationConverterType<VolumeCL>()
    {}

    DataRepresentation* VolumeRAM2CLConverter::createFrom(const DataRepresentation* source) {     
        DataRepresentation* destination = 0;
        const VolumeRAM* volumeRAM = static_cast<const VolumeRAM*>(source);
        uvec3 dimension = volumeRAM->getDimensions();
        const void* data = volumeRAM->getData();
        destination = new VolumeCL(dimension, volumeRAM->getDataFormat(), data);

        return destination;
    }
    void VolumeRAM2CLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
        const VolumeRAM* volumeSrc = static_cast<const VolumeRAM*>(source);
        VolumeCL* volumeDst = static_cast<VolumeCL*>(destination);
        if(volumeSrc->getDimensions() != volumeDst->getDimensions()) {
            volumeDst->setDimensions(volumeSrc->getDimensions());
        }
        volumeDst->upload(volumeSrc->getData());


    }

    VolumeCL2RAMConverter::VolumeCL2RAMConverter()
        : RepresentationConverterType<VolumeRAM>()
    {}


    DataRepresentation* VolumeCL2RAMConverter::createFrom(const DataRepresentation* source) {     
        DataRepresentation* destination = 0;
        const VolumeCL* volumeCL = static_cast<const VolumeCL*>(source);
        uvec3 dimension = volumeCL->getDimensions();
        destination = createVolumeRAM(dimension, volumeCL->getDataFormat());

        if (destination) {
            VolumeRAM* volumeRAM = static_cast<VolumeRAM*>(destination);
            volumeCL->download(volumeRAM->getData());
            //const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
            //queue.enqueueReadImage(volumeCL->getVolume(), true, glm::svec3(0), glm::svec3(dimension), 0, 0, volumeRAM->getData());
        }      
        return destination;
    }
    void VolumeCL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
        const VolumeCL* volumeSrc = static_cast<const VolumeCL*>(source);
        VolumeRAM* volumeDst = static_cast<VolumeRAM*>(destination);
        if(volumeSrc->getDimensions() != volumeDst->getDimensions()) {
            volumeDst->setDimensions(volumeSrc->getDimensions());
        }
        volumeSrc->download(volumeDst->getData());

    }

} // namespace
