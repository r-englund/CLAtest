#include <inviwo/core/interaction/trackball.h>
#include <modules/opengl/inviwoopengl.h> // FOR DEBUGGING

namespace inviwo {

    static const float MOVEMENT_THRESHOLD =0.007f;	
    static const float RADIUS = 0.5f;
	bool mouseHold = false;

    Trackball::Trackball(CameraProperty* camera)
        : InteractionHandler(),
          camera_(camera),
          lastMousePos_(ivec2(0)),
          lastTrackballPos_(vec3(0.5f)) {}

    Trackball::~Trackball() {}

    vec3 Trackball::mapNormalizedMousePosToTrackball(vec2 mousePos) {
        // set x and y to lie in interval [-r, r]
        float r = RADIUS;
        vec3 result = vec3(mousePos.x-RADIUS, -1.0f*(mousePos.y-RADIUS), 0.0f);
        
        // HOLROYD
        // compute z = sqrt(r^2-x^2+y^2) if r/sqrt(2) => sqrt(x^2+y^2) (sphere)
        if ((result.x*result.x + result.y*result.y) <= r*r/(2.0f)) {
            result.z = r*r - (result.x*result.x + result.y*result.y);
            result.z = result.z > 0.0f ? sqrtf(result.z) : 0.0f;
            //std::cout << "Sphere " << result.z << std::endl;
        
        // else compute z = (r^2/(2*sqrt(x^2 + y^2)) (hyperbolic sheet)
        } else {
            result.z = ((r*r)/(2.0f*sqrtf(result.x*result.x + result.y*result.y)));
            //std::cout << "Hyperbolic " << result.z << std::endl;
        }

        // Map trackball coordinates to the camera according to result' = viewmatrix*(eye^-1)*result <=> result' = viewmatrix*result
        //vec4 tmp = vec4(result.x, result.y, result.z, 1.0f);
        //tmp = glm::inverse(camera_->viewMatrix())*tmp;
        //result = vec3(tmp.x, tmp.y, tmp.z);
        return glm::normalize(result);
    }

    void Trackball::invokeEvent(Event* event) {
        MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);

        // FOR DEBUGGING 
        // Draws the camera direction. Should only be seen as a single white pixel in a perfect world
        //glBegin(GL_LINES);
        //    glColor3f(1.0f,1.0f,1.0f);
        //    glVertex3f(camera_->lookTo().x, camera_->lookTo().y, camera_->lookTo().z);
        //    glVertex3f(camera_->lookFrom().x, camera_->lookFrom().y, camera_->lookFrom().z);
        //glEnd();

