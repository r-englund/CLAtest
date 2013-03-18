#include <inviwo/core/datastructures/imageramconverter.h>
#include <inviwo/core/datastructures/imagerepresentation.h>

namespace inviwo {

    ImageDisk2RAMConverter::ImageDisk2RAMConverter()
        : RepresentationConverterType<ImageRAM>()
    {}

    ImageDisk2RAMConverter::~ImageDisk2RAMConverter() {}

    /**
    * Converts a ImageDisk representation to a RAM representation. This is done if a Image
    * has a representation of ImageDisk and a ImageRAM representation is required. This is
    * used in data.h.
    *
    * @param source is the input representation that is to be converted.
    * @return the imageRAM representation of the file. Returns NULL if source is not a
    * ImageDisk object.
    **/
    DataRepresentation* ImageDisk2RAMConverter::convert(DataRepresentation* source) {
        ImageDisk* imageDisk = dynamic_cast<ImageDisk*>(source);
        if(imageDisk){
            return new ImageRAM(imageDisk->loadFileData(), imageDisk->getDimension());
        }
        return NULL;
    }

} // namespace
