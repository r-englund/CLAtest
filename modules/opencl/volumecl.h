#ifndef IVW_VOLUMECL_H
#define IVW_VOLUMECL_H

#include <inviwo/core/inviwo.h>
#include <inviwo/core/datastructures/volumerepresentation.h>
#include <inviwo/core/util/typetostring.h>
#include <modules/opencl/glmcl.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeCL : public VolumeRepresentation {

public:
    VolumeCL(DataFormatBase format = DataFormatBase());
    VolumeCL(uvec3 dimensions, DataFormatBase format = DataFormatBase());
    virtual ~VolumeCL();
    
    virtual void initialize() {};
    virtual void deinitialize() {};
    cl::ImageFormat getFormat() const { return imageFormat_;}
    cl::Image3D getVolume() const { return *image3D_; }

protected:
    cl::Image3D* image3D_;
    cl::ImageFormat imageFormat_;
};

template<typename T>
class IVW_MODULE_OPENCL_API VolumeCLPrecision : public VolumeCL {
public:
    VolumeCLPrecision(DataFormatBase format = GenericDataFormat(T)());
    VolumeCLPrecision(uvec3 dimensions, DataFormatBase format = GenericDataFormat(T)());
    VolumeCLPrecision(T* voxels, uvec3 dimensions, DataFormatBase format = GenericDataFormat(T)());
    virtual ~VolumeCLPrecision() {};
    virtual void initialize() { initialize(0); }
    virtual void initialize(void* voxels = NULL);
    virtual void deinitialize();
    virtual DataRepresentation* clone();
private :
    void setTypeAndFormat();
};

template<typename T>
VolumeCLPrecision<T>::VolumeCLPrecision(DataFormatBase format) : VolumeCL(format) {
    VolumeCLPrecision<T>::setTypeAndFormat();
    VolumeCLPrecision<T>::initialize(0);
}

template<typename T>
VolumeCLPrecision<T>::VolumeCLPrecision(uvec3 dimensions, DataFormatBase format) : VolumeCL(dimensions, format) {
    VolumeCLPrecision<T>::setTypeAndFormat();
    VolumeCLPrecision<T>::initialize(0);
}

template<typename T>
VolumeCLPrecision<T>::VolumeCLPrecision(T* texels, uvec3 dimensions, DataFormatBase format) : VolumeCL(dimensions, format) {
    VolumeCLPrecision<T>::setTypeAndFormat();
    VolumeCLPrecision<T>::initialize(texels);
}

template<typename T>
void VolumeCLPrecision<T>::setTypeAndFormat() {
    imageFormat_ = cl::typeToImageFormat(getDataFormatId());

}



template<typename T>
void VolumeCLPrecision<T>::initialize(void* texels) {
    image3D_ = new cl::Image3D(OpenCL::getInstance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions_.x, dimensions_.y, dimensions_.z);
    if (texels != NULL) {
        OpenCL::getInstance()->getQueue().enqueueWriteImage(*image3D_, true, glm::svec3(0), glm::svec3(dimensions_), 0, 0, texels);
    }
    VolumeCL::initialize();
}

template<typename T>
DataRepresentation* VolumeCLPrecision<T>::clone() {
    VolumeCLPrecision* newVolumeCL = new VolumeCLPrecision<T>(dimensions_);
    //TODO:: Copy volume textures if necessary
    return newVolumeCL;
}

template<typename T>
void VolumeCLPrecision<T>::deinitialize() {
    delete image3D_;
    image3D_ = 0;
    VolumeCL::deinitialize();
}

typedef VolumeCLPrecision<uint8_t> VolumeCLuint8;
typedef VolumeCLPrecision<int8_t> VolumeCLint8;
typedef VolumeCLPrecision<uint16_t> VolumeCLuint16;
typedef VolumeCLPrecision<int16_t> VolumeCLint16;
typedef VolumeCLPrecision<uint32_t> VolumeCLuint32;
typedef VolumeCLPrecision<int32_t> VolumeCLint32;
typedef VolumeCLPrecision<float> VolumeCLfloat;

} // namespace

#endif // IVW_VOLUMECL_H
