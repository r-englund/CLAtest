/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include "lightingraycaster.h"
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassIdentifier(LightingRaycaster, "org.inviwo.LightingRaycaster");
ProcessorDisplayName(LightingRaycaster,  "Lighting Raycaster");
ProcessorTags(LightingRaycaster, Tags::GL);
ProcessorCategory(LightingRaycaster, "Volume Rendering");
ProcessorCodeState(LightingRaycaster, CODE_STATE_EXPERIMENTAL);

LightingRaycaster::LightingRaycaster()
    : VolumeRaycasterGL("lighting/lightingraycasting.frag"),
      volumePort_("volume"),
      entryPort_("entry-points"),
      exitPort_("exit-points"),
      lightVolumePort_("lightVolume"),
      outport_("outport", &entryPort_, COLOR_DEPTH),
      transferFunction_("transferFunction", "Transfer function", TransferFunction(), &volumePort_),
      enableLightColor_("supportColoredLight", "Enable Light Color", false, PropertyOwner::INVALID_RESOURCES)
{
    addPort(volumePort_);
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(lightVolumePort_);
    addPort(outport_, "ImagePortGroup1");
    addProperty(transferFunction_);
    addProperty(enableLightColor_);
    enableLightColor_.setGroupID("lighting");
    addProperty(isoValue_);
    addShadingProperties();
}

void LightingRaycaster::initializeResources() {
    if (enableLightColor_.get())
        shader_->getFragmentShaderObject()->addShaderDefine("LIGHT_COLOR_ENABLED");
    else
        shader_->getFragmentShaderObject()->removeShaderDefine("LIGHT_COLOR_ENABLED");

    VolumeRaycasterGL::initializeResources();
}

void LightingRaycaster::process() {
    TextureUnit transFuncUnit;
    bindTransferFunction(transferFunction_.get(), transFuncUnit.getEnum());
    TextureUnit entryColorUnit, entryDepthUnit, exitColorUnit, exitDepthUnit;
    bindTextures(entryPort_, entryColorUnit.getEnum(), entryDepthUnit.getEnum());
    bindTextures(exitPort_, exitColorUnit.getEnum(), exitDepthUnit.getEnum());
    TextureUnit volUnit;
    bindVolume(volumePort_, volUnit.getEnum());
    TextureUnit lightVolUnit;
    bindVolume(lightVolumePort_, lightVolUnit.getEnum());
    activateAndClearTarget(outport_);
    shader_->activate();
    setGlobalShaderParameters(shader_);
    shader_->setUniform("transferFunc_", transFuncUnit.getUnitNumber());
    shader_->setUniform("entryColorTex_", entryColorUnit.getUnitNumber());
    shader_->setUniform("entryDepthTex_", entryDepthUnit.getUnitNumber());
    setTextureParameters(entryPort_, shader_, "entryParameters_");
    shader_->setUniform("exitColorTex_", exitColorUnit.getUnitNumber());
    shader_->setUniform("exitDepthTex_", exitDepthUnit.getUnitNumber());
    setTextureParameters(exitPort_, shader_, "exitParameters_");
    shader_->setUniform("volume_", volUnit.getUnitNumber());
    setVolumeParameters(volumePort_, shader_, "volumeParameters_");
    shader_->setUniform("lightVolume_", lightVolUnit.getUnitNumber());
    setVolumeParameters(lightVolumePort_, shader_, "lightVolumeParameters_");
    shader_->setUniform("samplingRate_", samplingRate_.get());
    shader_->setUniform("isoValue_", isoValue_.get());
    renderImagePlaneRect();
    shader_->deactivate();
    deactivateCurrentTarget();
}

} // namespace
