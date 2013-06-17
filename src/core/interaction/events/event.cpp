#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

Event::Event() {
    modifierNames_[MODIFIER_NONE] = "";
    modifierNames_[MODIFIER_ALT] = "Alt + ";
    modifierNames_[MODIFIER_CTRL] = "Ctrl + ";
    modifierNames_[MODIFIER_SHIFT] = "Shift + ";
}
Event::~Event() {}

void Event::serialize( IvwSerializer& s ) const {
	int modifier = modifier_;
	s.serialize("button", button_);
	s.serialize("modifier", modifier);
	s.serialize("buttonName", buttonName_);
	s.serialize("modifierName", modifierName_);

}

void Event::deserialize( IvwDeserializer& s ) {
	int modifier;
	s.deserialize("button", button_);
	s.deserialize("modifier", modifier);
	s.deserialize("buttonName", buttonName_);
	s.deserialize("modifierName", modifierName_);
	modifier_ = static_cast<Event::Modifier>(modifier);
}

} // namespace