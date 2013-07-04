#include "volumeraycastercl.h"
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/syncclgl.h>
#include <modules/opencl/imagecl.h>
#include <modules/opencl/imageclgl.h>
#include <modules/opencl/volumeclgl.h>
#include <modules/opencl/kernelmanager.h>

namespace inviwo {

ProcessorClassName(VolumeRaycasterCL, "VolumeRaycasterCL"); 
ProcessorCategory(VolumeRaycasterCL, "Volume Rendering");
ProcessorCodeState(VolumeRaycasterCL, CODE_STATE_EXPERIMENTAL);

VolumeRaycasterCL::VolumeRaycasterCL()
    : Processor(),
    volumePort_("volume"),
    entryPort_("entry-points"),
    exitPort_("exit-points"),
    outport_("outport"),
    lightSourcePos_("lightSourcePos", "Light source position", vec3(1.0f), vec3(-1.0f), vec3(1.0f)),
    samplingRate_("samplingRate", "Sampling rate", 1.0f, 1.0f, 15.0f),
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
    Processor::deinitialize();
}

void VolumeRaycasterCL::process() {
    if( kernel_ == NULL) {
        return;
    }
    
    const ImageCLGL* entryCLGL = entryPort_.getData()->getRepresentation<ImageCLGL>();
    // Will synchronize with OpenGL upon creation and destruction
    SyncCLGL glSync;
    entryCLGL->aquireGLObject(glSync.getGLSyncEvent());

    const ImageCLGL* exitCLGL = exitPort_.getData()->getRepresentation<ImageCLGL>();
    exitCLGL->aquireGLObject();

    Image* outImage = outport_.getData();
    //ImageCL* outImageCL = outImage->getEditableRepresentation<ImageCL>();
    ImageCLGL* outImageCL = outImage->getEditableRepresentation<ImageCLGL>();
    uvec2 outportDim = outImage->getDimension();
    outImageCL->aquireGLObject();


    const Volume* volume = volumePort_.getData();
    const VolumeCLGL* volumeCL = volume->getRepresentation<VolumeCLGL>();
    uvec3 volumeDim = volumeCL->getDimensions();
    volumeCL->aquireGLObject();
    const ImageCL* transferFunctionCL = transferFunction_.get().getData()->getRepresentation<ImageCL>();
    
    cl_uint arg = 0;
    kernel_->setArg(arg++, *volumeCL);
    kernel_->setArg(arg++, *entryCLGL);
    kernel_->setArg(arg++, *exitCLGL);
    kernel_->setArg(arg++, *transferFunctionCL);
    kernel_->setArg(arg++, samplingRate_.get());// / (float)std::max(volumeDim.x, std::max(volumeDim.y, volumeDim.z)) );
    kernel_->setArg(arg++, volumeDim);
    kernel_->setArg(arg++, *outImageCL);
    // 
    OpenCL::getInstance()->getQueue().enqueueNDRangeKernel(*kernel_, cl::NullRange, static_cast<glm::svec2>(outportDim));
    
    volumeCL->releaseGLObject();
    outImageCL->releaseGLObject();
    exitCLGL->releaseGLObject();
    entryCLGL->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());

}

} // namespace