        if (mouseEvent) {
            if (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_LEFT && mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS) {
                // ROTATION
                vec2 curMousePos = mouseEvent->posNormalized();
                vec3 curTrackballPos = mapNormalizedMousePosToTrackball(curMousePos);
                float lookLength;
    			
			    // disable movements on first press
			    if (!mouseHold){				
				    lastTrackballPos_ = curTrackballPos;
                    lastMousePos_ = curMousePos;
				    mouseHold = true;
			    }
                //vec2 normMousePos = mouseEvent->posNormalized();
                if (curTrackballPos != lastTrackballPos_) {

                    // Map trackball coordinates to the camera according to result' = viewmatrix*(eye^-1)*result <=> result' = viewmatrix*result
                    vec4 curr = vec4(curTrackballPos.x, curTrackballPos.y, curTrackballPos.z, 0.0f);
                    vec4 prev = vec4(lastTrackballPos_.x, lastTrackballPos_.y, lastTrackballPos_.z, 0.0f);
                    curr = camera_->viewMatrix()*curr;
                    prev = camera_->viewMatrix()*prev;
                    //result = vec3(tmp.x, tmp.y, tmp.z);

                    // calculate rotation angle
				    float rotationAngle = acos(glm::dot(curr, prev));
                    std::cout << rotationAngle << std::endl;   

                    // FOR DEBUGGING 
                    // Draws a line with the trackball x and y. Color with z.
                    //glBegin(GL_LINES);
                    //    glColor3f(1.0f, curTrackballPos.z, 0.0f);
                    //    glVertex3f(0.0f,0,0.0f);
                    //    glVertex3f(curTrackballPos.x, curTrackballPos.y, 0.0f);
                    //glEnd();
                    
                    // obtain rotation axis
                    if(rotationAngle > MOVEMENT_THRESHOLD){
                        vec3 rotationAxis = glm::cross(vec3(curr.x, curr.y, curr.z), vec3(prev.x, prev.y, prev.z) );                       
                        
				        // generate quaternion and rotate camera                
                        rotationAxis = glm::normalize(rotationAxis);                
                        quat quaternion = glm::angleAxis(rotationAngle*180.0f/3.14f, rotationAxis);
                        lookLength = glm::length(camera_->lookFrom()-camera_->lookTo());
                        vec3 offset = camera_->lookFrom();
                        vec3 rotation = glm::rotate(quaternion, offset);
                        camera_->setLookFrom(rotation);
                        camera_->setLookTo(glm::rotate(quaternion, camera_->lookTo()));
                        camera_->setLookUp(glm::rotate(quaternion, camera_->lookUp()));
                        
                        // Check the length of the length-vector, might change due to float precision
                        if (lookLength != glm::length(camera_->lookFrom()-camera_->lookTo())){
                            float diff = lookLength/glm::length(camera_->lookFrom()-camera_->lookTo());
                            camera_->setLookTo(camera_->lookTo()*diff);
                        }

                        //std::cout << lookLength-glm::length(camera_->lookFrom() - camera_->lookTo()) << std::endl;
                        camera_->invalidate();
                        
                        lastMousePos_ = curMousePos;
                        lastTrackballPos_ = curTrackballPos;
                    }
                }
        } else if (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_RIGHT && mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS) {
            // ZOOM
            float diff;
            vec2 curMousePos = mouseEvent->posNormalized();
            vec3 curTrackballPos = mapNormalizedMousePosToTrackball(curMousePos);
          
            // compute direction vector
            vec3 direction = camera_->lookFrom() - camera_->lookTo();
			
            // disable movements on first press
			if (!mouseHold){				
				lastMousePos_ = curMousePos;
                lastTrackballPos_ = curTrackballPos;
				mouseHold = true;
			}

			if (curMousePos != lastMousePos_ && direction.length() > 0){

				// use the difference in mouse y-position to determine amount of zoom				
				diff = curTrackballPos.y - lastTrackballPos_.y;

                // zoom by moving the camera
                camera_->setLookTo(camera_->lookTo()+direction*diff);
				camera_->invalidate();
				lastMousePos_ = curMousePos;
                lastTrackballPos_ = curTrackballPos;
			}
			
		} else if (mouseEvent->state() == MouseEvent::MOUSE_STATE_RELEASE) {
			mouseHold = false;
        } else if (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_MIDDLE && mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS) {
            // PAN
            vec2 curMousePos = mouseEvent->posNormalized();
            vec3 curTrackballPos = mapNormalizedMousePosToTrackball(curMousePos);

            // disable movements on first press
            if (!mouseHold){				
                lastMousePos_ = curMousePos;
                lastTrackballPos_ = curTrackballPos;
                mouseHold = true;
            }
           
            if (curMousePos != lastMousePos_){
                vec3 diffX, diffY, tmp;

                // Calculate movement
                vec3 direction = camera_->lookFrom()-camera_->lookTo();
                vec3 up = camera_->lookUp();
                diffX = (float)(lastMousePos_.x - curTrackballPos.x)*glm::normalize(glm::cross(direction, up));
                diffY = (float)(lastMousePos_.y - curTrackballPos.y)*up;
                camera_->setLookTo(camera_->lookTo()+diffX+diffY);
                camera_->setLookFrom(camera_->lookFrom()+diffX+diffY);
                camera_->invalidate();
                lastMousePos_ = curMousePos;
                lastTrackballPos_ = curTrackballPos;
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