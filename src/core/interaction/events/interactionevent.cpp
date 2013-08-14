
#include <inviwo/core/interaction/events/interactionevent.h>

namespace inviwo {

InteractionEvent::InteractionEvent() {
    modifierNames_[MODIFIER_NONE] = "";
    modifierNames_[MODIFIER_ALT] = "Alt";
    modifierNames_[MODIFIER_CTRL] = "Ctrl";
    modifierNames_[MODIFIER_SHIFT] = "Shift";
}
InteractionEvent::~InteractionEvent() {}

void InteractionEvent::serialize(IvwSerializer& s) const {
	s.serialize("type", getClassName(), true);
	s.serialize("modifier", modifierName_);
}

void InteractionEvent::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
	d.deserialize("modifier", modifierName_);
	for (size_t i = 0; i < COUNT; ++i) {
		if (modifierNames_[i] == modifierName_) {
			modifier_ = static_cast<InteractionEvent::Modifier>(i);
			break;
		}
	}
}

} // namespace