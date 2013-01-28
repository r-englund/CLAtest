#include <modules/opencl/imagecl.h>
#include <modules/opencl/imageclresizer.h>
#include <inviwo/core/util/assertion.h>
namespace inviwo {

ImageCL::ImageCL()
    : ImageRepresentation(ivec2(128,128)), image2D_(0) //TODO: use actual value
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
    if (dimensions == dimensions_) {
        return;
    }
    cl::Image2D* resizedImage2D = new cl::Image2D(OpenCL::getInstance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions.x, dimensions.y);
    ImageCLResizer::resize(*image2D_, *resizedImage2D, dimensions);
    delete image2D_;
    image2D_ = resizedImage2D;

}

void ImageCL::copyAndResizeImage(DataRepresentation* target) {
    ImageCL* targetCL = dynamic_cast<ImageCL*>(target);

    if (!targetCL) return;

    ImageCLResizer::resize(*image2D_, *(targetCL->getImage()), targetCL->dimension());

}

} // namespace
