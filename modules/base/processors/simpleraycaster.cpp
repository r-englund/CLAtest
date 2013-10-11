#include "simpleraycaster.h"
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
    
    TextureUnit transFuncUnit;
    bindTransferFunction(transferFunction_.get(), transFuncUnit.getEnum());

    TextureUnit entryUnit, exitUnit;
    bindColorTexture(entryPort_, entryUnit.getEnum());
    bindColorTexture(exitPort_, exitUnit.getEnum());

    TextureUnit volUnit;
    bindVolume(volumePort_, volUnit.getEnum());

    activateAndClearTarget(outport_);
    uvec2 outportDim = outport_.getDimensions();
    
    raycastPrg_->activate();
    setGlobalShaderParameters(raycastPrg_);

    raycastPrg_->setUniform("transferFunc_", transFuncUnit.getUnitNumber());

    raycastPrg_->setUniform("entryTex_", entryUnit.getUnitNumber());
    setTextureParameters(entryPort_, raycastPrg_, "entryParameters_");
    raycastPrg_->setUniform("exitTex_", exitUnit.getUnitNumber());
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
