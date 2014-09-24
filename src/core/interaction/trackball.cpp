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

Trackball::Trackball(vec3* lookFrom, vec3* lookTo, vec3* lookUp)
    : InteractionHandler()
    , PropertyOwner()
    , pixelWidth_(0.007f)
    , isMouseBeingPressedAndHold_(false)
    , lastMousePos_(ivec2(0))
    , lastTrackballPos_(vec3(0.5f))
    , lookFrom_(lookFrom)
    , lookTo_(lookTo)
    , lookUp_(lookUp)

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
//    rotateEventProperty_.setGroupID("Continuous movement");
//    zoomEventProperty_.setGroupID("Continuous movement");
//    panEventProperty_.setGroupID("Continuous movement");
//    stepRotateUpProperty_.setGroupID("Step rotation");
//    stepRotateLeftProperty_.setGroupID("Step rotation");
//    stepRotateDownProperty_.setGroupID("Step rotation");
//    stepRotateRightProperty_.setGroupID("Step rotation");
//    stepZoomInProperty_.setGroupID("Step zooming");
//    stepZoomOutProperty_.setGroupID("Step zooming");
//    stepPanUpProperty_.setGroupID("Step panning");
//    stepPanLeftProperty_.setGroupID("Step panning");
//    stepPanDownProperty_.setGroupID("Step panning");
//    stepPanRightProperty_.setGroupID("Step panning");
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

