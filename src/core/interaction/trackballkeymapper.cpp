#include <inviwo/core/interaction/trackballkeymapper.h>

namespace inviwo {

    // Default key binding
    TrackballKeyMapper::TrackballKeyMapper() {
        keys[TRACKBALL_ROTATE] = MouseEvent::MOUSE_BUTTON_LEFT;
        keys[TRACKBALL_ZOOM] = MouseEvent::MOUSE_BUTTON_RIGHT;
        keys[TRACKBALL_PAN] = MouseEvent::MOUSE_BUTTON_MIDDLE;
    }

    TrackballKeyMapper::~TrackballKeyMapper() {}

    int TrackballKeyMapper::getKey( int action ) {
        return keys[action];
    }

    void TrackballKeyMapper::setKey( int action, int key ) {
        keys[action] = key;
    }

    

} // namespace
