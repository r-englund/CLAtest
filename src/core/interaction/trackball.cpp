#include <inviwo/core/interaction/trackball.h>
//#include <iostream>

namespace inviwo {

    static const float MOVEMENT_EPSILON = 0.0001f;	
	static const float ZOOM_FACTOR = 0.05f;
	bool mouseHold = false;

    Trackball::Trackball(CameraProperty* camera)
        : InteractionHandler(),
          camera_(camera),
          lastMousePos_(ivec2(0)),
          lastTrackballPos_(vec3(0.5f)) {}

    Trackball::~Trackball() {}

    vec3 Trackball::mapNormalizedMousePosToTrackball(vec2 mousePos) {
        // set x and y to lie in interval [-0.5, 0.5]
        vec3 result = vec3(mousePos.x-0.5f, mousePos.y-0.5f, 0.0f);
        // compute z = sqrt(r-x^2-y^2) with r=0.5
        result.z = 0.5f - result.x*result.x - result.y*result.y;
        result.z = result.z > 0.0f ? sqrtf(result.z) : 0.0f;
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
				mouseHold = true;
			}

            if (curTrackballPos != lastTrackballPos_) {
                // calculate rotation angle
				float rotationAngle = acos(glm::dot(curTrackballPos, lastTrackballPos_ ));
				
                // obtain rotation axis
                vec3 rotationAxis = glm::cross(curTrackballPos, lastTrackballPos_);
				//std::cout << rotationAxis.x << " " << rotationAxis.y << " " << rotationAxis.z << std::endl;
                
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
            vec3 diff = vec3(0);
            ivec2 curMousePos = mouseEvent->pos();

			// disable movements on first press
			if (!mouseHold){				
				lastMousePos_ = curMousePos;
				mouseHold = true;
			}

			if (curMousePos != lastMousePos_){
				// use the difference in mouse y-position to determine amount of zoom
				diff.z = curMousePos.y - lastMousePos_.y;
				// TODO: Zoom in a better fashion 
				camera_->setLookTo(camera_->lookTo()+diff*ZOOM_FACTOR);     
				camera_->invalidate();
				lastMousePos_ = curMousePos;
			}
			
		} else if (mouseEvent->state() == MouseEvent::MOUSE_STATE_RELEASE) {
			mouseHold = false;
			//std::cout << "Release" << std::endl;
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