#include "multichannelraycaster.h"
#include <modules/opengl/shaderutils.h>
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

ProcessorClassName(MultichannelRaycaster, "MultichannelRaycaster");
ProcessorCategory(MultichannelRaycaster, "Volume Rendering");
ProcessorCodeState(MultichannelRaycaster, CODE_STATE_EXPERIMENTAL);

MultichannelRaycaster::MultichannelRaycaster()
    : ProcessorGL()
    , shader_(NULL)
    , shaderFileName_("multichannlraycaster.frag")
    , volumePort_("volume")
    , entryPort_("entry-points")
    , exitPort_("exit-points")
    , outport_("outport", &entryPort_, COLOR_DEPTH)
    , transferFunctions_(NULL)
    , raycasting_("raycaster", "Raycasting")
    , camera_("camera", "Camera")
    , lighting_("lighting", "Lighting") {

    transferFunctions_.push_back(new TransferFunctionProperty(
        "transferFunction1", "Channel 1", TransferFunction(), &volumePort_));
    transferFunctions_.push_back(new TransferFunctionProperty(
        "transferFunction2", "Channel 2", TransferFunction(), &volumePort_));
    transferFunctions_.push_back(new TransferFunctionProperty(
        "transferFunction3", "Channel 3", TransferFunction(), &volumePort_));
    transferFunctions_.push_back(new TransferFunctionProperty(
        "transferFunction4", "Channel 4", TransferFunction(), &volumePort_));

    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    volumePort_.onChange(this, &MultichannelRaycaster::onVolumeChange);

    addProperty(raycasting_);
    addProperty(camera_);
    addProperty(lighting_);

    for (std::vector<TransferFunctionProperty*>::iterator tf = transferFunctions_.begin();
         tf != transferFunctions_.end(); ++tf) {
        addProperty(*tf);
        (*tf)->setGroupID("multichannlraycaster-tfs");
        Property::setGroupDisplayName("multichannlraycaster-tfs", "Transfer functions");
    }
    
}

void MultichannelRaycaster::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader(shaderFileName_, false);
    initializeResources();
}

void MultichannelRaycaster::deinitialize() {
    if (shader_) delete shader_;
    shader_ = NULL;
    ProcessorGL::deinitialize();
}

void MultichannelRaycaster::initializeResources() {
    addShaderDefines(shader_, raycasting_);
    addShaderDefines(shader_, camera_);
    addShaderDefines(shader_, lighting_);

    if (volumePort_.hasData()) {
        const int channels = volumePort_.getData()->getDataFormat()->getComponents();
        std::stringstream ss;
        ss << channels;
        shader_->getFragmentShaderObject()->addShaderDefine("NUMBER_OF_CHANNELS", ss.str());
    }

    shader_->build();
}

void MultichannelRaycaster::onVolumeChange() {
    if (volumePort_.hasData()) {
        initializeResources();
        int channels = volumePort_.getData()->getDataFormat()->getComponents();

        for (int i = 0; i < transferFunctions_.size(); i++) {
            transferFunctions_[i]->setVisible(i < channels ? true : false);
        }
    }
}

void MultichannelRaycaster::process() {   
    LGL_ERROR;
    TextureUnit entryColorUnit, entryDepthUnit, exitColorUnit, exitDepthUnit;
    bindTextures(entryPort_, entryColorUnit.getEnum(), entryDepthUnit.getEnum());
    bindTextures(exitPort_, exitColorUnit.getEnum(), exitDepthUnit.getEnum());

    TextureUnit volUnit;
    const Volume* volume = volumePort_.getData();
    const VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
    volumeGL->bindTexture(volUnit.getEnum());

    const int channels = volume->getDataFormat()->getComponents();

    TextureUnit* transFuncUnits = new TextureUnit[channels];
    GLint* tfUnitNumbers = new GLint[channels];

    for (int channel = 0; channel < channels; channel++) {
        const Layer* tfLayer = transferFunctions_[channel]->get().getData();
        const LayerGL* transferFunctionGL = tfLayer->getRepresentation<LayerGL>();
        transferFunctionGL->bindTexture(transFuncUnits[channel].getEnum());
        tfUnitNumbers[channel] = transFuncUnits[channel].getUnitNumber();
    }
    
    activateAndClearTarget(outport_);
    shader_->activate();
    setGlobalShaderParameters(shader_);
    shader_->setUniform("channels_", channels);
    shader_->setUniform("transferFuncs_", tfUnitNumbers, channels);
    shader_->setUniform("entryColorTex_", entryColorUnit.getUnitNumber());
    shader_->setUniform("entryDepthTex_", entryDepthUnit.getUnitNumber());
    setTextureParameters(entryPort_, shader_, "entryParameters_");
    shader_->setUniform("exitColorTex_", exitColorUnit.getUnitNumber());
    shader_->setUniform("exitDepthTex_", exitDepthUnit.getUnitNumber());
    setTextureParameters(exitPort_, shader_, "exitParameters_");
    shader_->setUniform("volume_", volUnit.getUnitNumber());
    volumeGL->setVolumeUniforms(volume, shader_, "volumeParameters_");
    shader_->setUniform("viewToTexture_",
                        volume->getCoordinateTransformer().getWorldToTextureMatrix());

    setShaderUniforms(shader_, raycasting_);
    setShaderUniforms(shader_, camera_);
    setShaderUniforms(shader_, lighting_);
    
    renderImagePlaneRect();
    shader_->deactivate();
    deactivateCurrentTarget();
    LGL_ERROR;

    delete[] transFuncUnits;
    delete[] tfUnitNumbers;
}

}  // namespace
