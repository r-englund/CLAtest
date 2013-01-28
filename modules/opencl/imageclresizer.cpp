#include <inviwo/core/inviwocommondefines.h>
#include <inviwo/core/util/assertion.h>
#include <modules/opencl/imageclresizer.h>

namespace inviwo {


ImageCLResizer::ImageCLResizer() {
    std::string kernelStr = inviwo::loadFile(IVW_DIR+"modules/opencl/cl/img_resize.cl");

    cl::Program::Sources source(1, std::make_pair(kernelStr.c_str(), kernelStr.length()));
    cl::Program program = cl::Program(OpenCL::getInstance()->getContext(), source);
    try {
        program.build(std::vector<cl::Device>(1, OpenCL::getInstance()->getDevice()));
    } catch (cl::Error&) {
        OpenCL::printBuildError(std::vector<cl::Device>(1, OpenCL::getInstance()->getDevice()), program);
    }

    resizeKernel_ = cl::Kernel(program, "resizeImage");
}

 void ImageCLResizer::resize(const cl::Image2D& src, const cl::Image2D& dst, const glm::ivec2& resizeToDimension)
 {
    static ImageCLResizer instance;

    instance.getResizeKernel()->setArg(0, src);
    instance.resizeKernel_.setArg(1, dst);
    cl::Event event;
    OpenCL::getInstance()->getSynchronosGPUQueue().enqueueNDRangeKernel(instance.resizeKernel_, cl::NullRange, cl::NDRange(resizeToDimension[0], resizeToDimension[1]),
        cl::NullRange, NULL, &event);
    event.wait();
 }
} // namespace
