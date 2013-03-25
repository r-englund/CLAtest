#include <inviwo/core/properties/eventproperty.h>

namespace inviwo {

    EventProperty::EventProperty( std::string identifier, std::string displayName, Event* e, Action* action, PropertySemantics::Type semantics /*= PropertySemantics::Default*/ )
        : Property(identifier, displayName, semantics) {
            event_ = e;
            action_ = action;
    }

    void EventProperty::serialize( IvwSerializer& s ) const {
        Property::serialize(s);
    }

    void EventProperty::deserialize( IvwDeserializer& d ) {
        Property::deserialize(d);
    }


} // namespace