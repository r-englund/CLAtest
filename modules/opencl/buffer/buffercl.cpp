#include <modules/opencl/buffer/buffercl.h>

namespace inviwo {

BufferCL::BufferCL(size_t size, BufferType type, const DataFormatBase* format, const void* data, cl_mem_flags readWriteFlag)
    : BufferRepresentation(size, type, format), readWriteFlag_(readWriteFlag)
{
    initialize(data);
}

BufferCL::~BufferCL() {
    deinitialize();
}



void BufferCL::upload( const void* data, size_t size)
{   
    // Resize buffer if necessary
    if( size > getSize()*getSizeOfElement()) {
        deinitialize();
        setSize(size/getSizeOfElement());
        initialize(data);
    } else {
        OpenCL::instance()->getQueue().enqueueWriteBuffer(*buffer_, true, 0, getSize()*getSizeOfElement(), const_cast<void*>(data));
    }
    

}

void BufferCL::download( void* data ) const
{
    OpenCL::instance()->getQueue().enqueueReadBuffer(*buffer_, true, 0, getSize()*getSizeOfElement(), data);
}

BufferCL* BufferCL::clone() const{
    return new BufferCL(*this);
}

void BufferCL::initialize(){
    
    
}

void BufferCL::deinitialize(){
    delete buffer_;
    buffer_ = NULL;
}

void BufferCL::initialize(const void* data) {
    // Generate a new buffer
    if (data != NULL) {
        // CL_MEM_COPY_HOST_PTR can be used with CL_MEM_ALLOC_HOST_PTR to initialize the contents of the cl_mem object allocated using host-accessible (e.g. PCIe) memory.
        buffer_ = new cl::Buffer(OpenCL::instance()->getContext(), 
            readWriteFlag_ | CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR, 
            getSize()*getSizeOfElement(), const_cast<void*>(data));

    } else {
        buffer_ = new cl::Buffer(OpenCL::instance()->getContext(), readWriteFlag_, getSize()*getSizeOfElement());
    }
    BufferCL::initialize();
}






} // namespace

