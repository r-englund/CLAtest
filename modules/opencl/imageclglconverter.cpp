#include <modules/opencl/imageclconverter.h>
#include <modules/opencl/imageclglconverter.h>
#include <modules/opencl/syncclgl.h>
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


DataRepresentation* ImageCLGL2RAMConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const ImageCLGL* imageCLGL = dynamic_cast<const ImageCLGL*>(source);
    if (imageCLGL) {
        uvec2 dimension = imageCLGL->getDimensions();
        destination = createImageRAM(dimension, imageCLGL->getImageType(), imageCLGL->getDataFormat()); 
        const Texture2D* texture = imageCLGL->getTexture();
        if (destination) {
            ImageRAM* imageRAM = static_cast<ImageRAM*>(destination);
            texture->download(imageRAM->getData());
            //const cl::CommandQueue& queue = OpenCL::instance()->getQueue();
            //queue.enqueueReadImage(imageCL->getImage(), true, glm::svec3(0), glm::svec3(dimension, 1), 0, 0, imageRAM->getData());
        } else {
            LogError("Invalid conversion or not implemented");
        }
    }        
    return destination;
}

void ImageCLGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const ImageCLGL* imageSrc = dynamic_cast<const ImageCLGL*>(source);
    ImageRAM* imageDst = dynamic_cast<ImageRAM*>(destination);
    if(imageSrc && imageDst) {
        imageSrc->getTexture()->download(imageDst->getData());
    }
}


DataRepresentation* ImageGL2CLGLConverter::createFrom(const DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    const ImageGL* imageGL = dynamic_cast<const ImageGL*>(source);
    if (imageGL) {
        uvec2 dimension = imageGL->getDimensions();;
        const Texture2D* data = imageGL->getColorTexture();
        destination = new ImageCLGL(dimension, imageGL->getImageType(), imageGL->getDataFormat(), data);
    }        
    return destination;
}

void ImageGL2CLGLConverter::update(const DataRepresentation*, DataRepresentation*) {
     // Do nothing since they are sharing data

}


DataRepresentation* ImageCLGL2CLConverter::createFrom(const DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    const ImageCLGL* imageCLGL = dynamic_cast<const ImageCLGL*>(source);
    if (imageCLGL) {
        uvec2 dimension = imageCLGL->getDimensions();;
        destination = new ImageCL(dimension, imageCLGL->getImageType(), imageCLGL->getDataFormat());
        SyncCLGL glSync;
        imageCLGL->aquireGLObject(glSync.getGLSyncEvent());
        OpenCL::instance()->getQueue().enqueueCopyImage(imageCLGL->getImage(), static_cast<ImageCL*>(destination)->getImage(), glm::svec3(0), glm::svec3(0), glm::svec3(dimension, 1));
        imageCLGL->releaseGLObject(glSync.getGLSyncEvent());


    }        
    return destination;
}
void ImageCLGL2CLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const ImageCLGL* imageSrc = dynamic_cast<const ImageCLGL*>(source);
    ImageCL* imageDst = dynamic_cast<ImageCL*>(destination);
    if(imageSrc && imageDst) {
        SyncCLGL glSync;
        imageSrc->aquireGLObject(glSync.getGLSyncEvent());
        OpenCL::instance()->getQueue().enqueueCopyImage(imageSrc->getImage(), imageDst->getImage(), glm::svec3(0), glm::svec3(0), glm::svec3(imageSrc->getDimensions(), 1));
        imageSrc->releaseGLObject(glSync.getGLSyncEvent());
    }

}



ImageCL2CLGLConverter::ImageCL2CLGLConverter() : RepresentationConverterPackage<ImageCLGL>()
{
    addConverter(new ImageCL2RAMConverter());
    addConverter(new ImageRAM2GLConverter());
    addConverter(new ImageGL2CLGLConverter());
}

} // namespace
