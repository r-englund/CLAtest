#include "modules/opencl/imagecl.h"
#include "inviwo/core/util/assertion.h"
namespace inviwo {

ImageCL::ImageCL()
    : image2D_(0), ImageRepresentation(ivec2(128,128)) //TODO: use actual value
{
    initialize();
}

ImageCL::ImageCL(ivec2 dimensions)
    : image2D_(0), ImageRepresentation(dimensions)
{
    initialize();
}

ImageCL::~ImageCL() {}


void ImageCL::resize(ivec2 dimensions)
{
    // TODO: Resizing needs to be performed
    LogErrorS("OpenCL", "resize not implemented yet");
    ivwAssert(0, "not implemented");
}

void ImageCL::copyAndResizeImage(DataRepresentation* target) {
    // TODO: Copy and resizing needs to be performed 
    LogErrorS("OpenCL", "copyAndResizeImage not implemented yet");
    ivwAssert(0, "not implemented");
}

} // namespace
