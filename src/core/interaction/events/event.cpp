#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

Event::Event() {}
Event::~Event() {}

void Event::serialize(IvwSerializer& s) const {}
void Event::deserialize(IvwDeserializer& d) {}

} // namespace