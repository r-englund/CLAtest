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

namespace inviwo {

ProcessorClassName(DrawFreeHand, "DrawFreeHand");
ProcessorCategory(DrawFreeHand, "Image Operation");
ProcessorCodeState(DrawFreeHand, CODE_STATE_EXPERIMENTAL);

DrawFreeHand::DrawFreeHand()
    : ProcessorGL()
    , inport_("image.inport")
    , outport_("image.outport", &inport_, COLOR_ONLY)
    , shader_(NULL) {
      
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
    ProcessorGL::initialize();
    shader_ = new Shader("img_texturequad.vert", "img_texturequad.frag", true);
}

void DrawFreeHand::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void DrawFreeHand::process() {
    activateAndClearTarget(outport_);
    shader_->activate();
    setGlobalShaderParameters(shader_);
    renderImagePlaneRect();
    shader_->deactivate();
    deactivateCurrentTarget();
}

void DrawFreeHand::drawPoint(vec2){

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
                drawer_->drawPoint(mouseEvent->posNormalized());
        }
        return;
    }
}

} // inviwo namespace
