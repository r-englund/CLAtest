#include <inviwo/core/interaction/action.h>

namespace inviwo {

Action::Action() {};
Action::~Action() {};

void Action::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void Action::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
}

} // namespace