#include <modules/opencl/buffer/bufferclgl.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

BufferCLGL::BufferCLGL(size_t size, BufferType type, const DataFormatBase* format, const BufferGL* data, cl_mem_flags readWriteFlag)
    : BufferRepresentation(size, type, format), bufferGL_(data), readWriteFlag_(readWriteFlag)
{
    if(data) {
        initialize(data);
    }
}

BufferCLGL::BufferCLGL( const BufferCLGL& rhs )
: BufferRepresentation(rhs.getSize(), rhs.getBufferType(), rhs.getDataFormat()), bufferGL_(rhs.getBufferGL()), readWriteFlag_(rhs.readWriteFlag_)
{
    initialize(bufferGL_);
}

BufferCLGL::~BufferCLGL() { 
    deinitialize(); 
}

void BufferCLGL::initialize(const BufferGL* data) {
    ivwAssert(data != 0, "Cannot initialize with null OpenGL buffer");
    buffer_ = new cl::BufferGL(OpenCL::instance()->getContext(), readWriteFlag_, data->getId());
    BufferCLGL::initialize();
}

BufferCLGL* BufferCLGL::clone() const {
    return new BufferCLGL(*this);
}

void BufferCLGL::deinitialize() {
    delete buffer_;
    buffer_ = 0; 
}

} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::BufferCLGL& value)
{
    return setArg(index, value.getBuffer());
}


} // namespace cl
