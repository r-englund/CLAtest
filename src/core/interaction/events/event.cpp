#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

    Event::Event() {
        modifierNames_[MODIFIER_NONE] = "";
        modifierNames_[MODIFIER_ALT] = "Alt + ";
        modifierNames_[MODIFIER_CTRL] = "Ctrl + ";
        modifierNames_[MODIFIER_SHIFT] = "Shift + ";
    }
    Event::~Event() {}

} // namespace