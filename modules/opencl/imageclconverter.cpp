#include <modules/opencl/imageclconverter.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
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
        const void* data = imageRAM->getData();
        destination = new ImageCL(dimension, imageRAM->getDataFormat(), data);
        //// TODO: Add more formats
        //switch (imageRAM->getDataFormatId()) {
        //case UINT8:
        //    return new ImageCLuint8(static_cast<DataUINT8::type*>(imageRAM->getData()), imageRAM->getDimension());
        //case UINT16:
        //    return new ImageCLuint16(static_cast<DataUINT16::type*>(imageRAM->getData()), imageRAM->getDimension());
        //case UINT32:
        //    return new ImageCLuint32(static_cast<DataUINT32::type*>(imageRAM->getData()), imageRAM->getDimension());
        //case INT8:
        //    return new ImageCLint8(static_cast<DataINT8::type*>(imageRAM->getData()), imageRAM->getDimension());
        //case INT16:
        //    return new ImageCLint16(static_cast<DataINT16::type*>(imageRAM->getData()), imageRAM->getDimension());
        //case INT32:
        //    return new ImageCLint32(static_cast<DataINT32::type*>(imageRAM->getData()), imageRAM->getDimension());
        //case FLOAT32:
        //    return new ImageCLfloat32(static_cast<DataFLOAT32::type*>(imageRAM->getData()), imageRAM->getDimension());
        //case Vec4UINT8:
        //    return new ImageCLuint8vec4(static_cast<DataVec4UINT8::type*>(imageRAM->getData()), imageRAM->getDimension());
        //case Vec4FLOAT32:
        //    return new ImageCLvec4float32(static_cast<DataVec4FLOAT32::type*>(imageRAM->getData()), imageRAM->getDimension());
        //}
       
        //LogError("Invalid conversion or not implemented");
        
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
        destination = createImage(dimension, imageCL->getDataFormat()); 
        
        if (destination) {
            ImageRAM* imageRAM = static_cast<ImageRAM*>(destination);
            imageCL->download(imageRAM->getData());
            //const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
            //queue.enqueueReadImage(imageCL->getImage(), true, glm::svec3(0), glm::svec3(dimension, 1), 0, 0, imageRAM->getData());
        } else {
            LogError("Invalid conversion or not implemented");
        }
    }        
    return destination;
}

} // namespace
