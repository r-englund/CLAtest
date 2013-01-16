#ifndef IVW_MOUSEEVENT_H
#define IVW_MOUSEEVENT_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/interaction/events/event.h"

namespace inviwo {

    class MouseEvent : public Event {

    public:
        enum MouseButton {
            MOUSE_BUTTON_NONE   =      0,
            MOUSE_BUTTON_LEFT   = 1 << 1,
            MOUSE_BUTTON_MIDDLE = 1 << 2,
            MOUSE_BUTTON_RIGHT  = 1 << 3
        };

        enum MouseState {
            MOUSE_STATE_NONE    =      0,
            MOUSE_STATE_MOVE    = 1 << 1,
            MOUSE_STATE_PRESS   = 1 << 2,
            MOUSE_STATE_RELEASE = 1 << 3
        };

        MouseEvent(ivec2 position, MouseEvent::MouseButton button,
                   MouseEvent::MouseState state, Event::Modifier modifier, ivec2 canvasSize);
        ~MouseEvent();

        inline ivec2 pos() const { return position_; }
        inline vec2 posNormalized() const { return vec2(vec2(position_)/vec2(canvasSize_)); }
        inline unsigned int x() const { return position_.x; }
        inline unsigned int y() const { return position_.y; }
        inline MouseEvent::MouseButton button() const { return button_; }
        inline MouseEvent::MouseState state() const { return state_; }
        inline Event::Modifier modifier() const { return modifier_; }
        inline ivec2 canvasSize() const {return canvasSize_; }

    private:
        ivec2 position_;
        MouseEvent::MouseButton button_;
        MouseEvent::MouseState state_;
        Event::Modifier modifier_;
        ivec2 canvasSize_;
    };

} // namespace

#endif // IVW_MOUSEEVENT_H