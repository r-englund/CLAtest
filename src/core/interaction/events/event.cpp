#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

Event::Event() {
    modifierNames_[MODIFIER_NONE] = "";
    modifierNames_[MODIFIER_ALT] = "Alt + ";
    modifierNames_[MODIFIER_CTRL] = "Ctrl + ";
    modifierNames_[MODIFIER_SHIFT] = "Shift + ";
}
Event::~Event() {}

void Event::serialize(IvwSerializer& s) const {
	s.serialize("modifier", modifier_);
}

void Event::deserialize(IvwDeserializer& d) {
	int modifier;
	d.deserialize("modifier", modifier);
	modifier_ = static_cast<Event::Modifier>(modifier);
	modifierName_ = modifierNames_[modifier_];
}

} // namespace