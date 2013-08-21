#include <inviwo/core/datastructures/image/imageramconverter.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>

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
            case Vec4UINT8:
                return new ImageRAMVec4uint8(static_cast<DataVec4UINT8::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case FLOAT16:
                return new ImageRAMfloat16(static_cast<DataFLOAT16::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case FLOAT32:
                return new ImageRAMfloat32(static_cast<DataFLOAT32::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case FLOAT64:
                return new ImageRAMfloat64(static_cast<DataFLOAT64::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case INT8:
                return new ImageRAMint8(static_cast<DataINT8::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case INT16:
                return new ImageRAMint16(static_cast<DataINT16::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case INT32:
                return new ImageRAMint32(static_cast<DataINT32::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case UINT8:
                return new ImageRAMuint8(static_cast<DataUINT8::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case UINT16:
                return new ImageRAMuint16(static_cast<DataUINT16::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case UINT32:
                return new ImageRAMuint32(static_cast<DataUINT32::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            case UINT64:
                return new ImageRAMuint64(static_cast<DataUINT64::type*>(imageDisk->loadFileData()), imageDisk->getDimensions(), imageDisk->getImageType());
            default: 
                LogError("Cannot convert format from disk to RAM:" << imageDisk->getDataFormat().getString());
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
