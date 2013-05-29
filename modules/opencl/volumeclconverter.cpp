#include <modules/opencl/volumeclconverter.h>
#include <inviwo/core/datastructures/volumeramprecision.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

    VolumeRAM2CLConverter::VolumeRAM2CLConverter()
        : RepresentationConverterType<VolumeCL>()
    {}

    DataRepresentation* VolumeRAM2CLConverter::convert(DataRepresentation* source) {     
        DataRepresentation* destination = 0;
        VolumeRAM* volumeRAM = dynamic_cast<VolumeRAM*>(source);
        if (volumeRAM) {
            uvec3 dimension = volumeRAM->getDimensions();
            void* data = volumeRAM->getData();
            destination = new VolumeCL(dimension, volumeRAM->getDataFormat(), data);
        }        
        return destination;
    }

    VolumeCL2RAMConverter::VolumeCL2RAMConverter()
        : RepresentationConverterType<VolumeRAM>()
    {}


    DataRepresentation* VolumeCL2RAMConverter::convert(DataRepresentation* source) {     
        DataRepresentation* destination = 0;
        VolumeCL* volumeCL = dynamic_cast<VolumeCL*>(source);
        if (volumeCL) {
            uvec3 dimension = volumeCL->getDimensions();
            destination = createVolume(dimension, volumeCL->getDataFormat());

            if (destination) {
                VolumeRAM* volumeRAM = static_cast<VolumeRAM*>(destination);
                volumeCL->download(volumeRAM->getData());
                //const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
                //queue.enqueueReadImage(volumeCL->getVolume(), true, glm::svec3(0), glm::svec3(dimension), 0, 0, volumeRAM->getData());
            }
        }        
        return destination;
    }

} // namespace
