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
 * Contact: Daniel Jönsson
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

    , mouseRotate_("trackballRotate", "Rotate", 
        new MouseEvent(MouseEvent::MOUSE_BUTTON_LEFT, InteractionEvent::MODIFIER_NONE,
            MouseEvent::MOUSE_STATE_PRESS | MouseEvent::MOUSE_STATE_MOVE),
        new TrackballAction(TrackballAction::TRACKBALL_ROTATE, this, &Trackball::rotate))
    
    , mouseZoom_("trackballZoom", "Zoom",
        new MouseEvent(MouseEvent::MOUSE_BUTTON_RIGHT, InteractionEvent::MODIFIER_NONE,
            MouseEvent::MOUSE_STATE_PRESS | MouseEvent::MOUSE_STATE_MOVE),
        new TrackballAction(TrackballAction::TRACKBALL_ZOOM, this, &Trackball::zoom))

    , mousePan_("trackballPan", "Pan", 
        new  MouseEvent(MouseEvent::MOUSE_BUTTON_MIDDLE, InteractionEvent::MODIFIER_NONE,
            MouseEvent::MOUSE_STATE_PRESS | MouseEvent::MOUSE_STATE_MOVE),
        new TrackballAction(TrackballAction::TRACKBALL_PAN, this, &Trackball::pan))

    , mouseReset_("mouseReset", "Reset",
        new  MouseEvent(MouseEvent::MOUSE_BUTTON_NONE, InteractionEvent::MODIFIER_NONE,
        MouseEvent::MOUSE_STATE_RELEASE),
        new Action("reset", this, &Trackball::reset))

    , stepRotateUp_("stepRotateUp", "Step rotate up", 
        new KeyboardEvent('W', InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPROTATE_UP, this, &Trackball::rotateUp))

    , stepRotateLeft_("stepRotateLeft", "Step rotate left",
        new KeyboardEvent('A', InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPROTATE_LEFT, this, &Trackball::rotateLeft))

    , stepRotateDown_("stepRotateDown", "Step rotate down",
        new KeyboardEvent('S', InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPROTATE_DOWN, this, &Trackball::rotateDown))

    , stepRotateRight_("stepRotateRight", "Step rotate right", 
        new KeyboardEvent('D', InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPROTATE_RIGHT, this, &Trackball::rotateRight))

    , stepZoomIn_("stepZoomIn", "Step zoom in",
        new KeyboardEvent('R', InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPZOOM_IN, this, &Trackball::zoomIn))

    , stepZoomOut_("stepZoomOut", "Step zoom out",
        new KeyboardEvent('F', InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPZOOM_OUT, this, &Trackball::zoomOut))

    , stepPanUp_("stepPanUp", "Step pan up", 
        new KeyboardEvent('W', InteractionEvent::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPPAN_UP, this, &Trackball::panUp))

    , stepPanLeft_("stepPanLeft", "Step pan left",
        new KeyboardEvent('A', InteractionEvent::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPPAN_LEFT, this, &Trackball::panLeft))

    , stepPanDown_("stepPanDown", "Step pan down", 
        new KeyboardEvent('S', InteractionEvent::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPPAN_DOWN, this, &Trackball::panDown))

    , stepPanRight_("stepPanRight", "Step pan right", 
        new KeyboardEvent('D', InteractionEvent::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS),
        new TrackballAction(TrackballAction::TRACKBALL_STEPPAN_RIGHT, this, &Trackball::panRight)) {
        

    mouseReset_.setVisible(false);

    addProperty(mouseRotate_);
    addProperty(mouseZoom_);
    addProperty(mousePan_);
    addProperty(mouseReset_);
    addProperty(stepRotateUp_);
    addProperty(stepRotateLeft_);
    addProperty(stepRotateDown_);
    addProperty(stepRotateRight_);
    addProperty(stepZoomIn_);
    addProperty(stepZoomOut_);
    addProperty(stepPanUp_);
    addProperty(stepPanLeft_);
    addProperty(stepPanDown_);
    addProperty(stepPanRight_);
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
        else if(gestureEvent->type() == GestureEvent::PAN && gestureEvent->numFingers() > 2){
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
        if (mouseRotate_.getEvent()->matching(mouseEvent)) 
            mouseRotate_.getAction()->invoke(mouseEvent);
        else if (mouseZoom_.getEvent()->matching(mouseEvent))
            mouseZoom_.getAction()->invoke(mouseEvent);
        else if (mousePan_.getEvent()->matching(mouseEvent))
            mousePan_.getAction()->invoke(mouseEvent);
        else if (mouseReset_.getEvent()->matching(mouseEvent))
            mouseReset_.getAction()->invoke(mouseEvent);
       
        mouseEvent->markAsUsed();
        return;
    }

    KeyboardEvent* keyEvent = dynamic_cast<KeyboardEvent*>(event);
    if (keyEvent) {
        if (stepRotateUp_.getEvent()->matching(keyEvent))
            stepRotateUp_.getAction()->invoke(keyEvent);
        else if (stepRotateLeft_.getEvent()->matching(keyEvent))
            stepRotateLeft_.getAction()->invoke(keyEvent);
        else if (stepRotateDown_.getEvent()->matching(keyEvent))
            stepRotateDown_.getAction()->invoke(keyEvent);
        else if (stepRotateRight_.getEvent()->matching(keyEvent))
            stepRotateRight_.getAction()->invoke(keyEvent);
        else if (stepZoomIn_.getEvent()->matching(keyEvent))
            stepZoomIn_.getAction()->invoke(keyEvent);
        else if (stepZoomOut_.getEvent()->matching(keyEvent))
            stepZoomOut_.getAction()->invoke(keyEvent);
        else if (stepPanUp_.getEvent()->matching(keyEvent))
            stepPanUp_.getAction()->invoke(keyEvent);
        else if (stepPanLeft_.getEvent()->matching(keyEvent))
            stepPanLeft_.getAction()->invoke(keyEvent);
        else if (stepPanDown_.getEvent()->matching(keyEvent))
            stepPanDown_.getAction()->invoke(keyEvent);
        else if (stepPanRight_.getEvent()->matching(keyEvent))
            stepPanRight_.getAction()->invoke(keyEvent);

        keyEvent->markAsUsed();
        return;
    }
}

