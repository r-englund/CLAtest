#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#include "modules/opencl/inviwoopencl.h"
#include <cstdio>

int test_buffer()
{
	int Error(0);
    
    int test[4] = {0, 1, 2, 3};
    cl::Buffer buffer(CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(test), test); 
    
    int result[4];
    inviwo::OpenCL::getInstance()->getSynchronosGPUQueue().enqueueReadBuffer(buffer, true, 0, sizeof(test), result);
    for(int i = 0; i < 4; ++i) {
        Error |= test[i] != result[i];
        
    }
	return Error;
}

int main()
{
	int Error(0);

	Error += test_buffer();

	return Error;
}

