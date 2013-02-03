#include <inviwo/core/interaction/trackball.h>
//#include <iostream>

namespace inviwo {

    static const float MOVEMENT_EPSILON = 0.0001f;	
	static const float ZOOM_FACTOR = 0.005f;
    static const float PAN_FACTOR = 0.005f;
	bool mouseHold = false;

    Trackball::Trackball(CameraProperty* camera)
        : InteractionHandler(),
          camera_(camera),
          lastMousePos_(ivec2(0)),
          lastTrackballPos_(vec3(0.5f)) {}

    Trackball::~Trackball() {}

    vec3 Trackball::mapNormalizedMousePosToTrackball(vec2 mousePos) {
        // set x and y to lie in interval [-r, r]
        float r = 0.5f;
        vec3 result = vec3(mousePos.x-r, mousePos.y-r, 0.0f);
        
        // HOLROYD
        // compute z = sqrt(r^2-x^2+y^2) if r^2/2 > x^2+y^2 (sphere)
        if (sqrtf(result.x*result.x + result.y*result.y) <= r*r/2.0f) {
            result.z = r*r - (result.x*result.x + result.y*result.y);
            result.z = result.z > 0.0f ? sqrtf(result.z) : 0.0f;
            //std::cout << "Sphere " << result.z << std::endl;
        
            // else compute z = (r^2/2)/sqrt(x^2 + y^2) (hyperbolic sheet)
        } else if (result.x*result.x + result.y*result.y > 0) {
            result.z = ((r*r)/(2.0f*sqrtf(result.x*result.x + result.y*result.y)));
            //std::cout << "Hyperbolic " << result.z << std::endl;
        }
        
        
        return glm::normalize(result);
    }

    void Trackball::invokeEvent(Event* event) {
        MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
        if (mouseEvent) {
            if (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_MIDDLE && mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS) {
                // ROTATION
                ivec2 curMousePos = mouseEvent->pos();
                vec3 curTrackballPos = mapNormalizedMousePosToTrackball(mouseEvent->posNormalized());                
    			
			    // disable movements on first press
			    if (!mouseHold){				
				    lastTrackballPos_ = curTrackballPos;
                    lastMousePos_ = curMousePos;
				    mouseHold = true;
			    }

                if (curTrackballPos != lastTrackballPos_) {
                    // calculate rotation angle
				    float rotationAngle = acos(glm::dot(curTrackballPos, lastTrackballPos_));
    				
                    // obtain rotation axis
                    vec3 rotationAxis = glm::cross(curTrackballPos, lastTrackballPos_);
                    
				    // generate quaternion and rotate camera                
                    rotationAxis = glm::normalize(rotationAxis);                
                    quat quaternion = glm::angleAxis(rotationAngle*180.0f/3.14f, rotationAxis);
                    camera_->setLookFrom(glm::rotate(quaternion, camera_->lookFrom()));
                    camera_->setLookTo(glm::rotate(quaternion, camera_->lookTo()));
                    camera_->setLookUp(glm::rotate(quaternion, camera_->lookUp()));
                    camera_->invalidate();

                    lastMousePos_ = curMousePos;
                    lastTrackballPos_ = curTrackballPos;
                }
        } else if (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_RIGHT && mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS) {
            // ZOOM
            float diff;
            ivec2 curMousePos = mouseEvent->pos();
            vec2 lastMouseFloatPos = vec2(0);
            
            // compute direction vector
            vec3 direction = camera_->lookFrom() - camera_->lookTo();
			
            // disable movements on first press
			if (!mouseHold){				
				lastMousePos_ = curMousePos;
				mouseHold = true;
			}

			if (curMousePos != lastMousePos_ && direction.length() > 0){

				// use the difference in mouse y-position to determine amount of zoom				
				diff = (lastMousePos_.y - curMousePos.y)*ZOOM_FACTOR;

                // zoom by moving the camera
				camera_->setLookTo(camera_->lookTo()+direction*diff);     
				camera_->invalidate();
				lastMousePos_ = curMousePos;
			}
			
		} else if (mouseEvent->state() == MouseEvent::MOUSE_STATE_RELEASE) {
			mouseHold = false;
        } else if (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_LEFT && mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS) {
            // PAN
            ivec2 curMousePos = mouseEvent->pos();

            // disable movements on first press
            if (!mouseHold){				
                lastMousePos_ = curMousePos;
                mouseHold = true;
            }
           
            if (curMousePos != lastMousePos_){
                vec3 diffX, diffY, tmp;
                // Calculate movement for lookTo
                vec3 direction = camera_->lookFrom()-camera_->lookTo();
                vec3 up = camera_->lookUp();
                diffX = (float)(lastMousePos_.x - curMousePos.x)*glm::normalize(glm::cross(direction, up));
                diffY = (float)(curMousePos.y - lastMousePos_.y)*up;
                camera_->setLookTo(camera_->lookTo()+(diffX+diffY)*PAN_FACTOR);
                
                // Calculate movement for lookFrom
                up = up+direction;
                direction = camera_->lookTo()-camera_->lookFrom();                
                diffX = (float)(lastMousePos_.x - curMousePos.x)*glm::normalize(glm::cross(up, direction));
                diffY = (float)(curMousePos.y - lastMousePos_.y)*up;
                camera_->setLookFrom(camera_->lookFrom()+(diffX+diffY)*PAN_FACTOR);
                camera_->invalidate();
                lastMousePos_ = curMousePos;
            }
        } 
            return;
        }

        ResizeEvent* resizeEvent = dynamic_cast<ResizeEvent*>(event);
        if (resizeEvent) {
            ivec2 canvasSize = resizeEvent->canvasSize();
            float width = (float) canvasSize[0];
            float height = (float) canvasSize[1];
            camera_->setProjectionMatrix(60.f, width/height, 0.0001f, 100.0f );
            return;
        }

        
    }

} // namespace