#include <modules/opencl/image/imageclgl.h>
#include <modules/opencl/image/imageclresizer.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

ImageCLGL::ImageCLGL(uvec2 dimensions, ImageType type, const DataFormatBase* format, Texture2D* data)
    : ImageRepresentation(dimensions, type, format), texture_(data)
{
    if(data) {
        initialize(data);
    }
}

ImageCLGL::~ImageCLGL() { 
    deinitialize(); 
}

void ImageCLGL::initialize(Texture2D* texture) {
    ivwAssert(texture != 0, "Cannot initialize with null OpenGL texture");
    // Indicate that the texture should not be deleted.
    texture->increaseRefCount();
    image2D_ = new cl::Image2DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture->getID());
    texture->addObserver(this);
    ImageCLGL::initialize();
}

DataRepresentation* ImageCLGL::clone() const {
    Texture2D* clonedTexture = new Texture2D(*texture_); 
    ImageCLGL* newImageCLGL = new ImageCLGL(dimensions_, getImageType(), getDataFormat(), clonedTexture);
    return newImageCLGL;
}

void ImageCLGL::deinitialize() {
    delete image2D_;
    image2D_ = 0; 
    if(texture_ && texture_->decreaseRefCount() <= 0){
        delete texture_;
        texture_ = NULL;
    }
}


void ImageCLGL::resize(uvec2 dimensions) {
    if (dimensions == dimensions_) {
        return;
    }
    // Make sure that the OpenCL image is deleted before resizing the texture
    // By observing the texture we will make sure that the OpenCL image is 
    // deleted and reattached after resizing is done.
    const_cast<Texture2D*>(texture_)->resize(dimensions);
    ImageRepresentation::resize(dimensions);
}

bool ImageCLGL::copyAndResizeImage(DataRepresentation* targetRep) {
    //ivwAssert(false, "Not implemented");

    // Make sure that the OpenCL image is deleted before resizing the texture
    // TODO: Implement copying in addition to the resizing

    ImageCLGL* target = dynamic_cast<ImageCLGL*>(targetRep);
    ImageCLGL* source = this;
    target->resize(source->getDimensions());

    return true;
}

void ImageCLGL::notifyBeforeTextureInitialization() {
    delete image2D_; image2D_ = 0; 
}

void ImageCLGL::notifyAfterTextureInitialization() {
    image2D_ = new cl::Image2DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture_->getID());
}




} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::ImageCLGL& value)
{
    return setArg(index, value.getImage());
}


} // namespace cl
