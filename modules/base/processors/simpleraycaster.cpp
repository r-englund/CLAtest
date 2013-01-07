#include "simpleraycaster.h"

namespace inviwo {

SimpleRaycaster::SimpleRaycaster()
    : ProcessorGL(),
    volumePort_(Port::INPORT, "volume"),
    entryPort_(Port::INPORT, "entry-points"),
    exitPort_(Port::INPORT, "exit-points"),
    outport_(Port::OUTPORT, "outport"),
    enableShading_("enableShading", "Shading", true),
    samplingRate_("samplingRate", "Sampling rate", 1.0f, 0.1f, 15.0f)
{
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    addProperty(enableShading_);
    addProperty(samplingRate_);
}

SimpleRaycaster::~SimpleRaycaster() {}

Processor* SimpleRaycaster::create() const {
    return new SimpleRaycaster();
}

void SimpleRaycaster::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("raycasting.frag");
}

void SimpleRaycaster::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void SimpleRaycaster::process() {
    
    Image* outImage = outport_.getData();
    ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    ivec2 outportDim = outImageGL->size();

    Volume* volume = volumePort_.getData();
    VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
    bindColorTexture(entryPort_, GL_TEXTURE0);
    bindColorTexture(exitPort_, GL_TEXTURE1);
    volumeGL->bindTexture(GL_TEXTURE2);
    activateTarget(outport_);
    shader_->activate();
    shader_->setUniform("entryTex_", 0);
    shader_->setUniform("exitTex_", 1);
    shader_->setUniform("volume_", 2);
    shader_->setUniform("dimension_", vec2(1.f/outportDim[0], 1.f/outportDim[1]));
    shader_->setUniform("enableShading_", enableShading_.get());
    shader_->setUniform("samplingRate_", samplingRate_.get());
    renderImagePlaneQuad();
    shader_->deactivate();
    deactivateCurrentTarget();
    unbindColorTexture(entryPort_);
    unbindColorTexture(exitPort_);
    volumeGL->unbindTexture();
}

} // namespace
