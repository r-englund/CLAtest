/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include "simpleraycaster.h"
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassName(SimpleRaycaster, "SimpleRaycaster"); 
ProcessorCategory(SimpleRaycaster, "Volume Rendering");
ProcessorCodeState(SimpleRaycaster, CODE_STATE_EXPERIMENTAL);

SimpleRaycaster::SimpleRaycaster()
    : VolumeRaycasterGL("raycasting.frag"),
    volumePort_("volume"),
    entryPort_("entry-points"),
    exitPort_("exit-points"),
    outport_("outport", &entryPort_, COLOR_DEPTH),
    transferFunction_("transferFunction", "Transfer function", TransferFunction())
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
    ivwAssert(entryPort_.getData()!=0, "Entry port empty.");
    ivwAssert(exitPort_.getData()!=0, "Exit port empty.");

    // FIXME: enable onCHange for ports and put the following line there
    transferFunction_.setVolume(volumePort_.getData());
    
    TextureUnit transFuncUnit;
    bindTransferFunction(transferFunction_.get(), transFuncUnit.getEnum());

    TextureUnit entryColorUnit, entryDepthUnit, exitColorUnit, exitDepthUnit;
    bindTextures(entryPort_, entryColorUnit.getEnum(), entryDepthUnit.getEnum());
    bindTextures(exitPort_, exitColorUnit.getEnum(), exitDepthUnit.getEnum());

    TextureUnit volUnit;
    bindVolume(volumePort_, volUnit.getEnum());

    activateAndClearTarget(outport_);
    uvec2 outportDim = outport_.getDimensions();
    
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

    raycastPrg_->setUniform("dimension_", vec2(1.f/outportDim[0], 1.f/outportDim[1]));
    raycastPrg_->setUniform("samplingRate_", samplingRate_.get());
    raycastPrg_->setUniform("isoValue_", isoValue_.get());

    renderImagePlaneRect();
    
    raycastPrg_->deactivate();
    deactivateCurrentTarget();
}

} // namespace
