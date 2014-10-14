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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include "drawfreehand.h"
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>
#include <inviwo/core/interaction/interactionhandler.h>

namespace inviwo {

ProcessorClassIdentifier(DrawFreeHand, "org.inviwo.DrawFreeHand");
ProcessorDisplayName(DrawFreeHand,  "Draw Freehand");
ProcessorTags(DrawFreeHand, Tags::GL);
ProcessorCategory(DrawFreeHand, "Drawing");
ProcessorCodeState(DrawFreeHand, CODE_STATE_EXPERIMENTAL);

DrawFreeHand::DrawFreeHand()
    : CompositeProcessorGL()
    , inport_("image.inport")
    , outport_("image.outport", &inport_, COLOR_ONLY) 
    , pointSize_("pointSize", "Point Size", 5, 1, 10)
    , pointColor_("pointColor", "Point Color", vec4(1.f))
    , clearButton_("clearButton", "Clear Drawing")
{
    addPort(inport_);
    addPort(outport_);

    addProperty(pointSize_);
    pointColor_.setSemantics(PropertySemantics::Color);
    addProperty(pointColor_);
    clearButton_.onChange(this, &DrawFreeHand::clearPoints);
    addProperty(clearButton_);

    addInteractionHandler(new DrawFreeHandInteractionHandler(this));
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
    pointShader_ = new Shader("img_color.frag");
    points_ = new Mesh(GeometryEnums::POINTS, GeometryEnums::NONE);
    points_->initialize();
    points_->addAttribute(new Position2dBuffer());
    pointRenderer_ = new MeshRenderer(points_);
}

void DrawFreeHand::deinitialize() {
    CompositeProcessorGL::deinitialize();
    delete pointShader_;
    pointShader_ = NULL;
    delete pointRenderer_;
    delete points_;
}

void DrawFreeHand::process() {
    utilgl::activateAndClearTarget(outport_);
    glPointSize(static_cast<float>(pointSize_.get()));
    pointShader_->activate();
    pointShader_->setUniform("color_", pointColor_.get());
    pointRenderer_->render();
    pointShader_->deactivate();
    glPointSize(1.f);
    utilgl::deactivateCurrentTarget();
    compositePortsToOutport(outport_, inport_);
}

void DrawFreeHand::addPoint(vec2 p) {
    points_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->add(p);
}

void DrawFreeHand::clearPoints() {
    points_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->clear();
}

DrawFreeHand::DrawFreeHandInteractionHandler::DrawFreeHandInteractionHandler(DrawFreeHand* dfh) 
    : InteractionHandler()
    , drawPosEvent(MouseEvent::MOUSE_BUTTON_LEFT, InteractionEvent::MODIFIER_CTRL)
    , drawEnableEvent_('D', InteractionEvent::MODIFIER_CTRL)
    , drawer_(dfh)
    , drawModeEnabled_(false) {
}

void DrawFreeHand::DrawFreeHandInteractionHandler::invokeEvent(Event* event){
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
                vec2 point = mouseEvent->posNormalized();
                point *= 2.f;
                point -= 1.f;
                point.y = -point.y;
                drawer_->addPoint(point);
                drawer_->invalidate(PropertyOwner::INVALID_OUTPUT);
        }
        return;
    }
}

} // inviwo namespace
