#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imageramprecision.h>
#include <inviwo/core/io/imageloader.h>

namespace inviwo {

ImageRAM::ImageRAM(uvec2 dimension, ImageType type, const DataFormatBase* format)
    : ImageRepresentation(dimension, type, format)
{
    ImageRAM::initialize();

}

ImageRAM::~ImageRAM() {
    deinitialize();
}  

void ImageRAM::initialize() {
    data_ = NULL;
    pickingData_ = NULL;
}

void ImageRAM::deinitialize() {
    // Make sure that data is deinitialized in
    // child class (should not delete void pointer 
    // since destructor will not be called for object).
}

void ImageRAM::resize(uvec2 dimensions) {
    dimensions_ = dimensions;
    //Delete and reallocate data_ to new size
    ImageRAM::deinitialize();
    initialize();
}

void* ImageRAM::getPickingData() {
    if(!pickingData_)
        allocatePickingData();

    return pickingData_;
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

ImageRAM* createImageRAM(const uvec2& dimension, ImageType type, const DataFormatBase* format) {
    // TODO: Add more formats
    switch (format->getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createImageRAM", "Invalid format");
        return NULL;
    #define DataFormatIdMacro(i) case i: return new ImageRAMCustomPrecision<Data##i::type, Data##i::bits>(dimension, type); break;
    #include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("createImageRAM", "Invalid format or not implemented");
        return NULL;
    }
    return NULL;
}

} // namespace
