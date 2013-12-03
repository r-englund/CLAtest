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

#include "positionwidgetprocessor.h"
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/interaction/pickingmanager.h>
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>
#include <modules/opengl/geometry/meshglrenderer.h>

namespace inviwo {

ProcessorClassName(PositionWidgetProcessor, "PositionWidget"); 
ProcessorCategory(PositionWidgetProcessor, "Geometry Rendering");
ProcessorCodeState(PositionWidgetProcessor, CODE_STATE_EXPERIMENTAL); 

PositionWidgetProcessor::PositionWidgetProcessor()
    : CompositeProcessorGL(),
      geometryInport_("geometryInport"),
      imageInport_("imageInport"),
      outport_("outport", COLOR_DEPTH_PICKING),
      position_("position", "Position", vec3(0.0f), vec3(-100.f), vec3(100.f)),
      camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
{
    addPort(geometryInport_);
    addPort(imageInport_);
    addPort(outport_);

    addProperty(position_);
    position_.setVisible(false);
    addProperty(camera_);
    addInteractionHandler(new Trackball(&camera_));

    widgetPickingObject_ = PickingManager::instance()->registerPickingCallback(this, &PositionWidgetProcessor::updateWidgetPositionFromPicking);
}

PositionWidgetProcessor::~PositionWidgetProcessor() {}

void PositionWidgetProcessor::initialize() {
    CompositeProcessorGL::initialize();

    program_ = new Shader("picking.frag");
}

void PositionWidgetProcessor::deinitialize() {
    CompositeProcessorGL::deinitialize();

    delete program_;
    program_ = NULL;
}

void PositionWidgetProcessor::updateWidgetPositionFromPicking(){
    vec2 move = widgetPickingObject_->getPickingMove();
    if (move.x == 0.f && move.y == 0.f) return;

	vec2 pos = widgetPickingObject_->getPickingPosition();
    float depth = widgetPickingObject_->getPickingDepth();
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

    MeshGLRenderer renderer(static_cast<const Mesh*>(geometryInport_.getData()));

    program_->activate();
    program_->setUniform("pickingColor_", widgetPickingObject_->getPickingColor());

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()*glm::translate(position_.get())));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    renderer.render();

    glDepthFunc(GL_LESS);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glLoadIdentity();

    program_->deactivate();

    deactivateCurrentTarget();

    compositePortsToOutport(outport_, imageInport_);
}

} // namespace
