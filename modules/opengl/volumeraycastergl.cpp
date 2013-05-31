#include "volumeraycastergl.h"

namespace inviwo {

VolumeRaycasterGL::VolumeRaycasterGL()
    : ProcessorGL(),
      programFileName_("rc_simple.frag")
{}

VolumeRaycasterGL::VolumeRaycasterGL(std::string programFileName)
    : ProcessorGL(),
      programFileName_(programFileName)
{}

void VolumeRaycasterGL::initialize() {
    ProcessorGL::initialize();
    raycastPrg_ = new Shader(programFileName_);
}

void VolumeRaycasterGL::deinitialize() {
    delete raycastPrg_;
    raycastPrg_ = 0;
    ProcessorGL::deinitialize();
}


void VolumeRaycasterGL::initializeResources() {
    raycastPrg_->rebuild();
}


void VolumeRaycasterGL::bindVolume(const VolumeInport& inport, GLenum volTexUnit) {
    const Volume* volume = inport.getData();
    const VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
    //uvec3 volumeDim = volumeGL->getDimensions();
    volumeGL->bindTexture(volTexUnit);
}

} // namespace
