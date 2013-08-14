#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

MouseEvent::MouseEvent(ivec2 position, MouseEvent::MouseButton button,
                       MouseEvent::MouseState state, InteractionEvent::Modifier modifier, uvec2 canvasSize)
    : InteractionEvent(),
      position_(position),
      state_(state),
      canvasSize_(canvasSize) {
          buttonNames_[MOUSE_BUTTON_LEFT] = "Left mouse button";
          buttonNames_[MOUSE_BUTTON_RIGHT] = "Right mouse button";
          buttonNames_[MOUSE_BUTTON_MIDDLE] = "Middle mouse button";
		  buttonNames_[MOUSE_BUTTON_NONE] = "";

          modifierName_ = modifierNames_[modifier];
          modifier_ = modifier;
          buttonName_ = buttonNames_[button];              
          button_ = button;
}

MouseEvent::MouseEvent( MouseEvent::MouseButton button, InteractionEvent::Modifier modifier )
    : InteractionEvent() {
    buttonNames_[MOUSE_BUTTON_LEFT] = "Left mouse button";
    buttonNames_[MOUSE_BUTTON_RIGHT] = "Right mouse button";
    buttonNames_[MOUSE_BUTTON_MIDDLE] = "Middle mouse button";
	buttonNames_[MOUSE_BUTTON_NONE] = "";

    modifierName_ = modifierNames_[modifier];
    modifier_ = modifier;
    buttonName_ = buttonNames_[button];              
    button_ = button;
}

MouseEvent::~MouseEvent() {}

void MouseEvent::serialize( IvwSerializer& s ) const {
	InteractionEvent::serialize(s);
	s.serialize("button", buttonName_);
}

void MouseEvent::deserialize( IvwDeserializer& d ) {
	InteractionEvent::deserialize(d);
	d.deserialize("button", buttonName_);
	for (size_t i = 0; i < COUNT; ++i) {
		if (buttonNames_[i] == buttonName_) {
			button_ = i;
			break;
		}
	}
}

} // namespace