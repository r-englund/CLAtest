#include <modules/opencl/volumeclconverter.h>
#include <inviwo/core/datastructures/volumerepresentation.h>
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

            if (dynamic_cast<VolumeRAMuint8*>(volumeRAM)) {
                destination = new VolumeCLuint8(static_cast<uint8_t*>(data), dimension);
            } else if (dynamic_cast<VolumeRAMuint16*>(volumeRAM)) {
                destination = new VolumeCLuint16(static_cast<uint16_t*>(data), dimension);
            } else if (dynamic_cast<VolumeRAMuint32*>(volumeRAM)) {
                destination = new VolumeCLuint32(static_cast<uint32_t*>(data), dimension);
            } else if (dynamic_cast<VolumeRAMint8*>(volumeRAM)) {
                destination = new VolumeCLint8(static_cast<int8_t*>(data), dimension);
            } else if (dynamic_cast<VolumeRAMint16*>(volumeRAM)) {
                destination = new VolumeCLint16(static_cast<int16_t*>(data), dimension);
            } else if (dynamic_cast<VolumeRAMint32*>(volumeRAM)) {
                destination = new VolumeCLint32(static_cast<int32_t*>(data), dimension);
            } else if (dynamic_cast<VolumeRAMfloat32*>(volumeRAM)) {
                destination = new VolumeCLfloat(static_cast<float*>(data), dimension);
            }
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

            if (dynamic_cast<VolumeRAMuint8*>(volumeCL)) {
                destination = new VolumeRAMuint8(dimension);
            } else if (dynamic_cast<VolumeRAMuint16*>(volumeCL)) {
                destination = new VolumeRAMuint16(dimension);
            } else if (dynamic_cast<VolumeRAMuint32*>(volumeCL)) {
                destination = new VolumeRAMuint32(dimension);
            } else if (dynamic_cast<VolumeRAMint8*>(volumeCL)) {
                destination = new VolumeRAMint8(dimension);
            } else if (dynamic_cast<VolumeRAMint16*>(volumeCL)) {
                destination = new VolumeRAMint16(dimension);
            } else if (dynamic_cast<VolumeRAMint32*>(volumeCL)) {
                destination = new VolumeRAMint32(dimension);
            } else if (dynamic_cast<VolumeRAMfloat32*>(volumeCL)) {
                destination = new VolumeRAMfloat32(dimension);
            }
            if (destination) {
                VolumeRAM* volumeRAM = static_cast<VolumeRAM*>(destination);
                const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
                queue.enqueueReadImage(*volumeCL->getVolume(), true, glm::svec3(0), glm::svec3(dimension), 0, 0, volumeRAM->getData());
            }
        }        
        return destination;
    }

} // namespace
