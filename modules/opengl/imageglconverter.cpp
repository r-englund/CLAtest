#include "imageglconverter.h"

namespace inviwo {

    ImageGLConverter::ImageGLConverter()
        : RepresentationConverter()
    {}

    ImageGLConverter::~ImageGLConverter() {}

    void ImageGLConverter::convert(DataRepresentation* /*source*/, DataRepresentation* destination) {
        destination = new ImageGL();
    }

} // namespace
