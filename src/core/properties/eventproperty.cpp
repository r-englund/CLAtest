#include <inviwo/core/properties/eventproperty.h>

namespace inviwo {

EventProperty::EventProperty(std::string identifier, std::string displayName, Event e, Action action,
                             PropertyOwner::InvalidationLevel invalidationLevel,
                             PropertySemantics::Type semantics  )
    : Property(identifier, displayName, invalidationLevel, semantics) {
        event_ = e;
        action_ = action;
}

void EventProperty::serialize( IvwSerializer& s ) const {
    Property::serialize(s);
	s.serialize(getIdentifier(), event_);
	s.serialize(getIdentifier(), action_);
}

void EventProperty::deserialize( IvwDeserializer& d ) {
    Property::deserialize(d);
	d.deserialize(getIdentifier(), event_);
	d.deserialize(getIdentifier(), action_);
}

} // namespace