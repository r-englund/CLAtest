#include "imageglconverter.h"
#include "inviwo/core/datastructures/imagerepresentation.h"

namespace inviwo {

    ImageGLConverter::ImageGLConverter()
        : RepresentationConverterType<ImageGL>()
    {}

    ImageGLConverter::~ImageGLConverter() {}

    DataRepresentation* ImageGLConverter::convert(DataRepresentation* source) {
        DataRepresentation* destination = 0;
        ImageRepresentation* imageRepresentation = dynamic_cast<ImageRepresentation*>(source);
        if (imageRepresentation)
            destination = new ImageGL(imageRepresentation->size());
        return destination;
    }

} // namespace
