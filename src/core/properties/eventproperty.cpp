#include <inviwo/core/properties/eventproperty.h>

namespace inviwo {

EventProperty::EventProperty(std::string identifier, std::string displayName, Event* e, Action* action,
                             PropertyOwner::InvalidationLevel invalidationLevel,
                             PropertySemantics::Type semantics  )
    : Property(identifier, displayName, invalidationLevel, semantics) {
        event_ = e;
        action_ = action;
}

void EventProperty::serialize( IvwSerializer& s ) const {
    Property::serialize(s);
	s.serialize("Event", event_);
	s.serialize("Action", action_);
}

void EventProperty::deserialize( IvwDeserializer& d ) {
    Property::deserialize(d);
	d.deserialize("Event", event_);
	d.deserialize("Action", action_);
}

} // namespace