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

#include <inviwo/core/interaction/trackballlight.h>
#include <glm/gtx/vector_angle.hpp>

namespace inviwo {

static const float RADIUS = 0.5f;
static const float STEPSIZE = 0.05f;

TrackballLight::TrackballLight(vec3* lookFrom, vec3* lookTo, vec3* lookUp)
    : InteractionHandler()
    , PropertyOwner()
    , lookFrom_(lookFrom)
    , lookTo_(lookTo)
    , lookUp_(lookUp)
    , pixelWidth_(0.007f)
    , isMouseBeingPressedAndHold_(false)
    , lastMousePos_(ivec2(0))
    , lastTrackballLightPos_(vec3(0.5f))
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

TrackballLight::~TrackballLight() {}

vec3 TrackballLight::mapNormalizedMousePosToTrackballLight(vec2 mousePos, float dist) {
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

vec3 TrackballLight::mapToCamera(vec3 pos, float dist) {
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


void TrackballLight::invokeEvent(Event* event) {
    GestureEvent* gestureEvent = dynamic_cast<GestureEvent*>(event);
    if (gestureEvent) {
        if(gestureEvent->type() == GestureEvent::PINCH && gestureEvent->numFingers() == 2){
            vec3 direction = glm::normalize(*lookFrom_ - *lookTo_);
            *lookFrom_ = *lookFrom_-direction*(static_cast<float>(gestureEvent->deltaDistance()));
            notifyLookFromChanged(this);
        }
        else if(gestureEvent->type() == GestureEvent::PAN && gestureEvent->numFingers() == 2){
            vec3 offsetVector = vec3(gestureEvent->deltaPos(), 0.f);

            //The resulting rotation needs to be mapped to the camera distance,
            //as if the trackball is located at a certain distance from the camera.
            //TODO: Verify this
            //float zDist = (glm::length(*lookFrom_-*lookTo_)-1.f)/M_PI;
            //vec3 mappedOffsetVector = mapToCamera(offsetVector, zDist);

            vec3 mappedOffsetVector = mapToCamera(offsetVector);

            *lookTo_ += mappedOffsetVector;
            *lookFrom_ += mappedOffsetVector;
            notifyAllChanged(this);

        }
        isMouseBeingPressedAndHold_ = false;

        return;
    }

    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
    if (mouseEvent) {
        int button = mouseEvent->button();
        MouseEvent::MouseState state = mouseEvent->state();
        InteractionEvent::Modifier modifier = mouseEvent->modifier();

        if (button == rotateEvent_.button()
            && modifier == rotateEvent_.modifier()
            && (state == MouseEvent::MOUSE_STATE_MOVE)) {
            //perform rotation
            rotateCamera(mouseEvent);
        } else if (button == zoomEvent_.button()
                   && modifier == zoomEvent_.modifier()
                   && (state == MouseEvent::MOUSE_STATE_MOVE)) {
            //perform zoom
            zoomCamera(mouseEvent);
        } else if (button == panEvent_.button()
                   && modifier == panEvent_.modifier()
                   && (state == MouseEvent::MOUSE_STATE_MOVE)) {
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

void TrackballLight::rotateCamera(MouseEvent* mouseEvent) {
    ivwAssert(mouseEvent!=0, "Invalid mouse event.");
    // ROTATION
    vec2 curMousePos = mouseEvent->posNormalized();

    //The resulting rotation needs to be mapped to the camera distance,
    //as if the trackball is located at a certain distance from the camera.
    //TODO: Verify this
    //float zDist = (glm::length(*lookFrom_-*lookTo_)-1.f)/M_PI;
    //vec3 curTrackballLightPos = mapNormalizedMousePosToTrackballLight(curMousePos, zDist);

    vec3 curTrackballLightPos = mapNormalizedMousePosToTrackballLight(curMousePos);

    // disable movements on first press
    if (!isMouseBeingPressedAndHold_) {
        lastTrackballLightPos_ = curTrackballLightPos;
        lastMousePos_ = curMousePos;
        isMouseBeingPressedAndHold_ = true;
    }

    if (curTrackballLightPos != lastTrackballLightPos_) {
        // calculate rotation angle (in radians)
        float rotationAngle = glm::angle(curTrackballLightPos, lastTrackballLightPos_);
        //difference vector in trackball co-ordinates
        vec3 trackBallOffsetVector = lastTrackballLightPos_ - curTrackballLightPos;
        //compute next camera position
        vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector);
        vec3 currentCamPos = *lookFrom_;
        vec3 nextCamPos = currentCamPos + mappedTrackBallOffsetVector;

        // obtain rotation axis
        if (glm::degrees(rotationAngle) > pixelWidth_) {
            rotateFromPosToPos(currentCamPos, nextCamPos, rotationAngle);

            //update mouse positions
            lastMousePos_ = curMousePos;
            lastTrackballLightPos_ = curTrackballLightPos;
        }
    }

    return;
}

void TrackballLight::zoomCamera(MouseEvent* mouseEvent) {
    ivwAssert(mouseEvent!=0, "Invalid mouse event.");
    // ZOOM
    float diff;
    vec2 curMousePos = mouseEvent->posNormalized();
    vec3 curTrackballLightPos = mapNormalizedMousePosToTrackballLight(curMousePos);
    // compute direction vector
    vec3 direction = *lookFrom_ - *lookTo_;

    // disable movements on first press
    if (!isMouseBeingPressedAndHold_) {
        lastMousePos_ = curMousePos;
        lastTrackballLightPos_ = curTrackballLightPos;
        isMouseBeingPressedAndHold_ = true;
    }

    if (curMousePos != lastMousePos_ && direction.length() > 0) {
        // use the difference in mouse y-position to determine amount of zoom
        diff = curTrackballLightPos.y - lastTrackballLightPos_.y;
        // zoom by moving the camera
        *lookFrom_ -= direction*diff;
        notifyLookFromChanged(this);
        lastMousePos_ = curMousePos;
        lastTrackballLightPos_ = curTrackballLightPos;
    }

    return;
}

void TrackballLight::panCamera(MouseEvent* mouseEvent) {
    ivwAssert(mouseEvent!=0, "Invalid mouse event.");
    // PAN
    vec2 curMousePos = mouseEvent->posNormalized();
    vec3 curTrackballLightPos = mapNormalizedMousePosToTrackballLight(curMousePos);

    // disable movements on first press
    if (!isMouseBeingPressedAndHold_) {
        lastMousePos_ = curMousePos;
        lastTrackballLightPos_ = curTrackballLightPos;
        isMouseBeingPressedAndHold_ = true;
    }

    //difference vector in trackball co-ordinates
    vec3 trackBallOffsetVector = lastTrackballLightPos_ - curTrackballLightPos;
    //compute next camera position
    trackBallOffsetVector.z = 0.0f;

    //The resulting rotation needs to be mapped to the camera distance,
    //as if the trackball is located at a certain distance from the camera.
    //TODO: Verify this
    //float zDist = (glm::length(*lookFrom_-*lookTo_)-1.f)/M_PI;
    //vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector, zDist);

    vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector);

    if (curMousePos != lastMousePos_) {
        *lookTo_ += mappedTrackBallOffsetVector;
        *lookFrom_ += mappedTrackBallOffsetVector;
        notifyAllChanged(this);
        lastMousePos_ = curMousePos;
        lastTrackballLightPos_ = curTrackballLightPos;
    }

    return;
}

void TrackballLight::stepRotateCamera(Direction dir) {
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

    vec3 trackballDirection = mapNormalizedMousePosToTrackballLight(direction);
    vec3 trackballOrigin = mapNormalizedMousePosToTrackballLight(origin);
    // calculate rotation angle (in degrees)
    float rotationAngle = glm::angle(trackballDirection, trackballOrigin);
    //difference vector in trackball co-ordinates
    vec3 trackBallOffsetVector = trackballOrigin - trackballDirection;
    //compute next camera position
    vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector);
    vec3 currentCamPos = *lookFrom_;
    vec3 nextCamPos = currentCamPos + mappedTrackBallOffsetVector;

    // obtain rotation axis
    if (glm::degrees(rotationAngle) > pixelWidth_) {
        rotateFromPosToPos(currentCamPos, nextCamPos, rotationAngle);
    }

    return;
}

void TrackballLight::stepZoomCamera(Direction dir) {
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

void TrackballLight::stepPanCamera(Direction dir) {
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
    vec3 trackballDirection = mapNormalizedMousePosToTrackballLight(direction);
    vec3 trackballOrigin = mapNormalizedMousePosToTrackballLight(origin);
    //difference vector in trackball co-ordinates
    vec3 trackBallOffsetVector = trackballOrigin - trackballDirection;
    //compute next camera position
    trackBallOffsetVector.z = 0.0f;
    vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector);
    *lookTo_  += mappedTrackBallOffsetVector;
    *lookFrom_ += mappedTrackBallOffsetVector;
    notifyAllChanged(this);

    return;
}

void TrackballLight::addProperty(Property& property) {
    if (dynamic_cast<EventProperty*>(&property))
        addProperty(&property);
}

void TrackballLight::addProperty(Property* property) {
    if (dynamic_cast<EventProperty*>(property)) {
        properties_.push_back(property);
        property->setOwner(this);
    }
}

void TrackballLight::serialize(IvwSerializer& s) const {
    InteractionHandler::serialize(s);
    PropertyOwner::serialize(s);
}

void TrackballLight::deserialize(IvwDeserializer& d) {
    InteractionHandler::deserialize(d);
    PropertyOwner::deserialize(d);
}

void TrackballLight::rotateFromPosToPos(vec3 currentCamPos, vec3 nextCamPos, float rotationAngle)
{
    //rotation axis
    vec3 rotationAxis = glm::cross(currentCamPos, nextCamPos);
    // generate quaternion and rotate camera
    rotationAxis = glm::normalize(rotationAxis);
    quat quaternion = glm::angleAxis(rotationAngle, rotationAxis);
    float lookLength = glm::length(*lookFrom_-*lookTo_);
    vec3 offset = *lookTo_;
    vec3 rotation = glm::rotate(quaternion, offset);
    *lookTo_ = rotation;
    *lookFrom_ = glm::rotate(quaternion, *lookFrom_);
    *lookUp_ = glm::rotate(quaternion, *lookUp_);

    // Check the length of the length-vector, might change due to float precision
    if (lookLength != glm::length(*lookFrom_-*lookTo_)) {
        float diff = lookLength/glm::length(*lookFrom_-*lookTo_);
        *lookTo_ = (*lookTo_) * diff;
    }

    notifyAllChanged(this);	
}

} // namespace