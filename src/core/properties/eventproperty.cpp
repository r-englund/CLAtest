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

#include <inviwo/core/properties/eventproperty.h>

namespace inviwo {

EventProperty::EventProperty(std::string identifier, std::string displayName, InteractionEvent* e, Action* action,
                             PropertyOwner::InvalidationLevel invalidationLevel,
                             PropertySemantics semantics)
    : Property(identifier, displayName, invalidationLevel, semantics) {
    event_ = e;
    action_ = action;
}

EventProperty::~EventProperty() {
}

void EventProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    s.serialize("Event", event_);
    s.serialize("Action", action_);
}

void EventProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    d.deserialize("Event", *event_);
    d.deserialize("Action", *action_);
}

} // namespace