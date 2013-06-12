#include <modules/opencl/imageclgl.h>
#include <modules/opencl/imageclresizer.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

ImageCLGL::ImageCLGL(DataFormatBase format, Texture2D* data)
    : ImageRepresentation(uvec2(data->getWidth(), data->getHeight()), format), image2D_(0), texture_(data) 
{
    initialize(data);
}

ImageCLGL::ImageCLGL(uvec2 dimensions, DataFormatBase format, Texture2D* data)
    : ImageRepresentation(dimensions, format), texture_(data)
{
    initialize(data);
}

ImageCLGL::~ImageCLGL() { 
    deinitialize(); 
}

void ImageCLGL::initialize(const Texture2D* texture) {
    ivwAssert(texture != 0, "Cannot initialize with null OpenGL texture");
    image2D_ = new cl::Image2DGL(OpenCL::getInstance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture->getID());
    ImageCLGL::initialize();
}

DataRepresentation* ImageCLGL::clone() const {
    ImageCLGL* newImageCLGL = new ImageCLGL(dimensions_, getDataFormat(), NULL);
    OpenCL::getInstance()->getQueue().enqueueCopyImage(*image2D_, (newImageCLGL->getImage()), glm::svec3(0), glm::svec3(0), glm::svec3(dimensions_, 1));
    return newImageCLGL;
}

void ImageCLGL::deinitialize() {
    delete image2D_;
    image2D_ = 0; texture_ = 0;
}


void ImageCLGL::resize(uvec2 dimensions) {
    if (dimensions == dimensions_) {
        return;
    }
    // Make sure that the OpenCL image is deleted before resizing the texture
    delete image2D_;
    texture_->resize(dimensions);
    image2D_ = new cl::Image2DGL(OpenCL::getInstance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture_->getID());
    ImageRepresentation::resize(dimensions);
}

void ImageCLGL::copyAndResizeImage(DataRepresentation* target) {
    ivwAssert(false, "Not implemented");


}




} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::ImageCLGL& value)
{
    return setArg(index, value.getImage());
}


} // namespace cl
