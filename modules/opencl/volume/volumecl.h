#ifndef IVW_VOLUMECL_H
#define IVW_VOLUMECL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeCL : public VolumeRepresentation {

public:
    VolumeCL(const DataFormatBase* format = DataFormatBase::get(), const void* data = NULL);
    VolumeCL(uvec3 dimensions, const DataFormatBase* format = DataFormatBase::get(), const void* data = NULL);
    virtual ~VolumeCL();
    
    virtual void initialize() {};
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    
    void initialize(const void* voxels);

    virtual void setDimensions(uvec3 dimensions) { dimensions_ = dimensions; deinitialize(); initialize(); }

    void upload(const void* data);
    /**
     * Download data to preallocated memory.
     * 
     * @param data (void *) Preallocated pointer that will contain data after function returns.
     * @return (void)
     */
    void download(void* data) const;
    cl::ImageFormat getFormat() const { return imageFormat_;}
    cl::Image3D getVolume() const { return *image3D_; }

protected:
    cl::Image3D* image3D_;
    cl::ImageFormat imageFormat_;
};



} // namespace

namespace cl {

// Kernel argument specializations for VolumeCL type 
// (enables calling cl::Queue::setArg with VolumeCL
template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::VolumeCL& value);
 
} // namespace cl

#endif // IVW_VOLUMECL_H
