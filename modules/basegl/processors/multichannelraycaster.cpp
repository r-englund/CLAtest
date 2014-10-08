#include "multichannelraycaster.h"
#include <modules/opengl/shaderutils.h>
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/image/layergl.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <modules/opengl/textureutils.h>
#include <modules/opengl/volumeutils.h>
#include <inviwo/core/datastructures/volume/volume.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

ProcessorClassIdentifier(MultichannelRaycaster, "org.inviwo.MultichannelRaycaster");
ProcessorDisplayName(MultichannelRaycaster,  "Multichannel Raycaster");
ProcessorTags(MultichannelRaycaster, Tags::GL);
ProcessorCategory(MultichannelRaycaster, "Volume Rendering");
ProcessorCodeState(MultichannelRaycaster, CODE_STATE_EXPERIMENTAL);

MultichannelRaycaster::MultichannelRaycaster()
    : Processor()
    , shader_(NULL)
    , shaderFileName_("multichannlraycaster.frag")
    , volumePort_("volume")
    , entryPort_("entry-points")
    , exitPort_("exit-points")
    , outport_("outport", &entryPort_, COLOR_DEPTH)
    , transferFunctions_("transfer-functions", "Transfer functions")
    , raycasting_("raycaster", "Raycasting")
    , camera_("camera", "Camera")
    , lighting_("lighting", "Lighting") {

    transferFunctions_.addProperty(new TransferFunctionProperty(
        "transferFunction1", "Channel 1", TransferFunction(), &volumePort_));
    transferFunctions_.addProperty(new TransferFunctionProperty(
        "transferFunction2", "Channel 2", TransferFunction(), &volumePort_));
    transferFunctions_.addProperty(new TransferFunctionProperty(
        "transferFunction3", "Channel 3", TransferFunction(), &volumePort_));
    transferFunctions_.addProperty(new TransferFunctionProperty(
        "transferFunction4", "Channel 4", TransferFunction(), &volumePort_));

    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    addProperty(raycasting_);
    addProperty(camera_);
    addProperty(lighting_);

    addProperty(transferFunctions_);
    
    volumePort_.onChange(this, &MultichannelRaycaster::initializeResources);
}


MultichannelRaycaster::~MultichannelRaycaster() {
    std::vector<Property*> tfs = transferFunctions_.getProperties();
    while (!tfs.empty()) {
        delete tfs.back();
        tfs.pop_back();
    }
}

void MultichannelRaycaster::initialize() {
    Processor::initialize();
    shader_ = new Shader(shaderFileName_, false);
    initializeResources();
}

void MultichannelRaycaster::deinitialize() {
    if (shader_) delete shader_;
    shader_ = NULL;
    Processor::deinitialize();
}

void MultichannelRaycaster::initializeResources() {
    utilgl::addShaderDefines(shader_, raycasting_);
    utilgl::addShaderDefines(shader_, camera_);
    utilgl::addShaderDefines(shader_, lighting_);

    if (volumePort_.hasData()) {
        const int channels = volumePort_.getData()->getDataFormat()->getComponents();
        
        std::vector<Property*> tfs = transferFunctions_.getProperties();
        for (int i = 0; i < static_cast<int>(tfs.size()); i++) {
            tfs[i]->setVisible(i < channels ? true : false);
        }
        
        std::stringstream ss;
        ss << channels;
        shader_->getFragmentShaderObject()->addShaderDefine("NUMBER_OF_CHANNELS", ss.str());

        std::stringstream ss2;
        for (int i = 0; i < channels; ++i) {
            ss2 << "gradient = RC_CALC_GRADIENTS_FOR_CHANNEL(voxel, samplePos, volume_,"
             << " volumeParameters_, t, rayDirection, entryTex_, entryParameters_," << i << ");"
            
             << "color = RC_APPLY_CLASSIFICATION_FOR_CHANNEL(transferFuncs_[" << i
             << "], voxel, " << i << ")"
            
             << "color.rgb = RC_APPLY_SHADING(color.rgb, color.rgb, vec3(1.0), samplePos,"
             << " gradient, lightPosition_, vec3(0.0));"
            
             << "result = RC_APPLY_COMPOSITING(result, color, samplePos, voxel, gradient,"
             << " t, tDepth, tIncr);";
        }
        shader_->getFragmentShaderObject()->addShaderDefine("SAMPLE_CHANNELS", ss2.str());
        shader_->build();
    }

}

void MultichannelRaycaster::process() {   
    LGL_ERROR;
    std::vector<Property*> tfs = transferFunctions_.getProperties();
    TextureUnit entryColorUnit, entryDepthUnit, exitColorUnit, exitDepthUnit;
    utilgl::bindTextures(entryPort_, entryColorUnit.getEnum(), entryDepthUnit.getEnum());
    utilgl::bindTextures(exitPort_, exitColorUnit.getEnum(), exitDepthUnit.getEnum());

    TextureUnit volUnit;
    const Volume* volume = volumePort_.getData();
    const VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
    volumeGL->bindTexture(volUnit.getEnum());

    const int channels = volume->getDataFormat()->getComponents();

    TextureUnit* transFuncUnits = new TextureUnit[channels];
    GLint* tfUnitNumbers = new GLint[channels];

    for (int channel = 0; channel < channels; channel++) {
        const Layer* tfLayer = static_cast<TransferFunctionProperty*>(tfs[channel])->get().getData();
        const LayerGL* transferFunctionGL = tfLayer->getRepresentation<LayerGL>();
        transferFunctionGL->bindTexture(transFuncUnits[channel].getEnum());
        tfUnitNumbers[channel] = transFuncUnits[channel].getUnitNumber();
    }
    
    utilgl::activateAndClearTarget(outport_);
    shader_->activate();
    
    utilgl::setShaderUniforms(shader_, outport_, "outportParameters_");
    
    shader_->setUniform("transferFuncs_", tfUnitNumbers, channels);

    shader_->setUniform("entryColorTex_", entryColorUnit.getUnitNumber());
    shader_->setUniform("entryDepthTex_", entryDepthUnit.getUnitNumber());
    utilgl::setShaderUniforms(shader_, entryPort_, "entryParameters_");

    shader_->setUniform("exitColorTex_", exitColorUnit.getUnitNumber());
    shader_->setUniform("exitDepthTex_", exitDepthUnit.getUnitNumber());
    utilgl::setShaderUniforms(shader_, exitPort_, "exitParameters_");
 
    
    shader_->setUniform("viewToTexture_",
                        volume->getCoordinateTransformer().getWorldToTextureMatrix());
 
    shader_->setUniform("volume_", volUnit.getUnitNumber());
    utilgl::setShaderUniforms(shader_, volume, "volumeParameters_");
    utilgl::setShaderUniforms(shader_, raycasting_);
    utilgl::setShaderUniforms(shader_, camera_);
    utilgl::setShaderUniforms(shader_, lighting_);
    
    utilgl::singleDrawImagePlaneRect();
    shader_->deactivate();
    utilgl::deactivateCurrentTarget();
    LGL_ERROR;

    delete[] transFuncUnits;
    delete[] tfUnitNumbers;
}


}  // namespace
