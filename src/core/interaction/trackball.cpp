#include <inviwo/core/interaction/trackball.h>

namespace inviwo {

static const float RADIUS = 0.5f;
static const float STEPSIZE = 0.05f;

Trackball::Trackball(CameraProperty* camera) : InteractionHandler(), PropertyOwner(),
      camera_(camera),
      lastMousePos_(ivec2(0)),
      lastTrackballPos_(vec3(0.5f)),
      pixelWidth_(0.007f),
      isMouseBeingPressedAndHold_(false) {
	// Eventproperties for continuous movement
    rotateEventProperty_ = new EventProperty("trackballRotate", "Rotate", 
        new MouseEvent(MouseEvent::MOUSE_BUTTON_LEFT, Event::MODIFIER_NONE), 
        new TrackballAction(TrackballAction::TRACKBALL_ROTATE));
    zoomEventProperty_ = new EventProperty("trackballZoom", "Zoom", 
        new MouseEvent(MouseEvent::MOUSE_BUTTON_RIGHT, Event::MODIFIER_NONE), 
        new TrackballAction(TrackballAction::TRACKBALL_ZOOM));
    panEventProperty_ = new EventProperty("trackballPan", "Pan", 
        new MouseEvent(MouseEvent::MOUSE_BUTTON_MIDDLE, Event::MODIFIER_NONE), 
        new TrackballAction(TrackballAction::TRACKBALL_PAN));

	// Eventproperties for step rotate
	stepRotateUp_ = new EventProperty("stepRotateUp", "Step rotate up",
		new KeyboardEvent('W', Event::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPROTATE_UP));
	stepRotateLeft_ = new EventProperty("stepRotateLeft", "Step rotate left",
		new KeyboardEvent('A', Event::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPROTATE_LEFT));
	stepRotateDown_ = new EventProperty("stepRotateDown", "Step rotate down",
		new KeyboardEvent('S', Event::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPROTATE_DOWN));
	stepRotateRight_ = new EventProperty("stepRotateRight", "Step rotate right",
		new KeyboardEvent('D', Event::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPROTATE_RIGHT));

	// Eventproperties for step zoom
	stepZoomIn_ = new EventProperty("stepZoomIn", "Step zoom in",
		new KeyboardEvent('R', Event::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPZOOM_IN));
	stepZoomOut_ = new EventProperty("stepZoomOut", "Step zoom out",
		new KeyboardEvent('F', Event::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPZOOM_OUT));

	// Eventproperties for step pan
	stepPanUp_ = new EventProperty("stepPanUp", "Step pan up",
		new KeyboardEvent('W', Event::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPPAN_UP));
	stepPanLeft_ = new EventProperty("stepPanLeft", "Step pan left",
		new KeyboardEvent('A', Event::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPPAN_LEFT));
	stepPanDown_ = new EventProperty("stepPanDown", "Step pan down",
		new KeyboardEvent('S', Event::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPPAN_DOWN));
	stepPanRight_ = new EventProperty("stepPanRight", "Step pan right",
		new KeyboardEvent('D', Event::MODIFIER_SHIFT, KeyboardEvent::KEY_STATE_PRESS),
		new TrackballAction(TrackballAction::TRACKBALL_STEPPAN_RIGHT));


	rotateEventProperty_->setGroupID("Continuous movement");
	zoomEventProperty_->setGroupID("Continuous movement");
	panEventProperty_->setGroupID("Continuous movement");
	stepRotateUp_->setGroupID("Step rotation");
	stepRotateLeft_->setGroupID("Step rotation");
	stepRotateDown_->setGroupID("Step rotation");
	stepRotateRight_->setGroupID("Step rotation");
	stepZoomIn_->setGroupID("Step zooming");
	stepZoomOut_->setGroupID("Step zooming");
	stepPanUp_->setGroupID("Step panning");
	stepPanLeft_->setGroupID("Step panning");
	stepPanDown_->setGroupID("Step panning");
	stepPanRight_->setGroupID("Step panning");

    addProperty(rotateEventProperty_);
    addProperty(zoomEventProperty_);
    addProperty(panEventProperty_);
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
    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
	KeyboardEvent* keyEvent = dynamic_cast<KeyboardEvent*>(event);

    // FOR DEBUGGING 
    // Draws the camera direction. Should only be seen as a single white pixel in a perfect world
    //glBegin(GL_LINES);
    //    glColor3f(1.0f,1.0f,1.0f);
    //    glVertex3f(camera_->lookTo().x, camera_->lookTo().y, camera_->lookTo().z);
    //    glVertex3f(camera_->lookFrom().x, camera_->lookFrom().y, camera_->lookFrom().z);
    //glEnd();

	if (keyEvent) {
		int button = keyEvent->button();
		KeyboardEvent::KeyState state = keyEvent->state();
		Event::Modifier modifier = keyEvent->modifier();

		if (button == stepRotateUp_->getEvent()->button() 
			&& modifier == stepRotateUp_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepRotateCamera(UP);
		} else if (button == stepRotateLeft_->getEvent()->button() 
			&& modifier == stepRotateLeft_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepRotateCamera(LEFT);
		} else if (button == stepRotateDown_->getEvent()->button() 
			&& modifier == stepRotateDown_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepRotateCamera(DOWN);
		} else if (button == stepRotateRight_->getEvent()->button() 
			&& modifier == stepRotateRight_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepRotateCamera(RIGHT);
		} else if (button == stepZoomIn_->getEvent()->button() 
			&& modifier == stepZoomIn_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepZoomCamera(UP);
		} else if (button == stepZoomOut_->getEvent()->button() 
			&& modifier == stepZoomOut_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepZoomCamera(DOWN);
		} else if (button == stepPanUp_->getEvent()->button() 
			&& modifier == stepPanUp_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepPanCamera(UP);
		} else if (button == stepPanLeft_->getEvent()->button() 
			&& modifier == stepPanLeft_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepPanCamera(LEFT);
		} else if (button == stepPanDown_->getEvent()->button() 
			&& modifier == stepPanDown_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepPanCamera(DOWN);
		} else if (button == stepPanRight_->getEvent()->button() 
			&& modifier == stepPanRight_->getEvent()->modifier() 
			&& state == KeyboardEvent::KEY_STATE_PRESS) {
				stepPanCamera(RIGHT);
		}
	}

    if (mouseEvent) {
		int button = mouseEvent->button();
		MouseEvent::MouseState state = mouseEvent->state();
		Event::Modifier modifier = mouseEvent->modifier();

        if (button == rotateEventProperty_->getEvent()->button() 
			&& modifier == rotateEventProperty_->getEvent()->modifier() 
			&& state == MouseEvent::MOUSE_STATE_PRESS) {
            //perform rotation
            rotateCamera(mouseEvent);
        } else if (button == zoomEventProperty_->getEvent()->button()
			&& modifier == zoomEventProperty_->getEvent()->modifier()
			&& state == MouseEvent::MOUSE_STATE_PRESS) {
            //perform zoom
            zoomCamera(mouseEvent);
	    } else if (button == panEventProperty_->getEvent()->button() 
			&& modifier == panEventProperty_->getEvent()->modifier()
			&& state == MouseEvent::MOUSE_STATE_PRESS) {
            //perform pan
            panCamera(mouseEvent);  
        } else if (state == MouseEvent::MOUSE_STATE_RELEASE) {
            isMouseBeingPressedAndHold_ = false;
        } 
        return;
    }    

    //FIXME: should this be here?
    ResizeEvent* resizeEvent = dynamic_cast<ResizeEvent*>(event);
    if (resizeEvent) {
        uvec2 canvasSize = resizeEvent->size();
        float width = (float) canvasSize[0];
        float height = (float) canvasSize[1];
        camera_->setProjectionMatrix(60.f, width/height, 0.0001f, 100.0f );
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

        // calculate rotation angle (in degrees)
        float rotationAngle = glm::angle(curTrackballPos, lastTrackballPos_);

        //difference vector in trackball co-ordinates
        vec3 trackBallOffsetVector = lastTrackballPos_ - curTrackballPos;        

        //compute next camera position
        vec3 mappedTrackBallOffsetVector = mapToCamera(trackBallOffsetVector);
        vec3 currentCamPos = camera_->getLookFrom();        
        vec3 nextCamPos = currentCamPos + mappedTrackBallOffsetVector;

        // FOR DEBUGGING 
        // Draws a line with the trackball x and y. Color with z.
        //glBegin(GL_LINES);
        //    glColor3f(1.0f, curTrackballPos.z, 0.0f);
        //    glVertex3f(0.0f,0,0.0f);
        //    glVertex3f(curTrackballPos.x, curTrackballPos.y, 0.0f);
        //glEnd();

        // obtain rotation axis
        if (rotationAngle > pixelWidth_) {
            //rotation axis
            vec3 rotationAxis = glm::cross(currentCamPos, nextCamPos);

            // generate quaternion and rotate camera                
            rotationAxis = glm::normalize(rotationAxis);                
            quat quaternion = glm::angleAxis(rotationAngle, rotationAxis);
            lookLength = glm::length(camera_->getLookFrom()-camera_->getLookTo());
            vec3 offset = camera_->getLookTo();
            vec3 rotation = glm::rotate(quaternion, offset);
            camera_->setLookTo(rotation);
            camera_->setLookFrom(glm::rotate(quaternion, camera_->getLookFrom()));
            camera_->setLookUp(glm::rotate(quaternion, camera_->getLookUp()));

            // Check the length of the length-vector, might change due to float precision
            if (lookLength != glm::length(camera_->getLookFrom()-camera_->getLookTo())) {
                float diff = lookLength/glm::length(camera_->getLookFrom()-camera_->getLookTo());
                camera_->setLookTo(camera_->getLookTo()*diff);
            }

            camera_->invalidate(PropertyOwner::INVALID_OUTPUT);

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
    if (!isMouseBeingPressedAndHold_){				
        lastMousePos_ = curMousePos;
        lastTrackballPos_ = curTrackballPos;
        isMouseBeingPressedAndHold_ = true;
    }

    if (curMousePos != lastMousePos_ && direction.length() > 0) {

        // use the difference in mouse y-position to determine amount of zoom				
        diff = curTrackballPos.y - lastTrackballPos_.y;

        // zoom by moving the camera
        camera_->setLookFrom(camera_->getLookFrom()-direction*diff);
        camera_->invalidate(PropertyOwner::INVALID_OUTPUT);
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
        camera_->setLookTo(camera_->getLookTo()     + mappedTrackBallOffsetVector);
        camera_->setLookFrom(camera_->getLookFrom() + mappedTrackBallOffsetVector);
        camera_->invalidate(PropertyOwner::INVALID_OUTPUT);
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
	if (rotationAngle > pixelWidth_) {
		//rotation axis
		vec3 rotationAxis = glm::cross(currentCamPos, nextCamPos);

		// generate quaternion and rotate camera                
		rotationAxis = glm::normalize(rotationAxis);                
		quat quaternion = glm::angleAxis(rotationAngle, rotationAxis);
		lookLength = glm::length(camera_->getLookFrom()-camera_->getLookTo());
		vec3 offset = camera_->getLookTo();
		vec3 rotation = glm::rotate(quaternion, offset);
		camera_->setLookTo(rotation);
		camera_->setLookFrom(glm::rotate(quaternion, camera_->getLookFrom()));
		camera_->setLookUp(glm::rotate(quaternion, camera_->getLookUp()));

		// Check the length of the length-vector, might change due to float precision
		if (lookLength != glm::length(camera_->getLookFrom()-camera_->getLookTo())) {
			float diff = lookLength/glm::length(camera_->getLookFrom()-camera_->getLookTo());
			camera_->setLookTo(camera_->getLookTo()*diff);
		}

		camera_->invalidate(PropertyOwner::INVALID_OUTPUT);
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
	camera_->invalidate(PropertyOwner::INVALID_OUTPUT);

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
	camera_->setLookTo(camera_->getLookTo()     + mappedTrackBallOffsetVector);
	camera_->setLookFrom(camera_->getLookFrom() + mappedTrackBallOffsetVector);
	camera_->invalidate(PropertyOwner::INVALID_OUTPUT);

	return;
}

void Trackball::addProperty( Property& property ) {
    if (dynamic_cast<EventProperty*>(&property)) 
       addProperty(&property);    
}

void Trackball::addProperty( Property* property ) {    
    if (dynamic_cast<EventProperty*>(property)) {
        properties_.push_back(property);
        property->setOwner(this);
    }
}

void Trackball::serialize(IvwSerializer &s) const {
	InteractionHandler::serialize(s);
	PropertyOwner::serialize(s);
}

void Trackball::deserialize(IvwDeserializer &d) {
    InteractionHandler::deserialize(d);
	PropertyOwner::deserialize(d);
}

} // namespace