#include <inviwo/core/interaction/action.h>

namespace inviwo {

Action::Action() {};
Action::~Action() {};

void Action::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
}

void Action::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
}

} // namespace