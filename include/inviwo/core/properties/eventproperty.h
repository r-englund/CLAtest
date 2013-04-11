#ifndef IVW_EVENTPROPERTY_H
#define IVW_EVENTPROPERTY_H

#include <inviwo/core/interaction/action.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/properties/property.h>


namespace inviwo {
class IVW_CORE_API EventProperty : public Property {
public:
    /** 
     * \brief Constructor.
     *
     * Constructor taking identifier, displayname, key or mouse event and the action to be mapped.
     * 
     * @param std::string identifier 
     * @param std::string displayName 
     * @param Event e The key or mouse event to bind to an action
     * @param Action action The action to be bound to an event
     * @param PropertySemantics::Type semantics 
     */EventProperty(std::string identifier, 
        std::string displayName,
        Event e,
        Action action,
        PropertySemantics::Type semantics = PropertySemantics::Default);

    virtual void serialize(IvwSerializer& s) const; //TODO
    virtual void deserialize(IvwDeserializer& d); //TODO

    /** 
     * \brief Gets the event.
     *
     * Gets the event which is bound to this property.
     * 
     * @return inviwo::Event Key or mouse event
     */Event getEvent() { return event_; }
    
    /** 
     * \brief Gets the action.
     *
     * Gets the action which is bound to this property.
     * 
     * @return inviwo::Action The bound action
     */Action getAction() { return action_; }

    /** 
     * \brief Maps action to new event.
     *
     * Takes in a new event to remap to the action of this property.
     * 
     * @param Event e The new event
     */void setEvent(Event e) { event_ = e; }

    // FOR TESTING
    void tmpTest(int i) { std::cout << "QMouseEvent: " << i << std::endl; }

private:
    Event event_;
    Action action_;
};
} // namespace

#endif // IVW_EVENTPROPERTY_H

