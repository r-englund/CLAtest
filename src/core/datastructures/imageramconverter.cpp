#include <inviwo/core/datastructures/imageramconverter.h>
#include <inviwo/core/datastructures/imagerepresentation.h>

namespace inviwo {

    ImageDisk2RAMConverter::ImageDisk2RAMConverter()
        : RepresentationConverterType<ImageRAM>()
    {}

    ImageDisk2RAMConverter::~ImageDisk2RAMConverter() {}

    DataRepresentation* ImageDisk2RAMConverter::convert(DataRepresentation* source) {
        ImageDisk* imageDisk = dynamic_cast<ImageDisk*>(source);
        if(imageDisk){
            return new ImageRAM(imageDisk->loadFileData(), imageDisk->getDimension());
        }

        return NULL;
    }

} // namespace