void Trackball::rotate(Event* event) {  
    MouseEvent* mouseEvent = static_cast<MouseEvent*>(event);
    
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

void Trackball::zoom(Event* event) {
    MouseEvent* mouseEvent = static_cast<MouseEvent*>(event);

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

void Trackball::pan(Event* event) {
    MouseEvent* mouseEvent = static_cast<MouseEvent*>(event);

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

void Trackball::rotateLeft(Event* event) {
    stepRotate(LEFT);
}

void Trackball::rotateRight(Event* event) {
    stepRotate(RIGHT);
}

void Trackball::rotateUp(Event* event) {
    stepRotate(UP);
}

void Trackball::rotateDown(Event* event) {
    stepRotate(DOWN);
}

void Trackball::panLeft(Event* event) {
    stepPan(LEFT);
}

void Trackball::panRight(Event* event) {
    stepPan(RIGHT);
}

void Trackball::panUp(Event* event) {
    stepPan(UP);
}

void Trackball::panDown(Event* event) {
    stepPan(DOWN);
}

void Trackball::zoomIn(Event* event) {
    stepZoom(UP);
}

void Trackball::zoomOut(Event* event) {
    stepZoom(DOWN);
}

void Trackball::reset(Event* event) {
    isMouseBeingPressedAndHold_ = false;
}


void TrackballObservable::notifyAllChanged(const Trackball* trackball) const {
    // Notify observers
    for (ObserverSet::iterator it = observers_->begin(), itEnd = observers_->end(); it != itEnd;
         ++it) {
        // static_cast can be used since only template class objects can be added
        static_cast<TrackballObserver*>(*it)->onAllTrackballChanged(trackball);
    }
}

void TrackballObservable::notifyLookFromChanged(const Trackball* trackball) const {
    for (ObserverSet::iterator it = observers_->begin(), itEnd = observers_->end(); it != itEnd;
         ++it) {
        static_cast<TrackballObserver*>(*it)->onLookFromChanged(trackball);
    }
}

void TrackballObservable::notifyLookToChanged(const Trackball* trackball) const {
    for (ObserverSet::iterator it = observers_->begin(), itEnd = observers_->end(); it != itEnd;
         ++it) {
        static_cast<TrackballObserver*>(*it)->onLookToChanged(trackball);
    }
}

void TrackballObservable::notifyLookUpChanged(const Trackball* trackball) const {
    for (ObserverSet::iterator it = observers_->begin(), itEnd = observers_->end(); it != itEnd;
         ++it) {
        static_cast<TrackballObserver*>(*it)->onLookUpChanged(trackball);
    }
}

TrackballObservable::TrackballObservable() : Observable<TrackballObserver>() {

}

} // namespace