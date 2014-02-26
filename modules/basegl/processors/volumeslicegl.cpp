/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
      flipHorizontal_("flipHorizontal", "Flip Horizontal View", false),
      flipVertical_("flipVertical", "Flip Vertical View", false),
      sliceNumber_("sliceNumber", "Slice Number", 1, 1, 256),
      tfMappingEnabled_("tfMappingEnabled", "Enable Transfer Function", true),
      transferFunction_("transferFunction", "Transfer function", TransferFunction(), &inport_),
      shader_(NULL)
{
    addPort(inport_);
    addPort(outport_);
    coordinatePlane_.addOption("xy", "XY Plane", XY);
    coordinatePlane_.addOption("xz", "XZ Plane", XZ);
    coordinatePlane_.addOption("yz", "YZ Plane", YZ);
    coordinatePlane_.set(XY);
    coordinatePlane_.onChange(this, &VolumeSliceGL::planeSettingsChanged);
    addProperty(coordinatePlane_);
    flipHorizontal_.onChange(this, &VolumeSliceGL::planeSettingsChanged);
    addProperty(flipHorizontal_);
    flipVertical_.onChange(this, &VolumeSliceGL::planeSettingsChanged);
    addProperty(flipVertical_);
    addProperty(sliceNumber_);
    tfMappingEnabled_.onChange(this, &VolumeSliceGL::tfMappingEnabledChanged);
    addProperty(tfMappingEnabled_);
    addProperty(transferFunction_);
    volumeDimensions_ = uvec3(256);
    addInteractionHandler(new VolumeSliceGLInteractationHandler(this));
}

VolumeSliceGL::~VolumeSliceGL() {}

void VolumeSliceGL::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("volumeslice.frag", false);
    planeSettingsChanged();
    tfMappingEnabledChanged();
    volumeDimensionChanged();
}

void VolumeSliceGL::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void VolumeSliceGL::process() {
    if (volumeDimensions_ != inport_.getData()->getDimension()) {
        volumeDimensions_ = inport_.getData()->getDimension();
        volumeDimensionChanged();
    }

    TextureUnit transFuncUnit;
    const Layer* tfLayer = transferFunction_.get().getData();
    const LayerGL* transferFunctionGL = tfLayer->getRepresentation<LayerGL>();
    transferFunctionGL->bindTexture(transFuncUnit.getEnum());
    TextureUnit volUnit;
    const VolumeGL* volumeGL = inport_.getData()->getRepresentation<VolumeGL>();
    volumeGL->bindTexture(volUnit.getEnum());
    activateAndClearTarget(outport_);
    shader_->activate();
    setGlobalShaderParameters(shader_);

    if (tfMappingEnabled_.get())
        shader_->setUniform("transferFunc_", transFuncUnit.getUnitNumber());

    shader_->setUniform("volume_", volUnit.getUnitNumber());
    volumeGL->setVolumeUniforms(inport_.getData(), shader_, "volumeParameters_");
    shader_->setUniform("dimension_", vec2(1.0f/outport_.getData()->getDimension().x, 1.0f/outport_.getData()->getDimension().y));
    float sliceNum = (static_cast<float>(sliceNumber_.get())-0.5f)/glm::max<float>(static_cast<float>(sliceNumber_.getMaxValue()), 1.f);
    shader_->setUniform("sliceNum_", sliceNum);
    renderImagePlaneRect();
    shader_->deactivate();
    deactivateCurrentTarget();
}

void VolumeSliceGL::shiftSlice(int shift){
    int newSlice = sliceNumber_.get()+shift;
    if(newSlice >= sliceNumber_.getMinValue() && newSlice <= sliceNumber_.getMaxValue())
        sliceNumber_.set(newSlice);
}

void VolumeSliceGL::planeSettingsChanged() {
    std::string fH = (flipHorizontal_.get() ? "1.0-" : "");
    std::string fV = (flipVertical_.get() ? "1.0-" : "");

    if (shader_) {
        switch (coordinatePlane_.get())
        {
            case XY:
                shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)", fH + "x," + fV + "y,z");
                break;

            case XZ:
                shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)", fH + "x,z," + fV + "y");
                break;

            case YZ:
                shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)", fV + "y,z," + fH + "x");
                break;
        }

        shader_->rebuild();
    }
}

void VolumeSliceGL::tfMappingEnabledChanged() {
    if (shader_) {
        if (tfMappingEnabled_.get()) {
            shader_->getFragmentShaderObject()->addShaderDefine("TF_MAPPING_ENABLED");
            transferFunction_.setVisible(true);
        }
        else {
            shader_->getFragmentShaderObject()->removeShaderDefine("TF_MAPPING_ENABLED");
            transferFunction_.setVisible(false);
        }

        shader_->rebuild();
    }
}

void VolumeSliceGL::volumeDimensionChanged() {
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

VolumeSliceGL::VolumeSliceGLInteractationHandler::VolumeSliceGLInteractationHandler(VolumeSliceGL* vs) 
    : InteractionHandler()
    , wheelEvent_(MouseEvent::MOUSE_BUTTON_NONE, InteractionEvent::MODIFIER_NONE)
    , upEvent_('W',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , downEvent_('S',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , slicer_(vs) {
}

void VolumeSliceGL::VolumeSliceGLInteractationHandler::invokeEvent(Event* event){
    KeyboardEvent* keyEvent = dynamic_cast<KeyboardEvent*>(event);
    if (keyEvent) {
        int button = keyEvent->button();
        KeyboardEvent::KeyState state = keyEvent->state();
        InteractionEvent::Modifier modifier = keyEvent->modifier();

        if (button == upEvent_.button()
            && modifier == upEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            slicer_->shiftSlice(1);
        else if (button == downEvent_.button()
            && modifier == downEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            slicer_->shiftSlice(-1);

        return;
    }

    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
    if (mouseEvent) {
        MouseEvent::MouseState state = mouseEvent->state();
        InteractionEvent::Modifier modifier = mouseEvent->modifier();

        if (modifier == wheelEvent_.modifier()
            && state == MouseEvent::MOUSE_STATE_WHEEL) {
                int steps = mouseEvent->wheelSteps();
                slicer_->shiftSlice(steps);
        }
        return;
    }
}

} // inviwo namespace
