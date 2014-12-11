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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include "drawpoints.h"
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>
#include <inviwo/core/interaction/interactionhandler.h>

namespace inviwo {

ProcessorClassIdentifier(DrawPoints, "org.inviwo.DrawPoints");
ProcessorDisplayName(DrawPoints,  "Draw Points");
ProcessorTags(DrawPoints, Tags::GL);
ProcessorCategory(DrawPoints, "Drawing");
ProcessorCodeState(DrawPoints, CODE_STATE_STABLE);

DrawPoints::DrawPoints()
    : CompositeProcessorGL()
    , inport_("image.inport")
    , outport_("image.outport", &inport_, COLOR_ONLY) 
    , pointSize_("pointSize", "Point Size", 5, 1, 10)
    , pointColor_("pointColor", "Point Color", vec4(1.f))
    , clearButton_("clearButton", "Clear Drawing")
    , points_(NULL)
    , pointRenderer_(NULL)
    , pointShader_(NULL)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(pointSize_);
    pointColor_.setSemantics(PropertySemantics::Color);
    addProperty(pointColor_);
    clearButton_.onChange(this, &DrawPoints::clearPoints);
    addProperty(clearButton_);

    addInteractionHandler(new DrawPointsInteractionHandler(this));
}

DrawPoints::~DrawPoints() {
    const std::vector<InteractionHandler*>& interactionHandlers = getInteractionHandlers();
    for(size_t i=0; i<interactionHandlers.size(); ++i) {
        InteractionHandler* handler = interactionHandlers[i];
        removeInteractionHandler(handler);
        delete handler;
    }
}

void DrawPoints::initialize() {
    CompositeProcessorGL::initialize();
    pointShader_ = new Shader("img_color.frag");
    points_ = new Mesh(GeometryEnums::POINTS, GeometryEnums::NONE);
    points_->addAttribute(new Position2dBuffer());
    pointRenderer_ = new MeshRenderer(points_);
}

void DrawPoints::deinitialize() {
    CompositeProcessorGL::deinitialize();
    delete pointShader_;
    pointShader_ = NULL;
    delete pointRenderer_;
    pointRenderer_ = NULL;
    delete points_;
    pointRenderer_ = NULL;
}

void DrawPoints::process() {
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

void DrawPoints::addPoint(vec2 p) {
    if (points_)
        points_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->add(p);
}

void DrawPoints::clearPoints() {
    if (points_)
        points_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->clear();
}

DrawPoints::DrawPointsInteractionHandler::DrawPointsInteractionHandler(DrawPoints* dfh) 
    : InteractionHandler()
    , drawPosEvent(MouseEvent::MOUSE_BUTTON_LEFT, InteractionEvent::MODIFIER_CTRL)
    , drawEnableEvent_('D', InteractionEvent::MODIFIER_CTRL)
    , drawer_(dfh)
    , drawModeEnabled_(false) {
}

void DrawPoints::DrawPointsInteractionHandler::invokeEvent(Event* event){
    KeyboardEvent* keyEvent = dynamic_cast<KeyboardEvent*>(event);
    if (keyEvent) {
        int button = keyEvent->button();
        int state = keyEvent->state();
        int modifier = keyEvent->modifiers();

        if (button == drawEnableEvent_.button() && modifier == drawEnableEvent_.modifiers()){
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
        if (mouseEvent->modifiers() == drawPosEvent.modifiers()
            && mouseEvent->button() == drawPosEvent.button()) {
                vec2 point = mouseEvent->posNormalized();
                point *= 2.f;
                point -= 1.f;
                point.y = -point.y;
                drawer_->addPoint(point);
                drawer_->invalidate(INVALID_OUTPUT);
        }
        return;
    }
}

} // inviwo namespace
