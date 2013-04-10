#include <inviwo/core/interaction/action.h>

namespace inviwo {
Action::Action() {};
Action::~Action() {};

void Action::serialize( IvwSerializer& s ) const {}
void Action::deserialize( IvwDeserializer& s ) {}

} // namespace