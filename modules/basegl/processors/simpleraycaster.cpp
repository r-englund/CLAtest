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

#include "simpleraycaster.h"
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassName(SimpleRaycaster, "SimpleRaycaster"); 
ProcessorCategory(SimpleRaycaster, "Volume Rendering");
ProcessorCodeState(SimpleRaycaster, CODE_STATE_STABLE);

SimpleRaycaster::SimpleRaycaster()
    : VolumeRaycasterGL("raycasting.frag"),
    volumePort_("volume"),
    entryPort_("entry-points"),
    exitPort_("exit-points"),
    outport_("outport", &entryPort_, COLOR_DEPTH),
    transferFunction_("transferFunction", "Transfer function", TransferFunction(), &volumePort_)
{
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");

    addProperty(transferFunction_);

    addProperty(isoValue_);

    addShadingProperties();
}

void SimpleRaycaster::process() {
    TextureUnit transFuncUnit;
    bindTransferFunction(transferFunction_.get(), transFuncUnit.getEnum());

    TextureUnit entryColorUnit, entryDepthUnit, exitColorUnit, exitDepthUnit;
    bindTextures(entryPort_, entryColorUnit.getEnum(), entryDepthUnit.getEnum());
    bindTextures(exitPort_, exitColorUnit.getEnum(), exitDepthUnit.getEnum());

    TextureUnit volUnit;
    bindVolume(volumePort_, volUnit.getEnum());

    activateAndClearTarget(outport_);
    
    raycastPrg_->activate();
    setGlobalShaderParameters(raycastPrg_);

    raycastPrg_->setUniform("transferFunc_", transFuncUnit.getUnitNumber());

    raycastPrg_->setUniform("entryColorTex_", entryColorUnit.getUnitNumber());
    raycastPrg_->setUniform("entryDepthTex_", entryDepthUnit.getUnitNumber());
    setTextureParameters(entryPort_, raycastPrg_, "entryParameters_");
    raycastPrg_->setUniform("exitColorTex_", exitColorUnit.getUnitNumber());
    raycastPrg_->setUniform("exitDepthTex_", exitDepthUnit.getUnitNumber());
    setTextureParameters(exitPort_, raycastPrg_, "exitParameters_");

    raycastPrg_->setUniform("volume_", volUnit.getUnitNumber());
    setVolumeParameters(volumePort_, raycastPrg_, "volumeParameters_");

    raycastPrg_->setUniform("samplingRate_", samplingRate_.get());
    raycastPrg_->setUniform("isoValue_", isoValue_.get());

    renderImagePlaneRect();
    
    raycastPrg_->deactivate();
    deactivateCurrentTarget();
}

} // namespace
