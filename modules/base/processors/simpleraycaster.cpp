#include "simpleraycaster.h"

namespace inviwo {

SimpleRaycaster::SimpleRaycaster()
    : ProcessorGL(),
    volumePort_(Port::INPORT, "volume"),
    entryPort_(Port::INPORT, "entry-points"),
    exitPort_(Port::INPORT, "exit-points"),
    outport_(Port::OUTPORT, "outport"),
    enableShading_("enableShading", "Shading", false),
    lightSourcePos_("lightSourcePos", "Light source position", vec3(1.0f), vec3(-1.0f), vec3(1.0f)),
	enableMIP_("enableMIP", "MIP", false),
    samplingRate_("samplingRate", "Sampling rate", 1.0f, 0.1f, 15.0f)
{
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    addProperty(samplingRate_);
    addProperty(enableShading_);
    addProperty(lightSourcePos_);
	addProperty(enableMIP_);
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
    uvec2 outportDim = outImageGL->getDimension();

    Volume* volume = volumePort_.getData();
    VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
    uvec3 volumeDim = volumeGL->getDimension();
    bindColorTexture(entryPort_, GL_TEXTURE0);
    bindColorTexture(exitPort_, GL_TEXTURE1);
    volumeGL->bindTexture(GL_TEXTURE2);
    activateTarget(outport_);
    shader_->activate();
    shader_->setUniform("entryTex_", 0);
    shader_->setUniform("exitTex_", 1);
    shader_->setUniform("volume_", 2);
    shader_->setUniform("dimension_", vec2(1.f/outportDim[0], 1.f/outportDim[1]));
    shader_->setUniform("samplingRate_", samplingRate_.get());
    shader_->setUniform("enableShading_", enableShading_.get());
    shader_->setUniform("lightSourcePos_", lightSourcePos_.get());
	shader_->setUniform("enableMIP_", enableMIP_.get());
    shader_->setUniform("volumeDimension_", vec3(volumeDim.x, volumeDim.y, volumeDim.z));
    renderImagePlaneQuad();
    shader_->deactivate();
    deactivateCurrentTarget();
    unbindColorTexture(entryPort_);
    unbindColorTexture(exitPort_);
    volumeGL->unbindTexture();
}

} // namespace
