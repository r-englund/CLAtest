#include "inviwo/core/interaction/trackball.h"

namespace inviwo {

    static const float MOVEMENT_EPSILON = 0.0001f;

    Trackball::Trackball(CameraProperty* camera)
        : InteractionHandler(),
          camera_(camera),
          lastMousePos_(ivec2(0)),
          lastTrackballPos_(vec3(0.5f)) {}

    Trackball::~Trackball() {}

    vec3 Trackball::mapNormalizedMousePosToTrackball(vec2 mousePos) {
        // set x and y to lie in interval [-0.5, 0.5]
        vec3 result = vec3(mousePos.x-0.5f, mousePos.y*-0.5f, 0.0f);
        // compute z = sqrt(r-x^2-y^2) with r=0.5
        result.z = 0.5f - mousePos.x*mousePos.x - mousePos.y*mousePos.y;
        result.z = result.z > 0.0f ? sqrtf(result.z) : 0.0f;
        return normalize(result);
    }

    void Trackball::invokeEvent(Event* event) {
        MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
        ivec2 curMousePos = mouseEvent->pos();
        vec3 curTrackballPos = mapNormalizedMousePosToTrackball(mouseEvent->posNormalized());
        if (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_LEFT && mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS) {
            /*
            vec3 direction = curTrackballPos - lastTrackballPos_;
            float velocity = length(direction);
            if (velocity > MOVEMENT_EPSILON) {
                //std::cout << "rotation speed " << velocity << std::endl;
            }
            */
            camera_->setLookFrom(curTrackballPos);
        } else if (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_RIGHT && mouseEvent->state() == MouseEvent::MOUSE_STATE_PRESS) {
            std::cout << "movement" << std::endl;
        }
        lastMousePos_ = curMousePos;
        lastTrackballPos_ = curTrackballPos;
    }

} // namespace