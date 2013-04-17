#ifndef IVW_IMAGECL_H
#define IVW_IMAGECL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/imagerepresentation.h>
#include <inviwo/core/util/typetostring.h>
#include <modules/opencl/glmcl.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API ImageCL : public ImageRepresentation {

public:
    ImageCL(DataFormatBase format = DataFormatBase());
    ImageCL(uvec2 dimensions, DataFormatBase format = DataFormatBase());
    virtual ~ImageCL();
    virtual std::string getClassName() const { return "ImageCL"; }
    virtual void initialize(){};
    virtual void deinitialize(){};
    virtual void resize(uvec2 dimensions);
    virtual uvec2 dimension() { return dimensions_;}
    virtual void copyAndResizeImage(DataRepresentation* target);
    virtual DataRepresentation* clone() const = 0;
    cl::ImageFormat getFormat() const { return imageFormat_;}
    cl::Image2D getEditableImage() { return *image2D_; }
    const cl::Image2D getImage() const { return *const_cast<const cl::Image2D*>(image2D_); }

protected:
    cl::Image2D* image2D_;
    cl::ImageFormat imageFormat_;
};

template<typename T>
class IVW_MODULE_OPENCL_API ImageCLPrecision : public ImageCL {
public:
    ImageCLPrecision(DataFormatBase format = GenericDataFormat(T)());
    ImageCLPrecision(uvec2 dimensions, DataFormatBase format = GenericDataFormat(T)());
    ImageCLPrecision(T* texels, uvec2 dimensions, DataFormatBase format = GenericDataFormat(T)());
    virtual ~ImageCLPrecision() {};
    virtual void initialize(void* texels);
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
private :
    void setTypeAndFormat();
};

template<typename T>
ImageCLPrecision<T>::ImageCLPrecision(DataFormatBase format) : ImageCL(format) {
    ImageCLPrecision<T>::setTypeAndFormat();
    ImageCLPrecision<T>::initialize(0);
}

template<typename T>
ImageCLPrecision<T>::ImageCLPrecision(uvec2 dimensions, DataFormatBase format) : ImageCL(dimensions, format) {
    ImageCLPrecision<T>::setTypeAndFormat();
    ImageCLPrecision<T>::initialize(0);
}

template<typename T>
ImageCLPrecision<T>::ImageCLPrecision(T* texels, uvec2 dimensions, DataFormatBase format) : ImageCL(dimensions, format) {
    ImageCLPrecision<T>::setTypeAndFormat();
    ImageCLPrecision<T>::initialize(texels);
}

template<typename T>
void ImageCLPrecision<T>::setTypeAndFormat() {
    imageFormat_ = cl::typeToImageFormat(getDataFormatId());

}



template<typename T>
void ImageCLPrecision<T>::initialize(void* texels) {
    
    
    if (texels != NULL) {
        // Could performance be increased by using pinned memory?
        // 3.1.1 http://www.nvidia.com/content/cudazone/CUDABrowser/downloads/papers/NVIDIA_OpenCL_BestPracticesGuide.pdf
        //cl::Buffer pinnedMem(OpenCL::getInstance()->getContext(), CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, sizeof(texels), NULL, NULL);
        //unsigned char* mappedMem = (unsigned char*)OpenCL::getInstance()->getQueue().enqueueMapBuffer(pinnedMem, true, CL_MAP_WRITE, 0, sizeof(texels), 0);
        //memcpy(mappedMem, texels, sizeof(texels));
        //OpenCL::getInstance()->getQueue().enqueueWriteImage(*image2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, mappedMem);
        //OpenCL::getInstance()->getQueue().enqueueUnmapMemObject(pinnedMem, mappedMem);

        // This should also use pinned memory...
        image2D_ = new cl::Image2D(OpenCL::getInstance()->getContext(), 
            CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR, 
            getFormat(), dimensions_.x, dimensions_.y, 0, texels);
        // Alternatively first allocate memory on device and then transfer
        //image2D_ = new cl::Image2D(OpenCL::getInstance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions_.x, dimensions_.y);
        //OpenCL::getInstance()->getQueue().enqueueWriteImage(*image2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, texels);
    } else {
        image2D_ = new cl::Image2D(OpenCL::getInstance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions_.x, dimensions_.y);
    }
    ImageCL::initialize();
}

template<typename T>
DataRepresentation* ImageCLPrecision<T>::clone() const {
    ImageCLPrecision* newImageCL = new ImageCLPrecision<T>(dimensions_);
    OpenCL::getInstance()->getQueue().enqueueCopyImage(*image2D_, (newImageCL->getImage()), glm::svec3(0), glm::svec3(0), glm::svec3(dimensions_, 1));
    return newImageCL;
}

template<typename T>
void ImageCLPrecision<T>::deinitialize() {
    delete image2D_;
    image2D_ = 0;
    ImageCL::deinitialize();
}
typedef ImageCLPrecision< uint8_t > ImageCLuint8;
typedef ImageCLPrecision< int8_t > ImageCLint8;
typedef ImageCLPrecision< uint16_t > ImageCLuint16;
typedef ImageCLPrecision< int16_t > ImageCLint16;
typedef ImageCLPrecision< uint32_t > ImageCLuint32;
typedef ImageCLPrecision< int32_t > ImageCLint32;
typedef ImageCLPrecision< float > ImageCLfloat32;

typedef ImageCLPrecision< glm::detail::tvec4<uint8_t> > ImageCLuint8vec4;
typedef ImageCLPrecision< glm::detail::tvec4<int8_t> > ImageCLint8vec4;
typedef ImageCLPrecision< glm::detail::tvec4<uint16_t> > ImageCLuint16vec4;
typedef ImageCLPrecision< glm::detail::tvec4<int16_t> > ImageCLint16vec4;
typedef ImageCLPrecision< glm::detail::tvec4<uint32_t> > ImageCLuivec4;
typedef ImageCLPrecision< glm::detail::tvec4<int32_t> > ImageCLivec4;
typedef ImageCLPrecision< glm::detail::tvec4<float> > ImageCLvec4float32;

} // namespace

namespace cl {

// Kernel argument specializations for ImageCL type 
// (enables calling cl::Queue::setArg with ImageCL
template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::ImageCL& value);

}



#endif // IVW_IMAGECL_H
