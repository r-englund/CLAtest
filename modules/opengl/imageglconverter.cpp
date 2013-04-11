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

    ImageGL2RAMConverter::ImageGL2RAMConverter()
        : RepresentationConverterType<ImageRAM>()
    {}

    ImageGL2RAMConverter::~ImageGL2RAMConverter() {}

    DataRepresentation* ImageGL2RAMConverter::convert(DataRepresentation* source) {
        ImageGL* imageRepresentation = dynamic_cast<ImageGL*>(source);
        unsigned int nChannels = imageRepresentation->getColorTexture()->getNChannels();
        uvec2 dim = imageRepresentation->getDimension();
        ImageRAM* image = NULL;
        switch (imageRepresentation->getColorTexture()->getDataType()) {
            case GL_UNSIGNED_BYTE: {
                switch (nChannels) {
                    case 1:
                        image = new ImageRAMPrecision<uint8_t>(dim); break;
                    case 2:
                        image = new ImageRAMPrecision< glm::detail::tvec2<uint8_t> >(dim); break;
                    case 3:
                        image = new ImageRAMPrecision< glm::detail::tvec3<uint8_t> >(dim); break;
                    case 4:
                        image = new ImageRAMPrecision< glm::detail::tvec4<uint8_t> >(dim); break;
                    default:
                        image = NULL;
                    }
                break;
            }
            //    
            //case GL_BYTE:
            //    dataTypeSize = 1;
            //    break;
            //case GL_UNSIGNED_SHORT:
            //case GL_SHORT:
            //case GL_HALF_FLOAT:
            //    dataTypeSize = 2;
            //    break;
            //case GL_UNSIGNED_INT:
            //case GL_INT:
            case GL_FLOAT: {
                switch (nChannels) {
                    case 1:
                        image = new ImageRAMPrecision<float>(dim); break;
                    case 2:
                        image = new ImageRAMPrecision< glm::detail::tvec2<float> >(dim); break;
                    case 3:
                        image = new ImageRAMPrecision< glm::detail::tvec3<float> >(dim); break;
                    case 4:
                        image = new ImageRAMPrecision< glm::detail::tvec4<float> >(dim); break;
                    default:
                        image = NULL;
                }
                break;
            }     
            //    dataTypeSize = 4;
            //    break;
            //default:
            //    dataTypeSize = 0;
            //    LogErrorS("ImageGL2RAMConverter::convert()", "Invalid data type: " << dataTypeSize);
        }
        if (image) {
            imageRepresentation->activateBuffer();
            imageRepresentation->getColorTexture()->download(image->getData());
            imageRepresentation->deactivateBuffer();
            return image;
        } else {
            LogErrorS("ImageGL2RAMConverter::convert()", "Invalid conversion");
        }
        return NULL;
    }

} // namespace
