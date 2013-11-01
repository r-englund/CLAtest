#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/buffer/bufferclconverter.h>
#include <modules/opencl/imageclconverter.h>
#include <modules/opencl/imageclglconverter.h>
#include <modules/opencl/openclmodule.h>
#include <modules/opencl/openclcapabilities.h>
#include <modules/opencl/processors/grayscalecl.h>
#include <modules/opencl/processors/volumeraycastercl.h>
#include <modules/opencl/volumeclconverter.h>
#include <modules/opencl/volumeclglconverter.h>
#include <modules/opencl/kernelmanager.h>



namespace inviwo {

OpenCLModule::OpenCLModule() : InviwoModule() {
    setIdentifier("OpenCL");
    setXMLFileName("opencl/openclmodule.xml");
}

void OpenCLModule::initialize() {
    
    OpenCL::instance();
    KernelManager::init();
    // Buffer CL
    addRepresentationConverter(new BufferRAM2CLConverter());
    addRepresentationConverter(new BufferCL2RAMConverter());

    // ImageCL
    addRepresentationConverter(new ImageDisk2CLConverter());
    addRepresentationConverter(new ImageRAM2CLConverter());
    addRepresentationConverter(new ImageCL2RAMConverter());
    addRepresentationConverter(new ImageGL2CLConverter());
    addRepresentationConverter(new ImageCL2GLConverter());

    // ImageCLGL
    addRepresentationConverter(new ImageDisk2CLGLConverter());
    addRepresentationConverter(new ImageRAM2CLGLConverter());
    addRepresentationConverter(new ImageGL2CLGLConverter());
    addRepresentationConverter(new ImageCL2CLGLConverter());
    addRepresentationConverter(new ImageCLGL2RAMConverter());
    addRepresentationConverter(new ImageCLGL2CLConverter());
    addRepresentationConverter(new ImageGL2CLGLConverter());

    // VolumeCL
    addRepresentationConverter(new VolumeDisk2CLConverter());
    addRepresentationConverter(new VolumeRAM2CLConverter());
    addRepresentationConverter(new VolumeCL2RAMConverter());
    // VolumeCLGL
    addRepresentationConverter(new VolumeDisk2CLGLConverter());
    addRepresentationConverter(new VolumeRAM2CLGLConverter());
    addRepresentationConverter(new VolumeGL2CLGLConverter());
    addRepresentationConverter(new VolumeCL2CLGLConverter());
    addRepresentationConverter(new VolumeCLGL2RAMConverter());
    addRepresentationConverter(new VolumeCLGL2CLConverter());
    addRepresentationConverter(new VolumeGL2CLGLConverter());


    addCapabilities(new OpenCLCapabilities());
    // Processors
    registerProcessor(GrayscaleCL);
    registerProcessor(VolumeRaycasterCL);
    InviwoModule::initialize();
}

void OpenCLModule::deinitialize() {
    
}

void OpenCLModule::setupModuleSettings(){
    if (getSettings()){
        OpenCLCapabilities* openclInfo = getTypeFromVector<OpenCLCapabilities>(getCapabilities());
        if (openclInfo){
            ButtonProperty* btnOpenCLInfo = new ButtonProperty("printOpenCLInfo", "Print OpenCL Info");
            btnOpenCLInfo->registerClassMemberFunction(openclInfo, &OpenCLCapabilities::printInfo);
            getSettings()->addProperty(btnOpenCLInfo);
        }           
    }
}

} // namespace
