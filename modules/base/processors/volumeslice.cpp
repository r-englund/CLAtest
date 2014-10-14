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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include "volumeslice.h"

namespace inviwo {

ProcessorClassIdentifier(VolumeSlice, "org.inviwo.VolumeSlice");
ProcessorDisplayName(VolumeSlice,  "Volume Slice");
ProcessorTags(VolumeSlice, Tags::None);
ProcessorCategory(VolumeSlice, "Volume Operation");
ProcessorCodeState(VolumeSlice, CODE_STATE_STABLE);

VolumeSlice::VolumeSlice()
    : Processor(),
      inport_("volume.inport"),
      outport_("image.outport", COLOR_ONLY),
      sliceAlongAxis_("sliceAxis", "Slice along axis"),
      sliceNumber_("sliceNumber", "Slice Number", 4, 1, 8)
{
    addPort(inport_);
    addPort(outport_);
    sliceAlongAxis_.addOption("x", "X axis", CoordinateEnums::X);
    sliceAlongAxis_.addOption("y", "Y axis", CoordinateEnums::Y);
    sliceAlongAxis_.addOption("z", "Z axis", CoordinateEnums::Z);
    sliceAlongAxis_.setSelectedIndex(0);
    sliceAlongAxis_.setCurrentStateAsDefault();
    addProperty(sliceAlongAxis_);
    addProperty(sliceNumber_);
    addInteractionHandler(new VolumeSliceInteractionHandler(this));
}

VolumeSlice::~VolumeSlice() {
    const std::vector<InteractionHandler*>& interactionHandlers = getInteractionHandlers();
    for(size_t i=0; i<interactionHandlers.size(); ++i) {
        InteractionHandler* handler = interactionHandlers[i];
        removeInteractionHandler(handler);
        delete handler;
    }
}

void VolumeSlice::initialize() {
    Processor::initialize();
}

void VolumeSlice::deinitialize() {
    Processor::deinitialize();
}

void VolumeSlice::shiftSlice(int shift){
    int newSlice = sliceNumber_.get()+shift;
    if(newSlice >= sliceNumber_.getMinValue() && newSlice <= sliceNumber_.getMaxValue())
        sliceNumber_.set(newSlice);
}

void VolumeSlice::process() {
    uvec3 dims = inport_.getData()->getDimension();

    switch (sliceAlongAxis_.get())
    {
        case CoordinateEnums::X:
            if(dims.x!=sliceNumber_.getMaxValue()){
                sliceNumber_.setMaxValue(static_cast<int>(dims.x));
                sliceNumber_.set(static_cast<int>(dims.x)/2);
            }
            break;
        case CoordinateEnums::Y:
            if(dims.y!=sliceNumber_.getMaxValue()){
                sliceNumber_.setMaxValue(static_cast<int>(dims.y));
                sliceNumber_.set(static_cast<int>(dims.y)/2);
            }
            break;
        case CoordinateEnums::Z:
            if(dims.z!=sliceNumber_.getMaxValue()){
                sliceNumber_.setMaxValue(static_cast<int>(dims.z));
                sliceNumber_.set(static_cast<int>(dims.z)/2);
            }
            break;
    }

    const VolumeRAM* vol = inport_.getData()->getRepresentation<VolumeRAM>();
    LayerRAM* sliceImage = VolumeRAMSlice::apply(vol, static_cast<CoordinateEnums::CartesianCoordinateAxis>(sliceAlongAxis_.get()), static_cast<unsigned int>(sliceNumber_.get()-1));

    Image* outImage = new Image(sliceImage->getDimension(), COLOR_ONLY, sliceImage->getDataFormat());
    outImage->getColorLayer()->addRepresentation(sliceImage);

    outport_.setData(outImage);
}

VolumeSlice::VolumeSliceInteractionHandler::VolumeSliceInteractionHandler(VolumeSlice* vs) 
: InteractionHandler()
, wheelEvent_(MouseEvent::MOUSE_BUTTON_NONE, InteractionEvent::MODIFIER_NONE)
, upEvent_('W',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
, downEvent_('S',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
, slicer_(vs) {
}

void VolumeSlice::VolumeSliceInteractionHandler::invokeEvent(Event* event){
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
