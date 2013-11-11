#ifndef IVW_BUFFERCLGL_H
#define IVW_BUFFERCLGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opengl/geometry/attributebuffergl.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API BufferCLGL : public BufferRepresentation {

public:
    BufferCLGL(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage = STATIC, const BufferGL* data = NULL, cl_mem_flags readWriteFlag = CL_MEM_READ_WRITE);
    BufferCLGL( const BufferCLGL& rhs );
    virtual ~BufferCLGL();
    virtual std::string getClassName() const { return "BufferCLGL"; }
    virtual void initialize(){};
    virtual void deinitialize();
    virtual BufferCLGL* clone() const;
    
    void initialize(const BufferGL* data);

    const cl::Buffer& getBuffer() const { return *(buffer_); }
    const BufferGL* getBufferGL() const { return bufferGL_; }

    void aquireGLObject(std::vector<cl::Event>* syncEvents = NULL) const {
        std::vector<cl::Memory> syncBuffers(1, *buffer_); 
        OpenCL::instance()->getQueue().enqueueAcquireGLObjects(&syncBuffers, syncEvents);
    }

    void releaseGLObject(std::vector<cl::Event>* syncEvents = NULL, cl::Event* event= NULL) const {
        std::vector<cl::Memory> syncBuffers(1, *buffer_); 
        OpenCL::instance()->getQueue().enqueueReleaseGLObjects(&syncBuffers, syncEvents, event);
    }

protected:
    cl::BufferGL* buffer_;
    const BufferGL* bufferGL_;
    cl_mem_flags readWriteFlag_;
};

} // namespace

namespace cl {

// Kernel argument specializations for BufferCLGL type 
// (enables calling cl::Queue::setArg with BufferCLGL)
template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::BufferCLGL& value);

} // namespace cl



#endif // IVW_BUFFERCLGL_H
