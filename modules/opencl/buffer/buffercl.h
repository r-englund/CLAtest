#ifndef IVW_BUFFERCL_H
#define IVW_BUFFERCL_H

#include <modules/opencl/openclmoduledefine.h>
#include <modules/opencl/inviwoopencl.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/datastructures/geometry/attributes.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API BufferCL: public BufferRepresentation {

public:
    /**
     * .
     * 
     * @param size 
     * @param type 
     * @param format 
     * @param data Data to transfer. Does not transfer data if data is NULL.
     * @param readWriteFlag Determine how memory will be used by Kernels: CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY, CL_MEM_READ_WRITE
     */
    BufferCL(size_t size, BufferType type, const DataFormatBase* format, const void* data = NULL, cl_mem_flags readWriteFlag = CL_MEM_READ_WRITE);
    virtual ~BufferCL();

    virtual void initialize();
    virtual void deinitialize();
    virtual BufferCL* clone() const;

    const Buffer* getAttribute() const;

    cl::Buffer getEditableBuffer() { return *buffer_; }
    const cl::Buffer& getBuffer() const { return *const_cast<const cl::Buffer*>(buffer_); }

    void upload(const void* data, size_t size);
    void download(void* data) const;

protected:

    /**
     * Create and optionally transfer data to device.
     * 
     * @param data Data to transfer. Does not transfer data if data is NULL.
     */
    void initialize(const void* data);

    cl::Buffer* buffer_;
    cl_mem_flags readWriteFlag_;


};

} // namespace

namespace cl {

// Kernel argument specializations for BufferCL type 
// (enables calling cl::Queue::setArg with BufferCL)
template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::BufferCL& value);

} // namespace cl

#endif // IVW_BUFFERCL_H
