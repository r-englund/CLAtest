#include <modules/opencl/imageclconverter.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmodule.h>
#include <modules/opencl/openclinfo.h>
#include <modules/opencl/processors/volumeraycastercl.h>
#include <modules/opencl/volumeclconverter.h>




namespace inviwo {

OpenCLModule::OpenCLModule() : InviwoModule() {
    setIdentifier("OpenCL");
    setXMLFileName("opencl/openclmodule.xml");
    addRepresentationConverter(new ImageDisk2CLConverter());
    addRepresentationConverter(new ImageRAM2CLConverter());
    addRepresentationConverter(new ImageCL2RAMConverter());
    addRepresentationConverter(new ImageGL2CLConverter());

    addRepresentationConverter(new VolumeDisk2CLConverter());
    addRepresentationConverter(new VolumeRAM2CLConverter());
    addRepresentationConverter(new VolumeCL2RAMConverter());
    
    addResourceInfo(new OpenCLInfo());

    addProcessor(new VolumeRaycasterCL());
}

void OpenCLModule::initialize() {
    OpenCL::getInstance();
    InviwoModule::initialize();
}

void OpenCLModule::deinitialize() {
    // Initialize OpenCL
    
}

} // namespace
