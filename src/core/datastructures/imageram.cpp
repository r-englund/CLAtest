#include <inviwo/core/datastructures/imageram.h>

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

ImageRAM* createImage(const uvec2& dimension, const DataFormatBase& format) {
    ImageRAM* result = 0;
    // TODO: Add more formats
    switch (format.getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createImage", "Invalid format");
    case FLOAT16:
        result = new ImageRAMfloat16(dimension); break;
    case FLOAT32:
        result = new ImageRAMfloat32(dimension); break;
    case FLOAT64:
        result = new ImageRAMfloat64(dimension); break;
    case INT8:
        result = new ImageRAMint8(dimension); break;
    case INT12:
        LogErrorCustom("createImage", "Invalid format or not implemented"); break;
    case INT16:
        result = new ImageRAMint16(dimension); break;
    case INT32:
        result = new ImageRAMint32(dimension); break;
    case UINT8:
        result = new ImageRAMuint8(dimension); break;
    case UINT12:
        LogErrorCustom("createImage", "Invalid format or not implemented"); break;
    case UINT16:
        result = new ImageRAMuint16(dimension); break;
    case Vec4FLOAT32:
        result = new ImageRAMVec4float32(dimension); break;
    case Vec4UINT8:
        result = new ImageRAMVec4uint8(dimension); break;
    default:
        LogErrorCustom("createImage", "Invalid format or not implemented");
        break;
    }
    return result;
}

} // namespace
