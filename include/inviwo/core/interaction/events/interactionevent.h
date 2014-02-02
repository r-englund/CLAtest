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

#ifndef IVW_INTERACTIONEVENT_H
#define IVW_INTERACTIONEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

class IVW_CORE_API InteractionEvent : public Event {
public:
    enum Modifier {
        MODIFIER_NONE  =      0,
        MODIFIER_ALT,
        MODIFIER_CTRL,
        MODIFIER_SHIFT,
        COUNT
    };

    InteractionEvent();
    virtual ~InteractionEvent();

    inline int button() const { return button_; }
    inline InteractionEvent::Modifier modifier() const { return modifier_; }
    inline std::string modifierName() const { return modifierName_; }
    inline std::string buttonName() const { return buttonName_; }

    virtual std::string getClassName() const { return "Undefined"; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    InteractionEvent::Modifier modifier_;
    int button_;
    std::string modifierName_;
    std::string buttonName_;
    std::string modifierNames_[COUNT];
};

} // namespace

#endif // IVW_INTERACTIONEVENT_H