#include <modules/opencl/imageclglconverter.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

ImageRAM2CLGLConverter::ImageRAM2CLGLConverter()
    : RepresentationConverterPackage<ImageCLGL>()
{
    addConverter(new ImageRAM2GLConverter());
    addConverter(new ImageGL2CLGLConverter());
}

ImageCLGL2RAMConverter::ImageCLGL2RAMConverter()
    : RepresentationConverterType<ImageRAM>()
{
}


DataRepresentation* ImageCLGL2RAMConverter::convert(DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    ImageCLGL* imageCL = dynamic_cast<ImageCLGL*>(source);
    if (imageCL) {
        uvec2 dimension = imageCL->dimension();
        destination = createImage(dimension, imageCL->getDataFormat()); 
        const Texture2D* texture = imageCL->getTexture();
        if (destination) {
            ImageRAM* imageRAM = static_cast<ImageRAM*>(destination);
            texture->download(imageRAM->getData());
            //const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
            //queue.enqueueReadImage(imageCL->getImage(), true, glm::svec3(0), glm::svec3(dimension, 1), 0, 0, imageRAM->getData());
        } else {
            LogError("Invalid conversion or not implemented");
        }
    }        
    return destination;
}


DataRepresentation* ImageGL2CLGLConverter::convert( DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    ImageGL* imageGL = dynamic_cast<ImageGL*>(source);
    if (imageGL) {
        uvec2 dimension = imageGL->getDimension();;
        Texture2D* data = imageGL->getColorTexture();
        destination = new ImageCLGL(dimension, imageGL->getDataFormat(), data);
    }        
    return destination;
}


DataRepresentation* ImageCLGL2CLConverter::convert( DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    ImageCLGL* imageCLGL = dynamic_cast<ImageCLGL*>(source);
    if (imageCLGL) {
        uvec2 dimension = imageCLGL->getDimension();;
        const cl::Image2DGL data = imageCLGL->getImage();
        destination = new ImageCL(dimension, imageCLGL->getDataFormat());
        glFinish();
        std::vector<cl::Memory> syncImages(1); syncImages[0] = imageCLGL->getImage()(); 
        OpenCL::getInstance()->getQueue().enqueueAcquireGLObjects(&syncImages);
        OpenCL::getInstance()->getQueue().enqueueCopyImage(data, static_cast<ImageCL*>(destination)->getImage(), glm::svec3(0), glm::svec3(0), glm::svec3(dimension, 1));
        OpenCL::getInstance()->getQueue().enqueueReleaseGLObjects(&syncImages);
        OpenCL::getInstance()->getQueue().finish();


    }        
    return destination;
}

} // namespace
