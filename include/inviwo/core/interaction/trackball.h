#ifndef IVW_TRACKBALL_H
#define IVW_TRACKBALL_H

#include "inviwo/core/interaction/interactionhandler.h"
#include "inviwo/core/interaction/events/mouseevent.h"
#include "inviwo/core/interaction/events/resizeevent.h"
#include "inviwo/core/properties/cameraproperty.h"

namespace inviwo {

    class Trackball : public InteractionHandler {

    public:
        Trackball(CameraProperty* camera);
        ~Trackball();

        virtual void invokeEvent(Event* event);

    private:
        CameraProperty* camera_;

        ivec2 lastMousePos_;
        vec3 lastTrackballPos_;

        vec3 mapNormalizedMousePosToTrackball(vec2 mousePos);
    };

} // namespace

#endif // IVW_TRACKBALL_H