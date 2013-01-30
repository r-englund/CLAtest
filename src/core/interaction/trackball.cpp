#include <inviwo/core/interaction/trackball.h>
#include <iostream>

namespace inviwo {

    static const float MOVEMENT_EPSILON = 0.0001f;	
	static const float ZOOM_FACTOR = 0.005f;
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

        // compute z = sqrt(r^2-x^2+y^2) if r^2/2 > x^2+y^2 (sphere)
        if (result.x*result.x + result.y*result.y <= r*r/2.0f) {
            result.z = 0.25f - result.x*result.x + result.y*result.y;
            result.z = result.z > 0.0f ? sqrtf(result.z) : 0.0f;
        
            // else compute z = (r^2/2)/sqrt(x^2 + y^2) (hyperbolic sheet)
        } else if (result.x*result.x + result.y*result.y > 0) {
            result.z = (r*r/2.0f)/sqrtf(result.x*result.x + result.y*result.y);
        }        
        
        return glm::normalize(result);
    }

    void Trackball::invokeEvent(Event* event) {
        MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
        if (mouseEvent) {
        if (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_LEFT && mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS) {
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
				float rotationAngle = acos(glm::dot(lastTrackballPos_, curTrackballPos));
				
                // obtain rotation axis
                vec3 rotationAxis = glm::cross(lastTrackballPos_, curTrackballPos);
                
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
            float diff;
            ivec2 curMousePos = mouseEvent->pos();
            // compute direction vector
            vec3 direction = camera_->lookFrom() - camera_->lookTo();

			//std::cout << "Look to: " << camera_->lookTo().x << " " << camera_->lookTo().y << " " << camera_->lookTo().z << std::endl;
			
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