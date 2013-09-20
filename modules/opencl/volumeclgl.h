#ifndef IVW_VOLUMECLGL_H
#define IVW_VOLUMECLGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <inviwo/core/util/typetostring.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opengl/glwrap/texture3d.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeCLGL : public VolumeRepresentation {

public:
    VolumeCLGL(const DataFormatBase* format = DataFormatBase::get(), const Texture3D* data = NULL);
    VolumeCLGL(const uvec3& dimensions, const DataFormatBase* format, const Texture3D* data);
    virtual ~VolumeCLGL();
    virtual std::string getClassName() const { return "VolumeCLGL"; }
    virtual void initialize(){};
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    
    void initialize(const Texture3D* texture);

    const cl::Image3DGL& getVolume() const { return *(image3D_); }
    const Texture3D* getTexture() const { return texture_; }

    void aquireGLObject(std::vector<cl::Event>* syncEvents = NULL) const {
        std::vector<cl::Memory> syncImages(1, *image3D_); 
        OpenCL::instance()->getQueue().enqueueAcquireGLObjects(&syncImages, syncEvents);
    }
    void releaseGLObject(std::vector<cl::Event>* syncEvents = NULL, cl::Event* event= NULL) const {
        std::vector<cl::Memory> syncImages(1, *image3D_); 
        OpenCL::instance()->getQueue().enqueueReleaseGLObjects(&syncImages, syncEvents, event);
    }

protected:
    cl::Image3DGL* image3D_;
    const Texture3D* texture_;
};

} // namespace

namespace cl {

// Kernel argument specializations for VolumeCLGL type 
// (enables calling cl::Queue::setArg with VolumeCLGL)
template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::VolumeCLGL& value);

} // namespace cl



#endif // IVW_VOLUMECLGL_H
