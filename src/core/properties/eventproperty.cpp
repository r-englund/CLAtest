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
    // TODO
    Property::serialize(s);
}

void EventProperty::deserialize( IvwDeserializer& d ) {
    // TODO
    Property::deserialize(d);
}

} // namespace