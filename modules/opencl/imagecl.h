#ifndef IVW_IMAGECL_H
#define IVW_IMAGECL_H

#include <inviwo/core/inviwo.h>
#include <inviwo/core/datastructures/imagerepresentation.h>
#include <inviwo/core/util/typetostring.h>
#include <modules/opencl/glmcl.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API ImageCL : public ImageRepresentation {

public:
    ImageCL();
    ImageCL(ivec2 dimensions);
    virtual ~ImageCL();
    virtual std::string getClassName() const { return "ImageCL"; }
    virtual void resize(ivec2 dimensions);
    virtual ivec2 dimension() { return dimensions_;}
    virtual void copyAndResizeImage(DataRepresentation* target);
    virtual DataRepresentation* clone()=0;
    cl::ImageFormat getFormat() const { return imageFormat_;}
    const cl::Image2D* getImage() const { return image2D_; }

protected:
    cl::Image2D* image2D_;
    cl::ImageFormat imageFormat_;
};

template<typename T>
class IVW_MODULE_OPENCL_API ImageCLPrecision : public ImageCL {
public:
    ImageCLPrecision();
    ImageCLPrecision(ivec2 dimensions);
    ImageCLPrecision(T* texels, ivec2 dimensions);
    virtual ~ImageCLPrecision() {};
    virtual void initialize(void* texels);
    virtual void deinitialize();
    virtual DataRepresentation* clone();
private :
    void setTypeAndFormat();
};

template<typename T>
ImageCLPrecision<T>::ImageCLPrecision() : ImageCL() {
    ImageCLPrecision<T>::setTypeAndFormat();
    ImageCLPrecision<T>::initialize(0);
}

template<typename T>
ImageCLPrecision<T>::ImageCLPrecision(ivec2 dimensions) : ImageCL(dimensions) {
    ImageCLPrecision<T>::setTypeAndFormat();
    ImageCLPrecision<T>::initialize(0);
}

template<typename T>
ImageCLPrecision<T>::ImageCLPrecision(T* texels, ivec2 dimensions) : ImageCL(dimensions) {
    ImageCLPrecision<T>::setTypeAndFormat();
    ImageCLPrecision<T>::initialize(texels);
}

template<typename T>
void ImageCLPrecision<T>::setTypeAndFormat() {
    imageFormat_ = cl::typeToImageFormat<T>();

}



template<typename T>
void ImageCLPrecision<T>::initialize(void* texels) {
    image2D_ = new cl::Image2D(OpenCL::getInstance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions_.x, dimensions_.y);
    if (!texels) {
        OpenCL::getInstance()->getSynchronosGPUQueue().enqueueWriteImage(*image2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, texels);
    }
    ImageCL::initialize();
}

template<typename T>
DataRepresentation* ImageCLPrecision<T>::clone() {
    ImageCLPrecision* newImageCL = new ImageCLPrecision<T>(dimensions_);
    OpenCL::getInstance()->getSynchronosGPUQueue().enqueueCopyImage(image2D_, *(newImageCL->getImage()), glm::svec3(0), glm::svec3(0), dimensions_);
    return newImageCL;
}

template<typename T>
void ImageCLPrecision<T>::deinitialize() {
    delete image2D_;
    image2D_ = 0;
    ImageCL::deinitialize();
}

typedef ImageCLPrecision< glm::detail::tvec4<uint8_t> > ImageCLuint8;
typedef ImageCLPrecision< glm::detail::tvec4<int8_t> > ImageCLint8;
typedef ImageCLPrecision< glm::detail::tvec4<uint16_t> > ImageCLuint16;
typedef ImageCLPrecision< glm::detail::tvec4<int16_t> > ImageCLint16;
typedef ImageCLPrecision< glm::detail::tvec4<uint32_t> > ImageCLuint32;
typedef ImageCLPrecision< glm::detail::tvec4<int32_t> > ImageCLint32;
typedef ImageCLPrecision< glm::detail::tvec4<float> > ImageCLfloat;

} // namespace

#endif // IVW_IMAGECL_H
