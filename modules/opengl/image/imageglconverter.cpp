#include "imageglconverter.h"
#include <inviwo/core/datastructures/image/imagerepresentation.h>

namespace inviwo {

ImageRAM2GLConverter::ImageRAM2GLConverter()
    : RepresentationConverterType<ImageGL>()
{}

ImageRAM2GLConverter::~ImageRAM2GLConverter() {}

DataRepresentation* ImageRAM2GLConverter::createFrom(const DataRepresentation* source) {
    const ImageRAM* imageRepresentation = dynamic_cast<const ImageRAM*>(source);
    
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
    const ImageRAM* imageSrc = dynamic_cast<const ImageRAM*>(source);
    ImageGL* imageDst = dynamic_cast<ImageGL*>(destination);
    if(imageSrc && imageDst) {
        // FIXME: OpenGL color should not have both depth and color
        imageDst->getColorTexture()->upload(imageSrc->getData());
    }

}

ImageGL2RAMConverter::ImageGL2RAMConverter()
    : RepresentationConverterType<ImageRAM>()
{}

ImageGL2RAMConverter::~ImageGL2RAMConverter() {}

DataRepresentation* ImageGL2RAMConverter::createFrom(const DataRepresentation* source) {
    const ImageGL* imageGL = dynamic_cast<const ImageGL*>(source);
    if(imageGL){
        ImageRAM* image = createImageRAM(imageGL->getDimensions(), imageGL->getImageType(), imageGL->getDataFormat()); 
        if (image) {
            imageGL->getColorTexture()->download(image->getData());
            return image;
        } else {
            LogError("Cannot convert format from GL to RAM:" << imageGL->getDataFormat()->getString());
        }
    }
    else {
        LogError("Source could not be cast to ImageGL");
    }
    return NULL;
}

void ImageGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const ImageGL* imageSrc = dynamic_cast<const ImageGL*>(source);
    ImageRAM* imageDst = dynamic_cast<ImageRAM*>(destination);
    if(imageSrc && imageDst) {
        // FIXME: OpenGL color should not have both depth and color
        if (imageDst->getDimensions()==imageSrc->getDimensions(),"GL and Ram representations are expected to have same dimensions.")
        imageSrc->getColorTexture()->download(imageDst->getData());
    }
}

} // namespace