vec3 Trackball::mapNormalizedMousePosToTrackball(const vec2& mousePos, float dist /*= 1.f*/)
{
    // set x and y to lie in interval [-r, r]
    float r = RADIUS;
    vec3 result = vec3(mousePos.x-RADIUS, -1.0f*(mousePos.y-RADIUS), 0.0f)*dist;

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

vec3 Trackball::mapToObject(vec3 pos, float dist) {
    //return (camera_->viewMatrix() * vec4(pos,0)).xyz;
    //TODO: Use proper co-ordinate transformation matrices
    //Get x,y,z axis vectors of current camera view
    vec3 currentViewYaxis = glm::normalize(*lookUp_);
    vec3 currentViewZaxis = glm::normalize(*lookFrom_-*lookTo_);
    vec3 currentViewXaxis = glm::normalize(glm::cross(currentViewYaxis, currentViewZaxis));

    //mapping to camera co-ordinate
    currentViewXaxis*=pos.x*dist;
    currentViewYaxis*=pos.y*dist;
    currentViewZaxis*=pos.z*dist;
    return (currentViewXaxis + currentViewYaxis + currentViewZaxis);
}


void Trackball::invokeEvent(Event* event) {
    GestureEvent* gestureEvent = dynamic_cast<GestureEvent*>(event);
    if (gestureEvent) {
        if(gestureEvent->type() == GestureEvent::PINCH && gestureEvent->numFingers() == 2){
            vec3 direction = *lookFrom_ - *lookTo_;
            float vecLength = glm::clamp(glm::length(direction), 0.5f, 4.f);
            vec3 normdirection = glm::normalize(direction);
            *lookFrom_ = *lookFrom_-normdirection*(static_cast<float>(vecLength*gestureEvent->deltaDistance()));
            notifyLookFromChanged(this);
        }
        else if(gestureEvent->type() == GestureEvent::PAN && gestureEvent->numFingers() == 3){
            vec3 offsetVector = vec3(gestureEvent->deltaPos().x*2.f, gestureEvent->deltaPos().y*2.f, 0.f);

            //The resulting rotation needs to be mapped to the camera distance,
            //as if the trackball is located at a certain distance from the camera.
            //TODO: Verify this
            float zDist = (glm::length(*lookFrom_-*lookTo_)-1.f)/M_PI;
            vec3 mappedOffsetVector = mapToObject(offsetVector, zDist);

            *lookTo_ += mappedOffsetVector;
            *lookFrom_ += mappedOffsetVector;
            notifyAllChanged(this);

        }
        isMouseBeingPressedAndHold_ = false;

        gestureEvent->markAsUsed();

        return;
    }

    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
    if (mouseEvent) {
        int button = mouseEvent->button();
        MouseEvent::MouseState state = mouseEvent->state();
        InteractionEvent::Modifier modifier = mouseEvent->modifier();

        if (button == rotateEvent_.button()
            && modifier == rotateEvent_.modifier()
            && (state == MouseEvent::MOUSE_STATE_MOVE || state == MouseEvent::MOUSE_STATE_PRESS)) {
                //perform rotation
                rotate(mouseEvent);
        } else if (button == zoomEvent_.button()
            && modifier == zoomEvent_.modifier()
            && (state == MouseEvent::MOUSE_STATE_MOVE || state == MouseEvent::MOUSE_STATE_PRESS)) {
                //perform zoom
                zoom(mouseEvent);
        } else if (button == panEvent_.button()
            && modifier == panEvent_.modifier()
            && (state == MouseEvent::MOUSE_STATE_MOVE || state == MouseEvent::MOUSE_STATE_PRESS)) {
                //perform pan
                pan(mouseEvent);
        } else if (state == MouseEvent::MOUSE_STATE_RELEASE)
            isMouseBeingPressedAndHold_ = false;

        mouseEvent->markAsUsed();

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
            stepRotate(UP);
        else if (button == stepRotateLeftEvent_.button()
            && modifier == stepRotateLeftEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepRotate(LEFT);
        else if (button == stepRotateDownEvent_.button()
            && modifier == stepRotateDownEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepRotate(DOWN);
        else if (button == stepRotateRightEvent_.button()
            && modifier == stepRotateRightEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepRotate(RIGHT);
        else if (button == stepZoomInEvent_.button()
            && modifier == stepZoomInEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepZoom(UP);
        else if (button == stepZoomOutEvent_.button()
            && modifier == stepZoomOutEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepZoom(DOWN);
        else if (button == stepPanUpEvent_.button()
            && modifier == stepPanUpEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepPan(UP);
        else if (button == stepPanLeftEvent_.button()
            && modifier == stepPanLeftEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepPan(LEFT);
        else if (button == stepPanDownEvent_.button()
            && modifier == stepPanDownEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepPan(DOWN);
        else if (button == stepPanRightEvent_.button()
            && modifier == stepPanRightEvent_.modifier()
            && state == KeyboardEvent::KEY_STATE_PRESS)
            stepPan(RIGHT);

        keyEvent->markAsUsed();

        return;
    }
}

void Trackball::rotate(MouseEvent* mouseEvent) {
    ivwAssert(mouseEvent!=0, "Invalid mouse event.");
    // ROTATION
    vec2 curMousePos = mouseEvent->posNormalized();

    //The resulting rotation needs to be mapped to the camera distance,
    //as if the trackball is located at a certain distance from the camera.
    //TODO: Verify this
    //float zDist = (glm::length(*lookFrom_-*lookTo_)-1.f)/M_PI;
    //vec3 curTrackballPos = mapNormalizedMousePosToTrackball(curMousePos, zDist);

    vec3 curTrackballPos = mapNormalizedMousePosToTrackball(curMousePos);

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
        vec3 mappedTrackBallOffsetVector = mapToObject(trackBallOffsetVector);
        vec3 currentCamPos = *lookFrom_;
        vec3 nextCamPos = currentCamPos + mappedTrackBallOffsetVector;

        // obtain rotation axis
        if (glm::degrees(rotationAngle) > pixelWidth_) {
            rotateFromPosToPos(currentCamPos, nextCamPos, rotationAngle);

            //update mouse positions
            lastMousePos_ = curMousePos;
            lastTrackballPos_ = curTrackballPos;
        }
    }

    return;
}

void Trackball::zoom(MouseEvent* mouseEvent) {
    ivwAssert(mouseEvent!=0, "Invalid mouse event.");
    // ZOOM
    float diff;
    vec2 curMousePos = mouseEvent->posNormalized();
    vec3 curTrackballPos = mapNormalizedMousePosToTrackball(curMousePos);
    // compute direction vector
    vec3 direction = *lookFrom_ - *lookTo_;

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
        *lookFrom_ -= direction*diff;
        notifyLookFromChanged(this);
        lastMousePos_ = curMousePos;
        lastTrackballPos_ = curTrackballPos;
    }

    return;
}

void Trackball::pan(MouseEvent* mouseEvent) {
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

    //The resulting rotation needs to be mapped to the camera distance,
    //as if the trackball is located at a certain distance from the camera.
    //TODO: Verify this
    //float zDist = (glm::length(*lookFrom_-*lookTo_)-1.f)/M_PI;
    //vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector, zDist);

    vec3 mappedTrackBallOffsetVector = mapToObject(trackBallOffsetVector);

    if (curMousePos != lastMousePos_) {
        *lookTo_ += mappedTrackBallOffsetVector;
        *lookFrom_ += mappedTrackBallOffsetVector;
        notifyAllChanged(this);
        lastMousePos_ = curMousePos;
        lastTrackballPos_ = curTrackballPos;
    }

    return;
}

void Trackball::stepRotate(Direction dir) {
    // STEP ROTATION
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

    vec3 trackballDirection = mapNormalizedMousePosToTrackball(direction);
    vec3 trackballOrigin = mapNormalizedMousePosToTrackball(origin);
    // calculate rotation angle (in degrees)
    float rotationAngle = glm::angle(trackballDirection, trackballOrigin);
    //difference vector in trackball co-ordinates
    vec3 trackBallOffsetVector = trackballOrigin - trackballDirection;
    //compute next camera position
    vec3 mappedTrackBallOffsetVector = mapToObject(trackBallOffsetVector);
    vec3 currentCamPos = *lookFrom_;
    vec3 nextCamPos = currentCamPos + mappedTrackBallOffsetVector;

    // obtain rotation axis
    if (glm::degrees(rotationAngle) > pixelWidth_) {
        rotateFromPosToPos(currentCamPos, nextCamPos, rotationAngle);
    }

    return;
}

void Trackball::stepZoom(Direction dir) {
    // ZOOM
    // compute direction vector
    vec3 direction = vec3(0);

    if (dir == UP)
        direction = *lookFrom_ - *lookTo_;
    else if (dir == DOWN)
        direction = *lookTo_ - *lookFrom_;

    // zoom by moving the camera
    *lookFrom_ -= direction*STEPSIZE;
    notifyLookFromChanged(this);
    return;
}

void Trackball::stepPan(Direction dir) {
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
    vec3 mappedTrackBallOffsetVector = mapToObject(trackBallOffsetVector);
    *lookTo_  += mappedTrackBallOffsetVector;
    *lookFrom_ += mappedTrackBallOffsetVector;
    notifyAllChanged(this);

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

void Trackball::rotateFromPosToPos(const vec3& currentCamPos, const vec3& nextCamPos, float rotationAngle)
{
    //rotation axis
    vec3 rotationAxis = glm::cross(currentCamPos, nextCamPos);
    // generate quaternion and rotate camera
    rotationAxis = glm::normalize(rotationAxis);
    quat quaternion = glm::angleAxis(rotationAngle, rotationAxis);
    //float lookLength = glm::length(*lookFrom_-*lookTo_);
    *lookFrom_ = *lookTo_+glm::rotate(quaternion, *lookFrom_-*lookTo_);
    *lookUp_ = glm::rotate(quaternion, *lookUp_);

    // Check the length of the length-vector, might change due to float precision
    //vec3 direction = *lookFrom_-*lookTo_;
    //float newLookLength = glm::length(direction);
    //if (lookLength != newLookLength) {
    //    float diff = newLookLength- lookLength;
    //    *lookFrom_ += glm::normalize(direction) * diff;
    //}

    notifyAllChanged(this);
}

} // namespace