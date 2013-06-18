#ifndef IVW_MOUSEEVENT_H
#define IVW_MOUSEEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

class IVW_CORE_API MouseEvent : public Event {
public:
    enum MouseButton {
        MOUSE_BUTTON_NONE   =      0,
        MOUSE_BUTTON_LEFT   = 1 << 1,
        MOUSE_BUTTON_MIDDLE = 1 << 2,
        MOUSE_BUTTON_RIGHT  = 1 << 3,
        COUNT
    };

    enum MouseState {
        MOUSE_STATE_NONE    =      0,
        MOUSE_STATE_MOVE    = 1 << 1,
        MOUSE_STATE_PRESS   = 1 << 2,
        MOUSE_STATE_RELEASE = 1 << 3
    };

    MouseEvent(ivec2 position, MouseEvent::MouseButton button,
               MouseEvent::MouseState state, Event::Modifier modifier, uvec2 canvasSize);
    MouseEvent(MouseEvent::MouseButton button, Event::Modifier modifier);
    ~MouseEvent();

    inline ivec2 pos() const { return position_; }
    inline vec2 posNormalized() const { return vec2(vec2(position_)/vec2(canvasSize_)); }
    inline unsigned int x() const { return position_.x; }
    inline unsigned int y() const { return position_.y; }
    inline MouseEvent::MouseState state() const { return state_; }
    inline uvec2 canvasSize() const {return canvasSize_; }

	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& s);

private:
    ivec2 position_;
    MouseEvent::MouseState state_;
    uvec2 canvasSize_;
    std::string buttonNames_[COUNT];
};

} // namespace

#endif // IVW_MOUSEEVENT_H