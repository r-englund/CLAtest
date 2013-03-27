#include "imageglconverter.h"
#include <inviwo/core/datastructures/imagerepresentation.h>

namespace inviwo {

    ImageRAM2GLConverter::ImageRAM2GLConverter()
        : RepresentationConverterType<ImageGL>()
    {}

    ImageRAM2GLConverter::~ImageRAM2GLConverter() {}

    DataRepresentation* ImageRAM2GLConverter::convert(DataRepresentation* source) {
        ImageRAM* imageRepresentation = dynamic_cast<ImageRAM*>(source);
        
        //This creates a texture from the defined input ImageRAM.
        uint8_t *data = static_cast<uint8_t*>(imageRepresentation->getData());
        uvec2 dim = imageRepresentation->getDimension();

        Texture2D* texture = new Texture2D(uvec2(dim.x, dim.y), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_NEAREST);
	    texture->setTexels(data);
	    texture->setWidth(dim.x);
	    texture->setHeight(dim.y);
        texture->upload();

        if (imageRepresentation){       
            return new ImageGL(texture, dim);
        }
        return NULL;
    }

} // namespace
