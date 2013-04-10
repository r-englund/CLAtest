#ifndef IVW_EVENTPROPERTY_H
#define IVW_EVENTPROPERTY_H

#include <inviwo/core/interaction/action.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {
class IVW_CORE_API EventProperty : public Property {
public:
    EventProperty(std::string identifier, 
        std::string displayName,
        Event e,
        Action action,
        PropertySemantics::Type semantics = PropertySemantics::Default);

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    Event getEvent() { return event_; }
    Action getAction() { return action_; }

    void setEvent(Event e) { event_ = e; }
    void setAction(Action a) { action_ = a; }

private:
    Event event_;
    Action action_;
};
} // namespace

#endif // IVW_EVENTPROPERTY_H

