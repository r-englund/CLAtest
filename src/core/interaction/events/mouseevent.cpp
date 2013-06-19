#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

MouseEvent::MouseEvent(ivec2 position, MouseEvent::MouseButton button,
                       MouseEvent::MouseState state, Event::Modifier modifier, uvec2 canvasSize)
    : Event(),
      position_(position),
      state_(state),
      canvasSize_(canvasSize) {
          buttonNames_[MOUSE_BUTTON_LEFT] = "Left mouse button";
          buttonNames_[MOUSE_BUTTON_RIGHT] = "Right mouse button";
          buttonNames_[MOUSE_BUTTON_MIDDLE] = "Middle mouse button";
		  buttonNames_[MOUSE_BUTTON_NONE] = "Unbound";

          modifierName_ = modifierNames_[modifier];
          modifier_ = modifier;
          buttonName_ = buttonNames_[button];              
          button_ = button;
}

MouseEvent::MouseEvent( MouseEvent::MouseButton button, Event::Modifier modifier )
    : Event() {
    buttonNames_[MOUSE_BUTTON_LEFT] = "Left mouse button";
    buttonNames_[MOUSE_BUTTON_RIGHT] = "Right mouse button";
    buttonNames_[MOUSE_BUTTON_MIDDLE] = "Middle mouse button";
	buttonNames_[MOUSE_BUTTON_NONE] = "Unbound";

    modifierName_ = modifierNames_[modifier];
    modifier_ = modifier;
    buttonName_ = buttonNames_[button];              
    button_ = button;
}

MouseEvent::~MouseEvent() {}

void MouseEvent::serialize( IvwSerializer& s ) const {
	Event::serialize(s);
	s.serialize("button", button_);
}

void MouseEvent::deserialize(IvwDeserializer& d) {
	Event::deserialize(d);
	d.deserialize("button", button_);
	buttonName_ = buttonNames_[button_];
}

} // namespace