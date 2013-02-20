#include <modules/opencl/imageclconverter.h>
#include <inviwo/core/datastructures/imagerepresentation.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

    ImageRAM2CLConverter::ImageRAM2CLConverter()
        : RepresentationConverterType<ImageCL>()
    {}

    DataRepresentation* ImageRAM2CLConverter::convert(DataRepresentation* source) {     
        DataRepresentation* destination = 0;
        ImageRAM* imageRAM = dynamic_cast<ImageRAM*>(source);
        if (imageRAM) {
            uvec2 dimension = imageRAM->getDimension();;
            void* data = imageRAM->getData();
            destination = new ImageCLuint8(static_cast<uint8_t*>(data), dimension);
            // TODO: Implement conversion
            //if (dynamic_cast<ImageRAMuint8*>(imageRAM)) {
            //    destination = new ImageCLuint8(static_cast<uint8_t*>(data), dimension);
            //} else if (dynamic_cast<ImageRAMuint16*>(imageRAM)) {
            //    destination = new ImageCLuint16(static_cast<uint16_t*>(data), dimension);
            //} else if (dynamic_cast<ImageRAMuint32*>(imageRAM)) {
            //    destination = new ImageCLuint32(static_cast<uint32_t*>(data), dimension);
            //} else if (dynamic_cast<ImageRAMint8*>(imageRAM)) {
            //    destination = new ImageCLint8(static_cast<int8_t*>(data), dimension);
            //} else if (dynamic_cast<ImageRAMint16*>(imageRAM)) {
            //    destination = new ImageCLint16(static_cast<int16_t*>(data), dimension);
            //} else if (dynamic_cast<ImageRAMint32*>(imageRAM)) {
            //    destination = new ImageCLint32(static_cast<int32_t*>(data), dimension);
            //} else if (dynamic_cast<ImageRAMfloat*>(imageRAM)) {
            //    destination = new ImageCLfloat(static_cast<float*>(data), dimension);
            //}
        }        
        return destination;
    }

    ImageCL2RAMConverter::ImageCL2RAMConverter()
        : RepresentationConverterType<ImageRAM>()
    {}


    DataRepresentation* ImageCL2RAMConverter::convert(DataRepresentation* source) {     
        DataRepresentation* destination = 0;
        ImageCL* imageCL = dynamic_cast<ImageCL*>(source);
        if (imageCL) {
            uvec2 dimension = imageCL->dimension();
            LogErrorS("OpenCL", "Conversion not implemented yet");
            // TODO: Implement conversion
            //if (dynamic_cast<ImageRAMuint8*>(imageCL)) {
            //    destination = new ImageRAMuint8(dimension);
            //} else if (dynamic_cast<ImageRAMuint16*>(imageCL)) {
            //    destination = new ImageRAMuint16(dimension);
            //} else if (dynamic_cast<ImageRAMuint32*>(imageCL)) {
            //    destination = new ImageRAMuint32(dimension);
            //} else if (dynamic_cast<ImageRAMint8*>(imageCL)) {
            //    destination = new ImageRAMint8(dimension);
            //} else if (dynamic_cast<ImageRAMint16*>(imageCL)) {
            //    destination = new ImageRAMint16(dimension);
            //} else if (dynamic_cast<ImageRAMint32*>(imageCL)) {
            //    destination = new ImageRAMint32(dimension);
            //} else if (dynamic_cast<ImageRAMfloat*>(imageCL)) {
            //    destination = new ImageRAMfloat(dimension);
            //}
            if (destination) {
                ImageRAM* imageRAM = static_cast<ImageRAM*>(destination);
                const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
                queue.enqueueReadImage(*imageCL->getImage(), true, glm::uvec3(0), glm::uvec3(dimension, 1), 0, 0, imageRAM->getData());
            }
        }        
        return destination;
    }

} // namespace
