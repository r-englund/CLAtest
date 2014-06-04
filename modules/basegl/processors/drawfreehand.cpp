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

#include "drawfreehand.h"
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

namespace inviwo {

ProcessorClassName(DrawFreeHand, "DrawFreeHand");
ProcessorCategory(DrawFreeHand, "Image Operation");
ProcessorCodeState(DrawFreeHand, CODE_STATE_EXPERIMENTAL);

DrawFreeHand::DrawFreeHand()
    : CompositeProcessorGL()
    , inport_("image.inport")
    , outport_("image.outport", &inport_, COLOR_ONLY) 
{
    addPort(inport_);
    addPort(outport_);

    addInteractionHandler(new DrawFreeHandInteractationHandler(this));
}

DrawFreeHand::~DrawFreeHand() {
    const std::vector<InteractionHandler*>& interactionHandlers = getInteractionHandlers();
    for(size_t i=0; i<interactionHandlers.size(); ++i) {
        InteractionHandler* handler = interactionHandlers[i];
        removeInteractionHandler(handler);
        delete handler;
    }
}

void DrawFreeHand::initialize() {
    CompositeProcessorGL::initialize();
    points_ = new Mesh(GeometryEnums::POINTS, GeometryEnums::NONE);
    points_->initialize();
    points_->addAttribute(new Position2dBuffer());
    pointRenderer_ = new MeshRenderer(points_);
}

void DrawFreeHand::deinitialize() {
    CompositeProcessorGL::deinitialize();
    delete pointRenderer_;
    delete points_;
}

void DrawFreeHand::process() {
    activateAndClearTarget(outport_);
    glPointSize(8.f);
    pointRenderer_->render();
    glPointSize(1.f);
    deactivateCurrentTarget();
    compositePortsToOutport(outport_, inport_);
}

void DrawFreeHand::addPoint(vec2 p) {
    points_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->add(p);
}

void DrawFreeHand::clearPoints() {
    points_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->clear();
}

DrawFreeHand::DrawFreeHandInteractationHandler::DrawFreeHandInteractationHandler(DrawFreeHand* dfh) 
    : InteractionHandler()
    , drawPosEvent(MouseEvent::MOUSE_BUTTON_LEFT, InteractionEvent::MODIFIER_CTRL)
    , drawEnableEvent_('D', InteractionEvent::MODIFIER_CTRL)
    , drawer_(dfh)
    , drawModeEnabled_(false) {
}

void DrawFreeHand::DrawFreeHandInteractationHandler::invokeEvent(Event* event){
    KeyboardEvent* keyEvent = dynamic_cast<KeyboardEvent*>(event);
    if (keyEvent) {
        int button = keyEvent->button();
        KeyboardEvent::KeyState state = keyEvent->state();
        InteractionEvent::Modifier modifier = keyEvent->modifier();

        if (button == drawEnableEvent_.button() && modifier == drawEnableEvent_.modifier()){
            if(state == KeyboardEvent::KEY_STATE_PRESS){
                drawModeEnabled_ = true;
            }
            else if(state == KeyboardEvent::KEY_STATE_RELEASE){
                drawModeEnabled_ = false;
            }
        }
        return;
    }

    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
    if (drawModeEnabled_ && mouseEvent) {
        if (mouseEvent->modifier() == drawPosEvent.modifier()
            && mouseEvent->button() == drawPosEvent.button()) {
                drawer_->addPoint(mouseEvent->posNormalized());
                drawer_->invalidate(PropertyOwner::INVALID_OUTPUT);
        }
        return;
    }
}

} // inviwo namespace
