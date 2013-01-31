#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/util/logdistributor.h>

#include <modules/opencl/openclmodule.h>
#include <modules/opencl/inviwoopencl.h>

#include <modules/opencl/imagecl.h>
#include <inviwo/core/datastructures/image.h>
#include <inviwo/core/datastructures/imageram.h>
#include <cstdio>


int test_buffer()
{
	int error(0);
    
    int test[4] = {0, 1, 2, 3};
    try {
        cl::Buffer buffer(CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(test), test); 
        
        int result[4];
        inviwo::OpenCL::getInstance()->getQueue().enqueueReadBuffer(buffer, true, 0, sizeof(test), result);
        for(int i = 0; i < 4; ++i) {
            error |= test[i] != result[i];
            
        }
    } catch(cl::Error) {
        error +=1;
    }

    

    return error;
}

int test_image() {
    int error(0);
    // Create image representations and module
    inviwo::InviwoApplication app("Test", "");
    app.initialize();
    uint8_t imageData[16] = {1, 1, 1, 1,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0};

    //cl::Image2D img(*inviwo::OpenCL::getInstance()->getContext(), CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, CL_FLOAT, 4, 4, imageData);
    inviwo::Image image(glm::ivec2(4,4));
    inviwo::ImageRAM* ram = image.getRepresentation<inviwo::ImageRAM>();

    try {
        inviwo::ImageCL *imageCL = image.getRepresentation<inviwo::ImageCL>();
        inviwo::OpenCL::getInstance()->getQueue().enqueueWriteImage(*(imageCL->getImage()), true, glm::svec3(0), glm::svec3(4, 4, 1), 0, 0, imageData);
        imageCL->resize(glm::ivec2(2,2));
        uint8_t resizedImageData[4];
        inviwo::OpenCL::getInstance()->getQueue().enqueueReadImage(*(imageCL->getImage()), true, glm::svec3(0), glm::svec3(2, 2, 1), 0, 0, resizedImageData);
    } catch(cl::Error) {
        error +=1;
    }

    return error;
}

int main()
{
    // Enable output to console
    inviwo::LogCentral::instance()->registerLogger(new inviwo::ConsoleLogger());
    
	int error(0);

	error += test_buffer();
    error += test_image();

	return error;
}

