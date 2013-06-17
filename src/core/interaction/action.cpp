#include <inviwo/core/interaction/action.h>

namespace inviwo {

Action::Action() {};
Action::~Action() {};

void Action::serialize( IvwSerializer& s ) const {
	s.serialize("action", action_);
	s.serialize("actionName", actionName_);
}
void Action::deserialize( IvwDeserializer& s ) {
	s.deserialize("action", action_);
	s.deserialize("actionName", actionName_);
}

} // namespace