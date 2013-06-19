#include <inviwo/core/interaction/events/keyboardevent.h>

namespace inviwo {

KeyboardEvent::KeyboardEvent(char ascii, Event::Modifier modifier) : Event() {
	button_ = ascii;
	buttonName_ = ascii;
	modifier_ = modifier;
	modifierName_ = modifierNames_[modifier_];	
}

KeyboardEvent::~KeyboardEvent() {}

void KeyboardEvent::serialize( IvwSerializer& s ) const {
	Event::serialize(s);
	s.serialize("button", buttonName_);
}

void KeyboardEvent::deserialize( IvwDeserializer& s ) {
	Event::deserialize(s);
	s.deserialize("button", buttonName_);
	button_ = buttonName_[0];
}

} // namespace