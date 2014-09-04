/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Timo Ropinski, Erik Sundén
 *
 *********************************************************************************/

#include "isoraycaster.h"
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <modules/opengl/shaderutils.h>

namespace inviwo {

ProcessorClassIdentifier(ISORaycaster, "org.inviwo.ISORaycaster");
ProcessorDisplayName(ISORaycaster, "ISO Raycaster");
ProcessorTags(ISORaycaster, Tags::GL);
ProcessorCategory(ISORaycaster, "Volume Rendering");
ProcessorCodeState(ISORaycaster, CODE_STATE_STABLE);

ISORaycaster::ISORaycaster()
    : ProcessorGL()
    , shader_(0)
    , volumePort_("volume")
    , entryPort_("entry-points")
    , exitPort_("exit-points")
    , outport_("outport", &entryPort_, COLOR_DEPTH)
    , channel_("channel", "Render Channel")
    , raycasting_("raycasting", "Raycasting")
    , lighting_("lighting", "Lighting")
    , camera_("camera", "Camera", vec3(0.0f, 0.0f, 3.5f), vec3(0.0f, 0.0f, 0.0f),
              vec3(0.0f, 1.0f, 0.0f)) {
              
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    volumePort_.onChange(this, &ISORaycaster::onVolumeChange);

    addProperty(channel_);
    addProperty(raycasting_);
    addProperty(camera_);
    addProperty(lighting_);


    std::stringstream ss;
    ss << "Channel " << 0;
    channel_.addOption(ss.str() , ss.str(), 0);

    raycasting_.compositingMode_.setVisible(false);
    setAllPropertiesCurrentStateAsDefault();
}


void ISORaycaster::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("isoraycasting.frag", false);
    initializeResources();
}

void ISORaycaster::deinitialize() {
    if (shader_) delete shader_;

    shader_ = 0;
    ProcessorGL::deinitialize();
}


void ISORaycaster::initializeResources(){
    
    addShaderDefines(shader_, raycasting_);
    addShaderDefines(shader_, camera_);
    addShaderDefines(shader_, lighting_);


    if (volumePort_.hasData()) {
        const int channels = volumePort_.getData()->getDataFormat()->getComponents();
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
    }

   
    shader_->build();
}
    
void ISORaycaster::onVolumeChange(){
    if(volumePort_.hasData()){
        int channels = volumePort_.getData()->getDataFormat()->getComponents();
        while(channels < static_cast<int>(channel_.size())){
            channel_.removeOption(static_cast<int>(channel_.size())-1);
        }
    }
}

void ISORaycaster::process() {
    int channels = volumePort_.getData()->getDataFormat()->getComponents();
    for(int i = static_cast<int>(channel_.size());i<channels;i++){
        std::stringstream ss;
        ss << "Channel " << i;
        channel_.addOption(ss.str() , ss.str(), i);
    }
    LGL_ERROR;
    TextureUnit entryColorUnit, entryDepthUnit, exitColorUnit, exitDepthUnit;
    bindTextures(entryPort_, entryColorUnit.getEnum(), entryDepthUnit.getEnum());
    bindTextures(exitPort_, exitColorUnit.getEnum(), exitDepthUnit.getEnum());

    TextureUnit volUnit;
    const Volume* volume = volumePort_.getData();
    const VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
    volumeGL->bindTexture(volUnit.getEnum());


    activateAndClearTarget(outport_);
    shader_->activate();
    setGlobalShaderParameters(shader_);
    
  
    shader_->setUniform("entryColorTex_", entryColorUnit.getUnitNumber());
    shader_->setUniform("entryDepthTex_", entryDepthUnit.getUnitNumber());
    setTextureParameters(entryPort_, shader_, "entryParameters_");
    shader_->setUniform("exitColorTex_", exitColorUnit.getUnitNumber());
    shader_->setUniform("exitDepthTex_", exitDepthUnit.getUnitNumber());
    setTextureParameters(exitPort_, shader_, "exitParameters_");

    shader_->setUniform("volume_", volUnit.getUnitNumber());
    volumeGL->setVolumeUniforms(volumePort_.getData(), shader_, "volumeParameters_");
    mat4 viewToTexture = volumePort_.getData()->getCoordinateTransformer().getWorldToTextureMatrix();
    shader_->setUniform("viewToTexture_", viewToTexture);


    shader_->setUniform("channel_", channel_.getSelectedValue());

    setShaderUniforms(shader_, raycasting_);
    setShaderUniforms(shader_, camera_);
    setShaderUniforms(shader_, lighting_);

    renderImagePlaneRect();
    shader_->deactivate();
    deactivateCurrentTarget();
}

} // namespace
