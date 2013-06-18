#include <inviwo/core/datastructures/image/imageram.h>

namespace inviwo {

ImageRAM::ImageRAM()
    : ImageRepresentation()
{
    ImageRAM::initialize();
}

ImageRAM::ImageRAM(uvec2 dimension)
    : ImageRepresentation(dimension)
{
    ImageRAM::initialize();
}

ImageRAM::ImageRAM(uvec2 dimension, DataFormatBase format)
    : ImageRepresentation(dimension, format), data_(NULL)
{
    ImageRAM::initialize();

}

ImageRAM::~ImageRAM() {
    deinitialize();
}  

void ImageRAM::initialize() {

}

void ImageRAM::deinitialize() {
    if(data_) {
        delete[] data_;
        data_ = NULL;
    }
}

DataRepresentation* ImageRAM::clone() const {
    return new ImageRAM();
}

void ImageRAM::resize(uvec2 dimensions) {
    dimensions_ = dimensions;
    //Delete and reallocate data_ to new size
    ImageRAM::deinitialize();
    initialize();
}

bool ImageRAM::copyAndResizeImage(DataRepresentation* targetImageRam) {
    IVW_UNUSED_PARAM(targetImageRam);
    return false;
}

ImageRAM* createImage(const uvec2& dimension, const DataFormatBase& format) {
    // TODO: Add more formats
    switch (format.getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createImage", "Invalid format");
        return NULL;
    case FLOAT16:
        return new ImageRAMfloat16(dimension); break;
    case FLOAT32:
        return new ImageRAMfloat32(dimension); break;
    case FLOAT64:
        return new ImageRAMfloat64(dimension); break;
    case INT8:
        return new ImageRAMint8(dimension); break;
    case INT12:
        return new ImageRAMint12(dimension); break;
    case INT16:
        return new ImageRAMint16(dimension); break;
    case INT32:
        return new ImageRAMint32(dimension); break;
    case UINT8:
        return new ImageRAMuint8(dimension); break;
    case UINT12:
        return new ImageRAMuint12(dimension); break;
    case UINT16:
        return new ImageRAMuint16(dimension); break;
    case Vec4FLOAT32:
        return new ImageRAMVec4float32(dimension); break;
    case Vec4UINT8:
        return new ImageRAMVec4uint8(dimension); break;
    default:
        LogErrorCustom("createImage", "Invalid format or not implemented");
        return NULL;
    }
    return NULL;
}

} // namespace
