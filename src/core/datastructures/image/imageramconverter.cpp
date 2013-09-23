#include <inviwo/core/datastructures/image/imageramconverter.h>
#include <inviwo/core/datastructures/image/imageramprecision.h>

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
DataRepresentation* ImageDisk2RAMConverter::createFrom(const DataRepresentation* source) {
    const ImageDisk* imageDisk = dynamic_cast<const ImageDisk*>(source);
    if (imageDisk){
        switch (imageDisk->getDataFormatId()) {
        #define DataFormatIdMacro(i) case i: return new ImageRAM_##i(static_cast<Data##i::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
        #include <inviwo/core/util/formatsdefinefunc.h>
        default: 
            LogError("Cannot convert format from Disk to RAM:" << imageDisk->getDataFormat()->getString());
        } 
        
    }
    return NULL;
}

void ImageDisk2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const ImageDisk* imageSrc = dynamic_cast<const ImageDisk*>(source);
    ImageRAM* imageDst = dynamic_cast<ImageRAM*>(destination);
    if(imageSrc && imageDst) {
        // FIXME: The file loader should have a function that loads data into a preallocated location.
        if (imageSrc->getDimensions()==imageDst->getDimensions())
            imageDst->setData(imageSrc->loadFileData());
        else {
            //Image disk and ram vary in dimension sometimes              

            //TODO: Avoid this condition. Needs investigation.
            imageDst->setData(imageSrc->loadFileDataAndRescale(imageDst->getDimensions())); 
        }
    }
}

} // namespace
