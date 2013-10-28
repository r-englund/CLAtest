#include <modules/opencl/imageclconverter.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

ImageRAM2CLConverter::ImageRAM2CLConverter()
    : RepresentationConverterType<ImageCL>()
{}

DataRepresentation* ImageRAM2CLConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const ImageRAM* imageRAM = static_cast<const ImageRAM*>(source);
    uvec2 dimension = imageRAM->getDimensions();;
    const void* data = imageRAM->getData();
    destination = new ImageCL(dimension, imageRAM->getImageType(), imageRAM->getDataFormat(), data);        
   
    return destination;
}
void ImageRAM2CLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const ImageRAM* imageSrc = static_cast<const ImageRAM*>(source);
    ImageCL* imageDst = static_cast<ImageCL*>(destination);
    if(imageSrc->getDimensions() != imageDst->getDimensions()) {
        imageDst->resize(imageSrc->getDimensions());
    }
    imageDst->upload(imageSrc->getData());
}

ImageCL2RAMConverter::ImageCL2RAMConverter()
    : RepresentationConverterType<ImageRAM>()
{}


DataRepresentation* ImageCL2RAMConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const ImageCL* imageCL = static_cast<const ImageCL*>(source);
    uvec2 dimension = imageCL->getDimensions();
    destination = createImageRAM(dimension, imageCL->getImageType(), imageCL->getDataFormat()); 
    
    if (destination) {
        ImageRAM* imageRAM = static_cast<ImageRAM*>(destination);
        imageCL->download(imageRAM->getData());
        //const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
        //queue.enqueueReadImage(imageCL->getImage(), true, glm::svec3(0), glm::svec3(dimension, 1), 0, 0, imageRAM->getData());
    } else {
        LogError("Invalid conversion or not implemented");
    }
    return destination;
}

void ImageCL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const ImageCL* imageSrc = static_cast<const ImageCL*>(source);
    ImageRAM* imageDst = static_cast<ImageRAM*>(destination);
    if(imageSrc->getDimensions() != imageDst->getDimensions()) {
        imageDst->resize(imageSrc->getDimensions());
    }
    imageSrc->download(imageDst->getData());

}

} // namespace
