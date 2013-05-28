#include "volumeraycastergl.h"

namespace inviwo {

VolumeRaycasterGL::VolumeRaycasterGL()
    : ProcessorGL()
{}
VolumeRaycasterGL::~VolumeRaycasterGL() {}

void VolumeRaycasterGL::initialize() {
    ProcessorGL::initialize();
}

void VolumeRaycasterGL::deinitialize() {
    ProcessorGL::deinitialize();
}

} // namespace
