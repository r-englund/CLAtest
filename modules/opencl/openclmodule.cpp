#include "modules/opencl/openclmodule.h"
#include "modules/opencl/volumeclconverter.h"
#include "modules/opencl/inviwoopencl.h"

namespace inviwo {

OpenCLModule::OpenCLModule() : InviwoModule() {
    setIdentifier("OpenCL");
    setXMLFileName("opencl/openclmodule.xml");
    addRepresentationConverter(new VolumeDisk2CLConverter());
    addRepresentationConverter(new VolumeRAM2CLConverter());
    addRepresentationConverter(new VolumeCL2RAMConverter());
}

void OpenCLModule::initialize() {
    OpenCL::getInstance();
}

void OpenCLModule::deinitialize() {
    // Initialize OpenCL
    
}

} // namespace
