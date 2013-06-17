#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/imageclconverter.h>
#include <modules/opencl/imageclglconverter.h>
#include <modules/opencl/openclmodule.h>
#include <modules/opencl/openclinfo.h>
#include <modules/opencl/processors/grayscalecl.h>
#include <modules/opencl/processors/volumeraycastercl.h>
#include <modules/opencl/volumeclconverter.h>
#include <modules/opencl/kernelmanager.h>



namespace inviwo {

OpenCLModule::OpenCLModule() : InviwoModule() {
    setIdentifier("OpenCL");
    setXMLFileName("opencl/openclmodule.xml");
}

void OpenCLModule::initialize() {
    
    OpenCL::getInstance();
    KernelManager::init();

    addRepresentationConverter(new ImageDisk2CLConverter());
    addRepresentationConverter(new ImageRAM2CLConverter());
    addRepresentationConverter(new ImageCL2RAMConverter());
    addRepresentationConverter(new ImageGL2CLConverter());
    addRepresentationConverter(new ImageCL2GLConverter());

    addRepresentationConverter(new ImageDisk2CLGLConverter());
    addRepresentationConverter(new ImageRAM2CLGLConverter());
    addRepresentationConverter(new ImageGL2CLGLConverter());
    addRepresentationConverter(new ImageCL2CLGLConverter());
    addRepresentationConverter(new ImageCLGL2RAMConverter());
    addRepresentationConverter(new ImageCLGL2CLConverter());
    addRepresentationConverter(new ImageGL2CLGLConverter());


    addRepresentationConverter(new VolumeDisk2CLConverter());
    addRepresentationConverter(new VolumeRAM2CLConverter());
    addRepresentationConverter(new VolumeCL2RAMConverter());



    addResourceInfo(new OpenCLInfo());

    registerProcessor(GrayscaleCL);
    registerProcessor(VolumeRaycasterCL);
    InviwoModule::initialize();
}

void OpenCLModule::deinitialize() {
    
}

} // namespace
