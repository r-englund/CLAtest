#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

Event::Event() {
    modifierNames_[MODIFIER_NONE] = "";
    modifierNames_[MODIFIER_ALT] = "Alt";
    modifierNames_[MODIFIER_CTRL] = "Ctrl";
    modifierNames_[MODIFIER_SHIFT] = "Shift";
}
Event::~Event() {}

void Event::serialize(IvwSerializer& s) const {
	s.serialize("type", getClassName(), true);
	s.serialize("modifier", modifierName_);
}

void Event::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
	d.deserialize("modifier", modifierName_);
	for (size_t i = 0; i < COUNT; ++i) {
		if (modifierNames_[i] == modifierName_) {
			modifier_ = static_cast<Event::Modifier>(i);
			break;
		}
	}
}

} // namespace