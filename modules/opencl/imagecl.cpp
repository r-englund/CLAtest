#include <modules/opencl/imagecl.h>
#include <modules/opencl/imageclresizer.h>
#include <inviwo/core/util/assertion.h>
namespace inviwo {

ImageCL::ImageCL(uvec2 dimensions, ImageType type, const DataFormatBase* format, const void* data)
    : ImageRepresentation(dimensions, type, format), imageFormat_(dataFormatToCLImageFormat(format->getId()))
{
    initialize(data);
}

ImageCL::~ImageCL() { 
    deinitialize(); 
}

void ImageCL::initialize(const void* texels) {

    if (texels != NULL) {
        // Could performance be increased by using pinned memory?
        // 3.1.1 http://www.nvidia.com/content/cudazone/CUDABrowser/downloads/papers/NVIDIA_OpenCL_BestPracticesGuide.pdf
        //cl::Buffer pinnedMem(OpenCL::instance()->getContext(), CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, sizeof(texels), NULL, NULL);
        //unsigned char* mappedMem = (unsigned char*)OpenCL::instance()->getQueue().enqueueMapBuffer(pinnedMem, true, CL_MAP_WRITE, 0, sizeof(texels), 0);
        //memcpy(mappedMem, texels, sizeof(texels));
        //OpenCL::instance()->getQueue().enqueueWriteImage(*image2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, mappedMem);
        //OpenCL::instance()->getQueue().enqueueUnmapMemObject(pinnedMem, mappedMem);

        // This should also use pinned memory...
        image2D_ = new cl::Image2D(OpenCL::instance()->getContext(), 
            CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR, 
            getFormat(), static_cast<size_t>(dimensions_.x), static_cast<size_t>(dimensions_.y), 0, const_cast<void*>(texels));
        // Alternatively first allocate memory on device and then transfer
        //image2D_ = new cl::Image2D(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions_.x, dimensions_.y);
        //OpenCL::instance()->getQueue().enqueueWriteImage(*image2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, texels);
    } else {
        image2D_ = new cl::Image2D(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, getFormat(), static_cast<size_t>(dimensions_.x), static_cast<size_t>(dimensions_.y));
    }
    ImageCL::initialize();
}

DataRepresentation* ImageCL::clone() const {
    ImageCL* newImageCL = new ImageCL(dimensions_, getImageType(), getDataFormat());
    OpenCL::instance()->getQueue().enqueueCopyImage(*image2D_, (newImageCL->getImage()), glm::svec3(0), glm::svec3(0), glm::svec3(dimensions_, 1));
    return newImageCL;
}

void ImageCL::deinitialize() {
    delete image2D_;
    image2D_ = 0;
}

void ImageCL::upload( const void* data )
{
    OpenCL::instance()->getQueue().enqueueWriteImage(*image2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, const_cast<void*>(data));
}

void ImageCL::download( void* data ) const
{
    OpenCL::instance()->getQueue().enqueueReadImage(*image2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, data);
}

void ImageCL::resize(uvec2 dimensions)
{
    if (dimensions == dimensions_) {
        return;
    }
    cl::Image2D* resizedImage2D = new cl::Image2D(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions.x, dimensions.y);
    ImageCLResizer::resize(*image2D_, *resizedImage2D, dimensions);
    delete image2D_;
    image2D_ = resizedImage2D;
    ImageRepresentation::resize(dimensions);

}

bool ImageCL::copyAndResizeImage(DataRepresentation* target) {
    ImageCL* targetCL = dynamic_cast<ImageCL*>(target);

    if (!targetCL) return false;

    ImageCLResizer::resize(*image2D_, (targetCL->getImage()), targetCL->getDimensions());
	
	return true;

}




} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::ImageCL& value)
{
    return setArg(index, value.getImage());
}


} // namespace cl
