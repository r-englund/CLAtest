#include "volumeraycastercl.h"
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/imagecl.h>
#include <modules/opencl/volumecl.h>
#include <modules/opencl/kernelmanager.h>

namespace inviwo {

ProcessorClassName(VolumeRaycasterCL, "VolumeRaycasterCL"); 
ProcessorCategory(VolumeRaycasterCL, "Ray Caster");
ProcessorCodeState(VolumeRaycasterCL, CODE_STATE_EXPERIMENTAL);

VolumeRaycasterCL::VolumeRaycasterCL()
    : Processor(),
    volumePort_("volume"),
    entryPort_("entry-points"),
    exitPort_("exit-points"),
    outport_("outport"),
    lightSourcePos_("lightSourcePos", "Light source position", vec3(1.0f), vec3(-1.0f), vec3(1.0f)),
    samplingRate_("samplingRate", "Sampling rate", 1.0f, 0.1f, 15.0f),
    transferFunction_("transferFunction", "Transfer function", TransferFunction()),
    kernel_(NULL)
{
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    addProperty(samplingRate_);
    addProperty(lightSourcePos_);
    addProperty(transferFunction_);
}

VolumeRaycasterCL::~VolumeRaycasterCL() {}

void VolumeRaycasterCL::initialize() {
    Processor::initialize();
    try {
        cl::Program* program = KernelManager::getRef().buildProgram(IVW_DIR+"modules/opencl/cl/volumeraycaster.cl");
        kernel_ = KernelManager::getRef().getKernel(program, "raycaster");
        //cl::Program program = OpenCL::buildProgram(IVW_DIR+"modules/opencl/cl/volumeraycaster.cl");
        //kernel_ = new cl::Kernel(program, "raycaster");

    } catch (cl::Error&) {
        
    }
    
    
}

void VolumeRaycasterCL::deinitialize() {
    delete kernel_;
    Processor::deinitialize();
}

void VolumeRaycasterCL::process() {
    if( kernel_ == NULL) {
        return;
    }
    Image* outImage = outport_.getData();
    const ImageCL* entryCL = entryPort_.getData()->getRepresentation<ImageCL>();
    outImage->resize(entryCL->getDimension());
    uvec2 outportDim = outImage->getDimension();
    //uvec2 outportDim = uvec2(4, 4);
    //Image* outImage = new Image(outportDim);
    //outImage->clearRepresentations();
    //outImage->addRepresentation(new ImageCLuint8vec4(outportDim));
    //outImage->addRepresentation(new ImageCLvec4float32(outportDim));
    ImageCL* outImageCL = outImage->getEditableRepresentation<ImageCL>();


    const Volume* volume = volumePort_.getData();
    const VolumeCL* volumeCL = volume->getRepresentation<VolumeCL>();
    uvec3 volumeDim = volumeCL->getDimensions();
    const ImageCL* transferFunctionCL = transferFunction_.get().getData()->getRepresentation<ImageCL>();
    
    cl_uint arg = 0;
    kernel_->setArg(arg++, *volumeCL);
    kernel_->setArg(arg++, *entryCL);
    kernel_->setArg(arg++, *exitPort_.getData()->getRepresentation<ImageCL>());
    kernel_->setArg(arg++, *transferFunctionCL);
    kernel_->setArg(arg++, samplingRate_.get());// / (float)std::max(volumeDim.x, std::max(volumeDim.y, volumeDim.z)) );
    kernel_->setArg(arg++, volumeDim);
    kernel_->setArg(arg++, *outImageCL);
    // 
    OpenCL::getInstance()->getQueue().enqueueNDRangeKernel(*kernel_, cl::NullRange, static_cast<glm::svec2>(outportDim));

    //ImageRAM* testImage = outImage->getRepresentation<ImageRAM>();
    // Texture2D* colorTexture_;
    //colorTexture_->bind();
    //colorTexture_->upload();
    //ImageRAM* testImage = entryPort_.getData()->getRepresentation<ImageRAM>();
    //glm::detail::tvec4<uint8_t>* data = static_cast<glm::detail::tvec4<uint8_t>*>(testImage->getData());
    ////glm::detail::tvec4<float>* data = static_cast<glm::detail::tvec4<float>*>(testImage->getData());
    //for(unsigned int y = 0; y < outportDim.y; ++y) {
    //    for(unsigned int x = 0; x < outportDim.x; ++x) {
    //        unsigned int index = outportDim.x*y + x;
    //        glm::detail::tvec4<int> dd(data[index]);
    //        //glm::detail::tvec4<float> dd(data[index]);
    //        std::cerr << "(" << x << ", " << y << ")" << dd.x  << ", " << dd.y << ", " << dd.z << ", " << dd.w << std::endl;

    //        //std::cerr << "(" << x << ", " << y << ")" << data[index] << std::endl;
    //    }
    //}
}

} // namespace
