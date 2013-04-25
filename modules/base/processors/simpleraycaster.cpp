#include "simpleraycaster.h"

namespace inviwo {

SimpleRaycaster::SimpleRaycaster()
    : ProcessorGL(),
    volumePort_("volume"),
    entryPort_("entry-points"),
    exitPort_("exit-points"),
    outport_("outport"),
    enableShading_("enableShading", "Shading", false),
    lightSourcePos_("lightSourcePos", "Light source position", vec3(1.0f), vec3(-1.0f), vec3(1.0f)),
	enableMIP_("enableMIP", "MIP", false),
    samplingRate_("samplingRate", "Sampling rate", 1.0f, 0.1f, 10.0f),
    transferFunction_("transferFunction", "Transfer function", TransferFunction())
{
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    addProperty(samplingRate_);
    addProperty(enableShading_);
    addProperty(lightSourcePos_);
	addProperty(enableMIP_);
    addProperty(transferFunction_);
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
    const Volume* volume = volumePort_.getData();
    const VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
    uvec3 volumeDim = volumeGL->getDimensions();
    bindColorTexture(entryPort_, GL_TEXTURE0);
    bindColorTexture(exitPort_, GL_TEXTURE1);
    volumeGL->bindTexture(GL_TEXTURE2);
    const ImageGL* transferFunctionGL = transferFunction_.get().getData()->getRepresentation<ImageGL>();
    transferFunctionGL->bindColorTexture(GL_TEXTURE3);
    activateTarget(outport_);
    Image* outImage = outport_.getData();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    uvec2 outportDim = outImageGL->getDimension();
    shader_->activate();
    shader_->setUniform("entryTex_", 0);
    shader_->setUniform("exitTex_", 1);
    shader_->setUniform("volume_", 2);
    shader_->setUniform("transferFunction_", 3);
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
    transferFunctionGL->unbindColorTexture();
}

} // namespace
