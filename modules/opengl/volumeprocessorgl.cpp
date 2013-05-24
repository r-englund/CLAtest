#include "volumeprocessorgl.h"

namespace inviwo {

VolumeProcessorGL::VolumeProcessorGL()
    : ProcessorGL()
{}
VolumeProcessorGL::~VolumeProcessorGL() {}

Processor* VolumeProcessorGL::create() const {
    return new VolumeProcessorGL();
}

void VolumeProcessorGL::initialize() {
    ProcessorGL::initialize();
}

void VolumeProcessorGL::deinitialize() {
    ProcessorGL::deinitialize();
}

} // namespace
