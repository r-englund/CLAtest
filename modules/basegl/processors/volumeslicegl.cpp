/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
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

#include "volumeslicegl.h"
#include <modules/opengl/volume/volumegl.h>

namespace inviwo {

ProcessorClassName(VolumeSliceGL, "VolumeSliceGL"); 
ProcessorCategory(VolumeSliceGL, "Volume Operation");
ProcessorCodeState(VolumeSliceGL, CODE_STATE_STABLE);

VolumeSliceGL::VolumeSliceGL()
    : ProcessorGL(),
    inport_("volume.inport"),
    outport_("image.outport", COLOR_ONLY),
    coordinatePlane_("coordinatePlane", "Coordinate Plane"),
    sliceNumber_("sliceNumber", "Slice Number", 1, 1, 256),
    transferFunction_("transferFunction", "Transfer function", TransferFunction(), &inport_),
    shader_(NULL)
{
    addPort(inport_);
    addPort(outport_);

    coordinatePlane_.addOption("xy", "XY Plane", XY);
    coordinatePlane_.addOption("xz", "XZ Plane", XZ);
    coordinatePlane_.addOption("yz", "YZ Plane", YZ);
    coordinatePlane_.set(XY);
    coordinatePlane_.onChange(this, &VolumeSliceGL::coordinatePlaneChanged);
    addProperty(coordinatePlane_);

    addProperty(sliceNumber_);

    addProperty(transferFunction_);

    volumeDimensions_ = uvec3(256);
}

VolumeSliceGL::~VolumeSliceGL() {}

void VolumeSliceGL::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("volumeslice.frag", false);
    coordinatePlaneChanged();
    volumeDimensionChanged();
}

void VolumeSliceGL::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void VolumeSliceGL::process(){
    if (volumeDimensions_ != inport_.getData()->getDimension()) {
        volumeDimensions_ = inport_.getData()->getDimension();
        volumeDimensionChanged();
    }

    TextureUnit volUnit;
    const VolumeGL* volumeGL = inport_.getData()->getRepresentation<VolumeGL>();
    volumeGL->bindTexture(volUnit.getEnum());

    TextureUnit transFuncUnit;
    const Layer* tfLayer = transferFunction_.get().getData();
    const LayerGL* transferFunctionGL = tfLayer->getRepresentation<LayerGL>();
    transferFunctionGL->bindTexture(transFuncUnit.getEnum());

    float sliceNum = static_cast<float>(sliceNumber_.get())/glm::max<float>(static_cast<float>(sliceNumber_.getMaxValue()-1), 1.f);

    activateAndClearTarget(outport_);

    shader_->activate();
    setGlobalShaderParameters(shader_);

    shader_->setUniform("volume_", volUnit.getUnitNumber());
    vec3 dimensions = vec3(volumeGL->getDimension());
    shader_->setUniform("volumeParameters_.dimensions_", dimensions);
    shader_->setUniform("volumeParameters_.dimensionsRCP_", vec3(1.0f)/dimensions);

    shader_->setUniform("dimension_", vec2(1.f / outport_.getDimension()[0], 1.f / outport_.getDimension()[1]));
    shader_->setUniform("transferFunc_", transFuncUnit.getUnitNumber());
    shader_->setUniform("sliceNum_", sliceNum);

    renderImagePlaneRect();

    shader_->deactivate();

    deactivateCurrentTarget();
}

void VolumeSliceGL::coordinatePlaneChanged(){
    if (shader_) {
        switch(coordinatePlane_.get())
        {
        case XY:
            shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)", "x,y,z");
            break;
        case XZ:
            shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)", "x,z,y");
            break;
        case YZ:
            shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)", "y,z,x");
            break;
        }
        shader_->rebuild();
    }
    invalidate(INVALID_OUTPUT);
}

void VolumeSliceGL::volumeDimensionChanged(){
    switch (coordinatePlane_.get()) {
    case XY:
        sliceNumber_.setMaxValue(static_cast<int>(volumeDimensions_.z));
        break;
    case XZ:
        sliceNumber_.setMaxValue(static_cast<int>(volumeDimensions_.y));
        break;
    case YZ:
        sliceNumber_.setMaxValue(static_cast<int>(volumeDimensions_.x));
        break;
    }
}

} // inviwo namespace
