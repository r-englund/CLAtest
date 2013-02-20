#include "imageglconverter.h"
#include <inviwo/core/datastructures/imagerepresentation.h>

namespace inviwo {

    ImageRAM2GLConverter::ImageRAM2GLConverter()
        : RepresentationConverterType<ImageGL>()
    {}

    ImageRAM2GLConverter::~ImageRAM2GLConverter() {}

    DataRepresentation* ImageRAM2GLConverter::convert(DataRepresentation* source) {
        DataRepresentation* destination = 0;
        ImageRepresentation* imageRepresentation = dynamic_cast<ImageRepresentation*>(source);
        if (imageRepresentation)
            destination = new ImageGL(imageRepresentation->getDimension());
        return destination;
    }

} // namespace
