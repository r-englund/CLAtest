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

#include "positionwidgetprocessor.h"

#include <inviwo/core/interaction/pickingmanager.h>
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>
#include <modules/opengl/rendering/meshrenderer.h>

namespace inviwo {

ProcessorClassIdentifier(PositionWidgetProcessor,  "PositionWidget");
ProcessorDisplayName(PositionWidgetProcessor,  "Position Widget");
ProcessorTags(PositionWidgetProcessor, Tags::GL);
ProcessorCategory(PositionWidgetProcessor, "Geometry Rendering");
ProcessorCodeState(PositionWidgetProcessor, CODE_STATE_EXPERIMENTAL);

PositionWidgetProcessor::PositionWidgetProcessor()
    : CompositeProcessorGL()
    , geometryInport_("geometryInport")
    , imageInport_("imageInport")
    , outport_("outport", COLOR_DEPTH_PICKING)
    , position_("position", "Position", vec3(0.0f), vec3(-100.f), vec3(100.f))
    , camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
    , handleInteractionEvents_("handleEvents", "Handle interaction events", true)
{
    addPort(geometryInport_);
    addPort(imageInport_);
    addPort(outport_);
    addProperty(position_);
    position_.setVisible(false);

    addProperty(camera_);
    trackball_ = new CameraTrackball(&camera_);
    addInteractionHandler(trackball_);
    addProperty(handleInteractionEvents_);
    handleInteractionEvents_.onChange(this, &PositionWidgetProcessor::handleInteractionEventsChanged);
}

PositionWidgetProcessor::~PositionWidgetProcessor() {
    const std::vector<InteractionHandler*>& interactionHandlers = getInteractionHandlers();
    for(size_t i=0; i<interactionHandlers.size(); ++i) {
        InteractionHandler* handler = interactionHandlers[i];
        removeInteractionHandler(handler);
        delete handler;
    }
}

void PositionWidgetProcessor::initialize() {
    CompositeProcessorGL::initialize();
    shader_ = new Shader("standard.vert", "picking.frag");
    widgetPickingObject_ = PickingManager::instance()->registerPickingCallback(this, &PositionWidgetProcessor::updateWidgetPositionFromPicking);
}

void PositionWidgetProcessor::deinitialize() {
    CompositeProcessorGL::deinitialize();
    delete shader_;
    shader_ = NULL;
    PickingManager::instance()->unregisterPickingObject(widgetPickingObject_);
}

void PositionWidgetProcessor::updateWidgetPositionFromPicking(const PickingObject* p) {
    vec2 move = p->getPickingMove();

    if (move.x == 0.f && move.y == 0.f) return;

    vec2 pos = p->getPickingPosition();
    float depth = p->getPickingDepth();
    //TODO: do we not need to incorporate transformations here?
    vec3 startNdc = vec3((2.f*pos.x)-1.f, (2.f*pos.y)-1.f, depth);
    vec3 endNdc = vec3((2.f*(pos.x+move.x))-1.f, (2.f*(pos.y+move.y))-1.f, depth);
    vec3 startWorld = camera_.getWorldPosFromNormalizedDeviceCoords(startNdc);
    vec3 endWorld = camera_.getWorldPosFromNormalizedDeviceCoords(endNdc);
    position_.set(position_.get() + (endWorld-startWorld));
    invalidate(INVALID_OUTPUT);
}

void PositionWidgetProcessor::process() {
    activateAndClearTarget(outport_);
    MeshRenderer renderer(static_cast<const Mesh*>(geometryInport_.getData()));
    shader_->activate();
    shader_->setUniform("pickingColor_", widgetPickingObject_->getPickingColor());

    mat4 modelViewProjectionMatrix_ = 
        camera_.projectionMatrix()
        * camera_.viewMatrix()
        * glm::translate(position_.get())
        * geometryInport_.getData()->getWorldTransform()
        * geometryInport_.getData()->getBasisAndOffset();
    shader_->setUniform("modelViewProjectionMatrix_", modelViewProjectionMatrix_);

    glDepthFunc(GL_ALWAYS);
    renderer.render();
    glDepthFunc(GL_LESS);

    shader_->deactivate();
    deactivateCurrentTarget();
    compositePortsToOutport(outport_, imageInport_);
}

void PositionWidgetProcessor::handleInteractionEventsChanged() {
    if (handleInteractionEvents_.get()) {
        addInteractionHandler(trackball_);
    } else {
        removeInteractionHandler(trackball_);
    }
}

} // namespace
