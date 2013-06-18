#ifndef IVW_EVENTPROPERTY_H
#define IVW_EVENTPROPERTY_H

#include <inviwo/core/interaction/action.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {

/** class EventProperty
* 
* Property which contains one event and one action to represent the current key binding for the contained action.
* @see EventPropertyWidgetQt
*/
class IVW_CORE_API EventProperty : public Property {
public:
    /** 
     * \brief Constructor used to create a new action-key binding.
     *
     * The constructor creates a new binding between a specified action and event.
     * 
     * @param std::string identifier 
     * @param std::string displayName 
     * @param Event e The key or mouse event to bind to an action
     * @param Action action The action to be bound to an event
     * @param PropertySemantics::Type semantics 
     */
    EventProperty(std::string identifier, 
        std::string displayName,
        Event* e,
        Action* action,
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
        PropertySemantics::Type semantics = PropertySemantics::Default);

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

	std::string getClassName()  const { return "EventProperty"; }

    Event* getEvent() const { return event_; }    
    Action* getAction() const { return action_; }

    /** 
     * \brief Maps action to new event.
     *
     * Changes the current action-to-key binding by replacing the old event with a new.
     * 
     * @param Event e The new event
     */
    void setEvent(Event* e) { event_ = e; }

private:
    Event* event_;
    Action* action_;
};

} // namespace

#endif // IVW_EVENTPROPERTY_H

