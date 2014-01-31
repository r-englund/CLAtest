/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include "lightingraycaster.h"
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassName(LightingRaycaster, "LightingRaycaster"); 
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
    if(enableLightColor_.get())
        raycastPrg_->getFragmentShaderObject()->addShaderDefine("LIGHT_COLOR_ENABLED");
    else
        raycastPrg_->getFragmentShaderObject()->removeShaderDefine("LIGHT_COLOR_ENABLED");

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

    raycastPrg_->setUniform("lightVolume_", lightVolUnit.getUnitNumber());
    setVolumeParameters(lightVolumePort_, raycastPrg_, "lightVolumeParameters_");

    raycastPrg_->setUniform("samplingRate_", samplingRate_.get());
    raycastPrg_->setUniform("isoValue_", isoValue_.get());

    renderImagePlaneRect();
    
    raycastPrg_->deactivate();
    deactivateCurrentTarget();
}

} // namespace
