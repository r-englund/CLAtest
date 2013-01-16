#ifndef IVW_KEYBOARDEVENT_H
#define IVW_KEYBOARDEVENT_H

#include "inviwo/core/interaction/events/event.h"

namespace inviwo {

    class KeyboardEvent : public Event {

    public:
        KeyboardEvent();
        ~KeyboardEvent();
    };

} // namespace

#endif // IVW_KEYBOARDEVENT_H