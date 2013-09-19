#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/io/imageloader.h>

namespace inviwo {

ImageRAM::ImageRAM(uvec2 dimension, ImageType type, DataFormatBase format)
    : ImageRepresentation(dimension, type, format)
{
    ImageRAM::initialize();

}

ImageRAM::~ImageRAM() {
    deinitialize();
}  

void ImageRAM::initialize() {
    data_ = NULL;
}

void ImageRAM::deinitialize() {
    // Make sure that data is deinitialized in
    // child class (should not delete void pointer 
    // since destructor will not be called for object.
}

void ImageRAM::resize(uvec2 dimensions) {
    dimensions_ = dimensions;
    //Delete and reallocate data_ to new size
    ImageRAM::deinitialize();
    initialize();
}

bool ImageRAM::copyAndResizeImage(DataRepresentation* targetImageRam) {
    ImageRAM* source = this;
    ImageRAM* target = dynamic_cast<ImageRAM*>(targetImageRam);
    ivwAssert(target!=0, "Target representation missing.");

    //CPU image rescaling using image loader
    uvec2 targetDimensions  = target->getDimensions();
    void* rawData = ImageLoader::rescaleImageRAM(source, targetDimensions.x, targetDimensions.y);

    if (!rawData) return false;

    target->setData(rawData);

    return true;
}

ImageRAM* createImageRAM(const uvec2& dimension, ImageType type, const DataFormatBase& format) {
    // TODO: Add more formats
    switch (format.getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createImageRAM", "Invalid format");
        return NULL;
    case FLOAT16:
        return new ImageRAMfloat16(dimension, type); break;
    case FLOAT32:
        return new ImageRAMfloat32(dimension, type); break;
    case FLOAT64:
        return new ImageRAMfloat64(dimension, type); break;
    case INT8:
        return new ImageRAMint8(dimension, type); break;
    case INT12:
        return new ImageRAMint12(dimension, type); break;
    case INT16:
        return new ImageRAMint16(dimension, type); break;
    case INT32:
        return new ImageRAMint32(dimension, type); break;
    case UINT8:
        return new ImageRAMuint8(dimension, type); break;
    case UINT12:
        return new ImageRAMuint12(dimension, type); break;
    case UINT16:
        return new ImageRAMuint16(dimension, type); break;
    case Vec4FLOAT32:
        return new ImageRAMVec4float32(dimension, type); break;
    case Vec4UINT8:
        return new ImageRAMVec4uint8(dimension, type); break;
    default:
        LogErrorCustom("createImageRAM", "Invalid format or not implemented");
        return NULL;
    }
    return NULL;
}

} // namespace
