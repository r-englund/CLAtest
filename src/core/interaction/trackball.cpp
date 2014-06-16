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
 * Main file authors: Hans-Christian Helltegen, Rickard Englund
 *
 *********************************************************************************/

#include <inviwo/core/interaction/trackball.h>
#include <glm/gtx/vector_angle.hpp>

namespace inviwo {

static const float RADIUS = 0.5f;
static const float STEPSIZE = 0.05f;

Trackball::Trackball(CameraProperty* camera)
    : InteractionHandler()
    , PropertyOwner()
    , pixelWidth_(0.007f)
    , isMouseBeingPressedAndHold_(false)
    , lastMousePos_(ivec2(0))
    , lastTrackballPos_(vec3(0.5f))
    , camera_(camera)
    , rotateEvent_(MouseEvent::MOUSE_BUTTON_LEFT, InteractionEvent::MODIFIER_NONE)
    , zoomEvent_(MouseEvent::MOUSE_BUTTON_RIGHT, InteractionEvent::MODIFIER_NONE)
    , panEvent_(MouseEvent::MOUSE_BUTTON_MIDDLE, InteractionEvent::MODIFIER_NONE)
    , stepRotateUpEvent_('w',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , stepRotateLeftEvent_('a',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , stepRotateDownEvent_('s',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , stepRotateRightEvent_('d',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , stepZoomInEvent_('r',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , stepZoomOutEvent_('f',InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , stepPanUpEvent_('w',InteractionEvent::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS)
    , stepPanLeftEvent_('a',InteractionEvent::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS)
    , stepPanDownEvent_('s',InteractionEvent::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS)
    , stepPanRightEvent_('d',InteractionEvent::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS)

    , rotateAction_(TrackballAction::TRACKBALL_ROTATE)
    , zoomAction_(TrackballAction::TRACKBALL_ZOOM)
    , panAction_(TrackballAction::TRACKBALL_PAN)
    , stepRotateUpAction_(TrackballAction::TRACKBALL_STEPROTATE_UP)
    , stepRotateLeftAction_(TrackballAction::TRACKBALL_STEPROTATE_LEFT)
    , stepRotateDownAction_(TrackballAction::TRACKBALL_STEPROTATE_DOWN)
    , stepRotateRightAction_(TrackballAction::TRACKBALL_STEPROTATE_RIGHT)
    , stepZoomInAction_(TrackballAction::TRACKBALL_STEPZOOM_IN)
    , stepZoomOutAction_(TrackballAction::TRACKBALL_STEPZOOM_OUT)
    , stepPanUpAction_(TrackballAction::TRACKBALL_STEPPAN_UP)
    , stepPanLeftAction_(TrackballAction::TRACKBALL_STEPPAN_LEFT)
    , stepPanDownAction_(TrackballAction::TRACKBALL_STEPPAN_DOWN)
    , stepPanRightAction_(TrackballAction::TRACKBALL_STEPPAN_RIGHT)

    , rotateEventProperty_("trackballRotate", "Rotate", &rotateEvent_,  &rotateAction_)
    , zoomEventProperty_("trackballZoom", "Zoom", &zoomEvent_, &zoomAction_)
    , panEventProperty_("trackballPan", "Pan", &panEvent_, &panAction_)
    , stepRotateUpProperty_("stepRotateUp", "Step rotate up",           &stepRotateUpEvent_, &stepRotateUpAction_)
    , stepRotateLeftProperty_("stepRotateLeft", "Step rotate left",    &stepRotateLeftEvent_, &stepRotateLeftAction_)
    , stepRotateDownProperty_("stepRotateDown", "Step rotate down",    &stepRotateDownEvent_, &stepRotateDownAction_)
    , stepRotateRightProperty_("stepRotateRight", "Step rotate right", &stepRotateRightEvent_, &stepRotateRightAction_)
    , stepZoomInProperty_("stepZoomIn", "Step zoom in",                &stepZoomInEvent_, &stepZoomInAction_)
    , stepZoomOutProperty_("stepZoomOut", "Step zoom out",             &stepZoomOutEvent_, &stepZoomOutAction_)
    , stepPanUpProperty_("stepPanUp", "Step pan up",                   &stepPanUpEvent_, &stepPanUpAction_)
    , stepPanLeftProperty_("stepPanLeft", "Step pan left",             &stepPanLeftEvent_,&stepPanLeftAction_)
    , stepPanDownProperty_("stepPanDown", "Step pan down",             &stepPanDownEvent_,&stepPanDownAction_)
    , stepPanRightProperty_("stepPanRight", "Step pan right",          &stepPanRightEvent_,&stepPanRightAction_)
{
    rotateEventProperty_.setGroupID("Continuous movement");
    zoomEventProperty_.setGroupID("Continuous movement");
    panEventProperty_.setGroupID("Continuous movement");
    stepRotateUpProperty_.setGroupID("Step rotation");
    stepRotateLeftProperty_.setGroupID("Step rotation");
    stepRotateDownProperty_.setGroupID("Step rotation");
    stepRotateRightProperty_.setGroupID("Step rotation");
    stepZoomInProperty_.setGroupID("Step zooming");
    stepZoomOutProperty_.setGroupID("Step zooming");
    stepPanUpProperty_.setGroupID("Step panning");
    stepPanLeftProperty_.setGroupID("Step panning");
    stepPanDownProperty_.setGroupID("Step panning");
    stepPanRightProperty_.setGroupID("Step panning");
    addProperty(&rotateEventProperty_);
    addProperty(&zoomEventProperty_);
    addProperty(&panEventProperty_);
    addProperty(&stepRotateUpProperty_);
    addProperty(&stepRotateLeftProperty_);
    addProperty(&stepRotateDownProperty_);
    addProperty(&stepRotateRightProperty_);
    addProperty(&stepZoomInProperty_);
    addProperty(&stepZoomOutProperty_);
    addProperty(&stepPanUpProperty_);
    addProperty(&stepPanLeftProperty_);
    addProperty(&stepPanDownProperty_);
    addProperty(&stepPanRightProperty_);
}

Trackball::~Trackball() {}

vec3 Trackball::mapNormalizedMousePosToTrackball(vec2 mousePos) {
    // set x and y to lie in interval [-r, r]
    float r = RADIUS;
    vec3 result = vec3(mousePos.x-RADIUS, -1.0f*(mousePos.y-RADIUS), 0.0f);

    // Mapping according to Holroyds trackball
    //Piece-wise sphere + hyperbolic sheet
    if ((result.x*result.x + result.y*result.y) <= r*r/(2.0f)) {
        //Spherical Region
        result.z = r*r - (result.x*result.x + result.y*result.y);
        result.z = result.z > 0.0f ? sqrtf(result.z) : 0.0f;
    } else {
        //Hyperbolic Region - for smooth z values
        result.z = ((r*r)/(2.0f*sqrtf(result.x*result.x + result.y*result.y)));
    }

    return glm::normalize(result);
}

vec3 Trackball::mapToCamera(vec3 pos) {
    //return (camera_->viewMatrix() * vec4(pos,0)).xyz;
    //TODO: Use proper co-ordinate transformation matrices
    //Get x,y,z axis vectors of current camera view
    vec3 currentViewYaxis = glm::normalize(camera_->getLookUp());
    vec3 currentViewZaxis = glm::normalize(camera_->getLookFrom()-camera_->getLookTo());
    vec3 currentViewXaxis = glm::normalize(glm::cross(currentViewYaxis, currentViewZaxis));
    //mapping to camera co-ordinate
    currentViewXaxis*=pos.x;
    currentViewYaxis*=pos.y;
    currentViewZaxis*=pos.z;
    return (currentViewXaxis + currentViewYaxis + currentViewZaxis);
}


void Trackball::invokeEvent(Event* event) {
    GestureEvent* gestureEvent = dynamic_cast<GestureEvent*>(event);
    if (gestureEvent) {
        if(gestureEvent->type() == GestureEvent::PINCH){
            vec3 direction = camera_->getLookFrom() - camera_->getLookTo();
            camera_->setLookFrom(camera_->getLookFrom()-direction*static_cast<float>(gestureEvent->deltaDistance()));
        }
        else if(gestureEvent->type() == GestureEvent::PAN){
            vec3 offsetVector = vec3(gestureEvent->deltaPos(), 0.f);
            vec3 mappedOffsetVector = mapToCamera(offsetVector);

            camera_->lockInvalidation();
            camera_->setLookTo(camera_->getLookTo()     + mappedOffsetVector);
            camera_->setLookFrom(camera_->getLookFrom() + mappedOffsetVector);
            camera_->unlockInvalidation();
            camera_->invalidate();
        }

        return;
    }

    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
    if (mouseEvent) {
        int button = mouseEvent->button();
        MouseEvent::MouseState state = mouseEvent->state();
        InteractionEvent::Modifier modifier = mouseEvent->modifier();

        if (button == rotateEvent_.button()
            && modifier == rotateEvent_.modifier()
            && state == MouseEvent::MOUSE_STATE_PRESS) {
            //perform rotation
            rotateCamera(mouseEvent);
        } else if (button == zoomEvent_.button()
                   && modifier == zoomEvent_.modifier()
                   && state == MouseEvent::MOUSE_STATE_PRESS) {
            //perform zoom
            zoomCamera(mouseEvent);
        } else if (button == panEvent_.button()
                   && modifier == panEvent_.modifier()
                   && state == MouseEvent::MOUSE_STATE_PRESS) {
            //perform pan
            panCamera(mouseEvent);
        } else if (state == MouseEvent::MOUSE_STATE_RELEASE)
            isMouseBeingPressedAndHold_ = false;

        return;
    }

    KeyboardEvent* keyEvent = dynamic_cast<KeyboardEvent*>(event);
    if (keyEvent) {
        int button = keyEvent->button();
        KeyboardEvent::KeyState state = keyEvent->state();
        InteractionEvent::Modifier modifier = keyEvent->modifier();

        if (button == stepRotateUpEvent_.button()
            && modifier == stepRotateUpEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepRotateCamera(UP);
        else if (button == stepRotateLeftEvent_.button()
            && modifier == stepRotateLeftEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepRotateCamera(LEFT);
        else if (button == stepRotateDownEvent_.button()
            && modifier == stepRotateDownEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepRotateCamera(DOWN);
        else if (button == stepRotateRightEvent_.button()
            && modifier == stepRotateRightEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepRotateCamera(RIGHT);
        else if (button == stepZoomInEvent_.button()
            && modifier == stepZoomInEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepZoomCamera(UP);
        else if (button == stepZoomOutEvent_.button()
            && modifier == stepZoomOutEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepZoomCamera(DOWN);
        else if (button == stepPanUpEvent_.button()
            && modifier == stepPanUpEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepPanCamera(UP);
        else if (button == stepPanLeftEvent_.button()
            && modifier == stepPanLeftEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepPanCamera(LEFT);
        else if (button == stepPanDownEvent_.button()
            && modifier == stepPanDownEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepPanCamera(DOWN);
        else if (button == stepPanRightEvent_.button()
            && modifier == stepPanRightEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepPanCamera(RIGHT);

        return;
    }
}

void Trackball::rotateCamera(MouseEvent* mouseEvent) {
    ivwAssert(mouseEvent!=0, "Invalid mouse event.");
    // ROTATION
    vec2 curMousePos = mouseEvent->posNormalized();
    vec3 curTrackballPos = mapNormalizedMousePosToTrackball(curMousePos);
    float lookLength;

    // disable movements on first press
    if (!isMouseBeingPressedAndHold_) {
        lastTrackballPos_ = curTrackballPos;
        lastMousePos_ = curMousePos;
        isMouseBeingPressedAndHold_ = true;
    }

    if (curTrackballPos != lastTrackballPos_) {
        // calculate rotation angle (in radians)
        float rotationAngle = glm::angle(curTrackballPos, lastTrackballPos_);
        //difference vector in trackball co-ordinates
        vec3 trackBallOffsetVector = lastTrackballPos_ - curTrackballPos;
        //compute next camera position
        vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector);
        vec3 currentCamPos = camera_->getLookFrom();
        vec3 nextCamPos = currentCamPos + mappedTrackBallOffsetVector;

        // obtain rotation axis
        if (glm::degrees(rotationAngle) > pixelWidth_) {
            //rotation axis
            vec3 rotationAxis = glm::cross(currentCamPos, nextCamPos);
            // generate quaternion and rotate camera
            rotationAxis = glm::normalize(rotationAxis);
            quat quaternion = glm::angleAxis(rotationAngle, rotationAxis);
            lookLength = glm::length(camera_->getLookFrom()-camera_->getLookTo());
            vec3 offset = camera_->getLookTo();
            vec3 rotation = glm::rotate(quaternion, offset);
            camera_->lockInvalidation();
            camera_->setLookTo(rotation);
            camera_->setLookFrom(glm::rotate(quaternion, camera_->getLookFrom()));
            camera_->setLookUp(glm::rotate(quaternion, camera_->getLookUp()));

            // Check the length of the length-vector, might change due to float precision
            if (lookLength != glm::length(camera_->getLookFrom()-camera_->getLookTo())) {
                float diff = lookLength/glm::length(camera_->getLookFrom()-camera_->getLookTo());
                camera_->setLookTo(camera_->getLookTo()*diff);
            }

            camera_->unlockInvalidation();
            camera_->invalidate();
            //update mouse positions
            lastMousePos_ = curMousePos;
            lastTrackballPos_ = curTrackballPos;
        }
    }

    return;
}

void Trackball::zoomCamera(MouseEvent* mouseEvent) {
    ivwAssert(mouseEvent!=0, "Invalid mouse event.");
    // ZOOM
    float diff;
    vec2 curMousePos = mouseEvent->posNormalized();
    vec3 curTrackballPos = mapNormalizedMousePosToTrackball(curMousePos);
    // compute direction vector
    vec3 direction = camera_->getLookFrom() - camera_->getLookTo();

    // disable movements on first press
    if (!isMouseBeingPressedAndHold_) {
        lastMousePos_ = curMousePos;
        lastTrackballPos_ = curTrackballPos;
        isMouseBeingPressedAndHold_ = true;
    }

    if (curMousePos != lastMousePos_ && direction.length() > 0) {
        // use the difference in mouse y-position to determine amount of zoom
        diff = curTrackballPos.y - lastTrackballPos_.y;
        // zoom by moving the camera
        camera_->setLookFrom(camera_->getLookFrom()-direction*diff);
        lastMousePos_ = curMousePos;
        lastTrackballPos_ = curTrackballPos;
    }

    return;
}

void Trackball::panCamera(MouseEvent* mouseEvent) {
    ivwAssert(mouseEvent!=0, "Invalid mouse event.");
    // PAN
    vec2 curMousePos = mouseEvent->posNormalized();
    vec3 curTrackballPos = mapNormalizedMousePosToTrackball(curMousePos);

    // disable movements on first press
    if (!isMouseBeingPressedAndHold_) {
        lastMousePos_ = curMousePos;
        lastTrackballPos_ = curTrackballPos;
        isMouseBeingPressedAndHold_ = true;
    }

    //difference vector in trackball co-ordinates
    vec3 trackBallOffsetVector = lastTrackballPos_ - curTrackballPos;
    //compute next camera position
    trackBallOffsetVector.z = 0.0f;
    vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector);

    if (curMousePos != lastMousePos_) {
        camera_->lockInvalidation();
        camera_->setLookTo(camera_->getLookTo()     + mappedTrackBallOffsetVector);
        camera_->setLookFrom(camera_->getLookFrom() + mappedTrackBallOffsetVector);
        camera_->unlockInvalidation();
        camera_->invalidate();
        lastMousePos_ = curMousePos;
        lastTrackballPos_ = curTrackballPos;
    }

    return;
}

void Trackball::stepRotateCamera(Direction dir) {
    // STEP ROTATION
    vec2 origin = vec2(0.5, 0.5);
    vec2 direction = origin;
    float lookLength;

    switch (dir) {
        case UP:
            direction.y -= STEPSIZE;
            break;

        case LEFT:
            direction.x -= STEPSIZE;
            break;

        case DOWN:
            direction.y += STEPSIZE;
            break;

        case RIGHT:
            direction.x += STEPSIZE;
            break;
    }

    vec3 trackballDirection = mapNormalizedMousePosToTrackball(direction);
    vec3 trackballOrigin = mapNormalizedMousePosToTrackball(origin);
    // calculate rotation angle (in degrees)
    float rotationAngle = glm::angle(trackballDirection, trackballOrigin);
    //difference vector in trackball co-ordinates
    vec3 trackBallOffsetVector = trackballOrigin - trackballDirection;
    //compute next camera position
    vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector);
    vec3 currentCamPos = camera_->getLookFrom();
    vec3 nextCamPos = currentCamPos + mappedTrackBallOffsetVector;

    // obtain rotation axis
    if (glm::degrees(rotationAngle) > pixelWidth_) {
        //rotation axis
        vec3 rotationAxis = glm::cross(currentCamPos, nextCamPos);
        // generate quaternion and rotate camera
        rotationAxis = glm::normalize(rotationAxis);
        quat quaternion = glm::angleAxis(rotationAngle, rotationAxis);
        lookLength = glm::length(camera_->getLookFrom()-camera_->getLookTo());
        vec3 offset = camera_->getLookTo();
        vec3 rotation = glm::rotate(quaternion, offset);
        camera_->lockInvalidation();
        camera_->setLookTo(rotation);
        camera_->setLookFrom(glm::rotate(quaternion, camera_->getLookFrom()));
        camera_->setLookUp(glm::rotate(quaternion, camera_->getLookUp()));

        // Check the length of the length-vector, might change due to float precision
        if (lookLength != glm::length(camera_->getLookFrom()-camera_->getLookTo())) {
            float diff = lookLength/glm::length(camera_->getLookFrom()-camera_->getLookTo());
            camera_->setLookTo(camera_->getLookTo()*diff);
        }

        camera_->unlockInvalidation();
        camera_->invalidate();
    }

    return;
}

void Trackball::stepZoomCamera(Direction dir) {
    // ZOOM
    // compute direction vector
    vec3 direction = vec3(0);

    if (dir == UP)
        direction = camera_->getLookFrom() - camera_->getLookTo();
    else if (dir == DOWN)
        direction = camera_->getLookTo() - camera_->getLookFrom();

    // zoom by moving the camera
    camera_->setLookFrom(camera_->getLookFrom()-direction*STEPSIZE);
    return;
}

void Trackball::stepPanCamera(Direction dir) {
    // PAN
    vec2 origin = vec2(0.5, 0.5);
    vec2 direction = origin;

    switch (dir) {
        case UP:
            direction.y -= STEPSIZE;
            break;

        case LEFT:
            direction.x -= STEPSIZE;
            break;

        case DOWN:
            direction.y += STEPSIZE;
            break;

        case RIGHT:
            direction.x += STEPSIZE;
            break;
    }

    //vec2 curMousePos = mouseEvent->posNormalized();
    vec3 trackballDirection = mapNormalizedMousePosToTrackball(direction);
    vec3 trackballOrigin = mapNormalizedMousePosToTrackball(origin);
    //difference vector in trackball co-ordinates
    vec3 trackBallOffsetVector = trackballOrigin - trackballDirection;
    //compute next camera position
    trackBallOffsetVector.z = 0.0f;
    vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector);
    camera_->lockInvalidation();
    camera_->setLookTo(camera_->getLookTo()     + mappedTrackBallOffsetVector);
    camera_->setLookFrom(camera_->getLookFrom() + mappedTrackBallOffsetVector);
    camera_->unlockInvalidation();
    camera_->invalidate();
    return;
}

void Trackball::addProperty(Property& property) {
    if (dynamic_cast<EventProperty*>(&property))
        addProperty(&property);
}

void Trackball::addProperty(Property* property) {
    if (dynamic_cast<EventProperty*>(property)) {
        properties_.push_back(property);
        property->setOwner(this);
    }
}

void Trackball::serialize(IvwSerializer& s) const {
    InteractionHandler::serialize(s);
    PropertyOwner::serialize(s);
}

void Trackball::deserialize(IvwDeserializer& d) {
    InteractionHandler::deserialize(d);
    PropertyOwner::deserialize(d);
}

} // namespace