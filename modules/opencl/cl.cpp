

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
