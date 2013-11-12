#include "imageglconverter.h"
#include <inviwo/core/datastructures/image/imagerepresentation.h>

namespace inviwo {

ImageRAM2GLConverter::ImageRAM2GLConverter()
    : RepresentationConverterType<ImageGL>()
{}

ImageRAM2GLConverter::~ImageRAM2GLConverter() {}

DataRepresentation* ImageRAM2GLConverter::createFrom(const DataRepresentation* source) {
    const ImageRAM* imageRepresentation = static_cast<const ImageRAM*>(source);
    
    //This creates a texture from the defined input ImageRAM.
    const void *data = imageRepresentation->getData();
    uvec2 dim = imageRepresentation->getDimensions();
    Texture2D* texture = new Texture2D(dim, getGLFormats()->getGLFormat(imageRepresentation->getDataFormatId()), GL_LINEAR);
    texture->upload(data);

    if (imageRepresentation){       
        return new ImageGL(dim, imageRepresentation->getImageType(), imageRepresentation->getDataFormat(), texture);
    }
    return NULL;
}
void ImageRAM2GLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const ImageRAM* imageSrc = static_cast<const ImageRAM*>(source);
    ImageGL* imageDst = static_cast<ImageGL*>(destination);
    if(imageSrc->getDimensions() != imageDst->getDimensions()) {
        imageDst->resize(imageSrc->getDimensions());
    }
    // FIXME: OpenGL color should not have both depth and color
    imageDst->getColorTexture()->upload(imageSrc->getData());

}

ImageGL2RAMConverter::ImageGL2RAMConverter()
    : RepresentationConverterType<ImageRAM>()
{}

ImageGL2RAMConverter::~ImageGL2RAMConverter() {}

DataRepresentation* ImageGL2RAMConverter::createFrom(const DataRepresentation* source) {
    const ImageGL* imageGL = static_cast<const ImageGL*>(source);
    ImageRAM* image = createImageRAM(imageGL->getDimensions(), imageGL->getImageType(), imageGL->getDataFormat()); 
    if (image) {
        imageGL->getColorTexture()->download(image->getData());
        imageGL->getPickingTexture()->download(image->getPickingData());
        imageGL->getDepthTexture()->download(image->getDepthData());
        return image;
    } else {
        LogError("Cannot convert format from GL to RAM:" << imageGL->getDataFormat()->getString());
    }
    return NULL;
}

void ImageGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const ImageGL* imageSrc = static_cast<const ImageGL*>(source);
    ImageRAM* imageDst = static_cast<ImageRAM*>(destination);
    if(imageSrc->getDimensions() != imageDst->getDimensions()) {
        imageDst->resize(imageSrc->getDimensions());
    }
    // FIXME: OpenGL color should not have both depth and color
    imageSrc->getColorTexture()->download(imageDst->getData());
    if (imageDst->getPickingData() != NULL && imageDst->getPickingData() != NULL)
        imageSrc->getPickingTexture()->download(imageDst->getPickingData());
    if (imageSrc->getDepthTexture() != NULL && imageDst->getDepthData() != NULL)
        imageSrc->getDepthTexture()->download(imageDst->getDepthData());
}

} // namespace
