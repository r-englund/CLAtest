#include <inviwo/core/interaction/events/eventmapper.h>

namespace inviwo {

    // Default key binding
    EventMapper::EventMapper() {
        keys[TRACKBALL_ROTATE] = MouseEvent::MOUSE_BUTTON_LEFT;
        keys[TRACKBALL_ZOOM] = MouseEvent::MOUSE_BUTTON_RIGHT;
        keys[TRACKBALL_PAN] = MouseEvent::MOUSE_BUTTON_MIDDLE;
    }

    EventMapper::~EventMapper() {}

    int EventMapper::getKey( int action ) {
        return keys[action];
    }

    void EventMapper::setKey( int action, int key ) {
        keys[action] = key;
    }

    

} // namespace
