/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Hans-Christian Helltegen
 *
 **********************************************************************/

#ifndef IVW_EVENTPROPERTY_H
#define IVW_EVENTPROPERTY_H

#include <inviwo/core/interaction/action.h>
#include <inviwo/core/interaction/events/interactionevent.h>
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
     * @param PropertySemantics semantics
     */
    EventProperty(std::string identifier,
                  std::string displayName,
                  InteractionEvent* e,
                  Action* action,
                  PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                  PropertySemantics semantics = PropertySemantics::Default);

    virtual ~EventProperty();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    std::string getClassName()  const { return "EventProperty"; }

    InteractionEvent* getEvent() const { return event_; }
    Action* getAction() const { return action_; }

    /**
     * \brief Maps action to new event.
     *
     * Changes the current action-to-key binding by replacing the old event with a new.
     *
     * @param Event e The new event
     */
    void setEvent(InteractionEvent* e) { event_ = e; }

private:
    InteractionEvent* event_;
    Action* action_;
};

} // namespace

#endif // IVW_EVENTPROPERTY_H

