#include <inviwo/core/interaction/trackballkeymapper.h>

namespace inviwo {

    // Default key binding
    TrackballKeyMapper::TrackballKeyMapper() {
        keys[TRACKBALL_ROTATE] = MouseEvent::MOUSE_BUTTON_LEFT;
        keys[TRACKBALL_ZOOM] = MouseEvent::MOUSE_BUTTON_RIGHT;
        keys[TRACKBALL_PAN] = MouseEvent::MOUSE_BUTTON_MIDDLE;
        actionNames[TRACKBALL_ROTATE] = "Rotate";
        actionNames[TRACKBALL_ZOOM] = "Zoom";
        actionNames[TRACKBALL_PAN] = "Pan";
        eventNames[MouseEvent::MOUSE_BUTTON_LEFT] = "Left mouse button";
        eventNames[MouseEvent::MOUSE_BUTTON_MIDDLE] = "Middle mouse button";
        eventNames[MouseEvent::MOUSE_BUTTON_RIGHT] = "Right mouse button";
    }

    TrackballKeyMapper::~TrackballKeyMapper() {}

    int TrackballKeyMapper::getKey( int action ) {
        return keys[action];
    }

    void TrackballKeyMapper::setKey( int action, int key ) {
        keys[action] = key;
    }

    std::string TrackballKeyMapper::getActionName( int action )
    {
        return actionNames[action];
    }

    std::string TrackballKeyMapper::getEventName( int eventNum )
    {
        return eventNames[eventNum];
    }

    

} // namespace
