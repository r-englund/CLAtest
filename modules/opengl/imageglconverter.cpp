#include "imageglconverter.h"
#include "inviwo/core/datastructures/imagerepresentation.h"

namespace inviwo {

    ImageGLConverter::ImageGLConverter()
        : RepresentationConverter()
    {}

    ImageGLConverter::~ImageGLConverter() {}

    void ImageGLConverter::convert(DataRepresentation* source, DataRepresentation* destination) {
        ImageRepresentation* imageRepresentation = dynamic_cast<ImageRepresentation*>(source);
        if (imageRepresentation) {
            destination = new ImageGL(imageRepresentation->size());
            return;
        }
        destination = 0;
    }

} // namespace
