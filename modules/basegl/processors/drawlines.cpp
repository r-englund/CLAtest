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

#include "drawlines.h"
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>

namespace inviwo {

ProcessorClassIdentifier(DrawLines, "org.inviwo.DrawLines");
ProcessorDisplayName(DrawLines, "Draw Lines");
ProcessorTags(DrawLines, Tags::GL);
ProcessorCategory(DrawLines, "Drawing");
ProcessorCodeState(DrawLines, CODE_STATE_EXPERIMENTAL);

DrawLines::DrawLines()
    : CompositeProcessorGL()
    , inport_("inport")
    , outport_("outport", &inport_, COLOR_ONLY)
    , lineSize_("lineSize", "Line Size", 5, 1, 10)
    , lineColor_("lineColor", "Line Color", vec4(1.f))
    , clearButton_("clearButton", "Clear Lines") {
    addPort(inport_);
    addPort(outport_);

    addProperty(lineSize_);
    lineColor_.setSemantics(PropertySemantics::Color);
    addProperty(lineColor_);
    clearButton_.onChange(this, &DrawLines::clearLines);
    addProperty(clearButton_);

    addInteractionHandler(new DrawLinesInteractionHandler(this));
}

DrawLines::~DrawLines() {
    const std::vector<InteractionHandler*>& interactionHandlers = getInteractionHandlers();
    for (size_t i = 0; i < interactionHandlers.size(); ++i) {
        InteractionHandler* handler = interactionHandlers[i];
        removeInteractionHandler(handler);
        delete handler;
    }
}

void DrawLines::initialize() {
    CompositeProcessorGL::initialize();
    lineShader_ = new Shader("img_color.frag");
    lines_ = new Mesh(GeometryEnums::LINES, GeometryEnums::STRIP);
    lines_->addAttribute(new Position2dBuffer());
    lineRenderer_ = new MeshRenderer(lines_);
}

void DrawLines::deinitialize() {
    CompositeProcessorGL::deinitialize();
    delete lineShader_;
    lineShader_ = NULL;
    delete lineRenderer_;
    delete lines_;
}

void DrawLines::process() {
    utilgl::activateAndClearTarget(outport_);
    glLineWidth(static_cast<float>(lineSize_.get()));
    lineShader_->activate();
    lineShader_->setUniform("color_", lineColor_.get());
    lineRenderer_->render();
    glPointSize(1.f);
    utilgl::deactivateCurrentTarget();
    compositePortsToOutport(outport_, inport_);
}

void DrawLines::addPoint(vec2 p) {
    lines_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->add(p);
}

void DrawLines::clearLines() {
    lines_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->clear();
}

DrawLines::DrawLinesInteractionHandler::DrawLinesInteractionHandler(DrawLines* dfh)
    : InteractionHandler()
    , drawPosEvent(MouseEvent::MOUSE_BUTTON_LEFT, InteractionEvent::MODIFIER_NONE)
    , drawEnableEvent_('d', InteractionEvent::MODIFIER_CTRL)
    , drawer_(dfh)
    , drawModeEnabled_(false) {}

void DrawLines::DrawLinesInteractionHandler::invokeEvent(Event* event) {
    KeyboardEvent* keyEvent = dynamic_cast<KeyboardEvent*>(event);
    if (keyEvent) {
        int button = keyEvent->button();
        KeyboardEvent::KeyState state = keyEvent->state();
        InteractionEvent::Modifier modifier = keyEvent->modifier();

        if (button == 68 && modifier == drawEnableEvent_.modifier()) {
            if (state == KeyboardEvent::KEY_STATE_PRESS) {
                drawModeEnabled_ = true;
            } else if (state == KeyboardEvent::KEY_STATE_RELEASE) {
                //drawModeEnabled_ = false;
            }
        }
        return;
    }

    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
    if (drawModeEnabled_ && mouseEvent && (mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS ||
                                           mouseEvent->state() == MouseEvent::MOUSE_STATE_MOVE)) {
        if (mouseEvent->modifier() == drawPosEvent.modifier() &&
            mouseEvent->button() == drawPosEvent.button()) {
            vec2 line = mouseEvent->posNormalized();
            line *= 2.f;
            line -= 1.f;
            line.y = -line.y;
            drawer_->addPoint(line);
            drawer_->invalidate(PropertyOwner::INVALID_OUTPUT);
        }
        return;
    }
}

}  // namespace
