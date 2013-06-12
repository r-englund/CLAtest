

#include <modules/opencl/cl.hpp>
#include <modules/opencl/inviwoopencl.h>
// An extended version of Khronos OpenCL wrapper
// As much as possible of a extensions should be kept in the cpp file 
// such that we can merge it with the one from Khronos

inline Device Device::getDefault(cl_int *)
{
    return inviwo::OpenCL::getInstance()->getDevice();
}

Context Context::getDefault(cl_int *) {
    return inviwo::OpenCL::getInstance()->getContext();
}

CommandQueue CommandQueue::getDefault(cl_int *) {
    return inviwo::OpenCL::getInstance()->getQueue();
}
namespace cl {
namespace detail {

#if defined(__CL_ENABLE_EXCEPTIONS)
cl_int errHandler (
        cl_int err,
        const char * errStr)
    {
        if (err != CL_SUCCESS) {
            inviwo::LogOpenCLError(err, errStr);
            throw cl::Error(err, errStr);
        }
        return err;
    }
#else
    cl_int errHandler (cl_int err, const char * errStr)
    {
        inviwo::LogOpenCLError(err, errStr);
        //(void) errStr; // suppress unused variable warning
        return err;
    }
#endif // __CL_ENABLE_EXCEPTIONS

}
}
