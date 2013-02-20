#include <inviwo/core/inviwocommondefines.h>
#include <inviwo/core/util/assertion.h>
#include <inviwo/core/util/logdistributor.h>
#include <modules/opencl/imageclresizer.h>

namespace inviwo {


ImageCLResizer::ImageCLResizer() {

    cl::Program program = OpenCL::buildProgram(IVW_DIR+"modules/opencl/cl/img_resize.cl");

    resizeKernel_ = cl::Kernel(program, "resizeImage");
}

 void ImageCLResizer::resize(const cl::Image2D& src, const cl::Image2D& dst, const glm::uvec2& resizeToDimension)
 {
    static ImageCLResizer instance;

    instance.getResizeKernel()->setArg(0, src);
    instance.resizeKernel_.setArg(1, dst);
    cl::Event event;
    OpenCL::getInstance()->getQueue().enqueueNDRangeKernel(instance.resizeKernel_, cl::NullRange, cl::NDRange(resizeToDimension[0], resizeToDimension[1]),
        cl::NullRange, NULL, &event);
    
    event.wait();
    #if IVW_PROFILING
    LogInfoS("OpenCL", "Image resizing from (" << src.getImageInfo<CL_IMAGE_WIDTH>() << ", " << src.getImageInfo<CL_IMAGE_HEIGHT>() << ") to (" << resizeToDimension.x << ", " << resizeToDimension.y << ") in " << event.getElapsedTime() << " ms");
    #endif
 }
} // namespace
