#include "volumeraycastercl.h"
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/imagecl.h>
#include <modules/opencl/volumecl.h>

namespace inviwo {

VolumeRaycasterCL::VolumeRaycasterCL()
    : Processor(),
    volumePort_(Port::INPORT, "volume"),
    entryPort_(Port::INPORT, "entry-points"),
    exitPort_(Port::INPORT, "exit-points"),
    outport_(Port::OUTPORT, "outport"),
    lightSourcePos_("lightSourcePos", "Light source position", vec3(1.0f), vec3(-1.0f), vec3(1.0f)),
    samplingRate_("samplingRate", "Sampling rate", 1.0f, 0.1f, 15.0f),
    kernel_(NULL)
{
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    addProperty(samplingRate_);
    addProperty(lightSourcePos_);
}

VolumeRaycasterCL::~VolumeRaycasterCL() {}

Processor* VolumeRaycasterCL::create() const {
    return new VolumeRaycasterCL();
}

void VolumeRaycasterCL::initialize() {
    Processor::initialize();
    try {
        cl::Program program = OpenCL::buildProgram(IVW_DIR+"modules/opencl/cl/volumeraycaster.cl");
        kernel_ = new cl::Kernel(program, "raycaster");

    } catch (cl::Error& e) {
        throw e;
    }
    
    
}

void VolumeRaycasterCL::deinitialize() {
    delete kernel_;
    Processor::deinitialize();
}

void VolumeRaycasterCL::process() {
    size_t ttt = sizeof(uint8_t);
    size_t tt = sizeof(glm::detail::tvec4<uint8_t>);
    //Image* outImage = outport_.getData();
    //uvec2 outportDim = outImage->size();
    uvec2 outportDim = uvec2(4, 4);
    Image* outImage = new Image(outportDim);
    outImage->clearRepresentations();
    outImage->addRepresentation(new ImageCLuint8vec4(outportDim));
    ImageCL* outImageCL = outImage->getRepresentation<ImageCL>();


    Volume* volume = volumePort_.getData();
    VolumeCL* volumeCL = volume->getRepresentation<VolumeCL>();
    uvec3 volumeDim = volumeCL->getDimensions();
    cl_uint arg = 0;
    //kernel_->setArg(arg++, volumeCL->getVolume());
    //kernel_->setArg(arg++, (entryPort_.getData()->getRepresentation<ImageCL>()->getImage()));
    //kernel_->setArg(arg++, exitPort_.getData()->getRepresentation<ImageCL>()->getImage());
    //kernel_->setArg(arg++, samplingRate_.get());
    kernel_->setArg(arg++, outImageCL->getImage());
    // 
    OpenCL::getInstance()->getQueue().enqueueNDRangeKernel(*kernel_, cl::NullRange, static_cast<glm::svec2>(outportDim));

    ImageRAM* testImage = outImage->getRepresentation<ImageRAM>();
    glm::detail::tvec4<uint8_t>* data = static_cast<glm::detail::tvec4<uint8_t>*>(testImage->getData());
    for(unsigned int y = 0; y < outportDim.y; ++y) {
        for(unsigned int x = 0; x < outportDim.x; ++x) {
            unsigned int index = outportDim.x*y + x;
            glm::detail::tvec4<int> dd(data[index]);
            std::cerr << "(" << x << ", " << y << ")" << static_cast<int>(dd.x)  << ", " << static_cast<int>(dd.y) << ", " << static_cast<int>(dd.z) << ", " << static_cast<int>(dd.w) << std::endl;
        }
    }
}

} // namespace
