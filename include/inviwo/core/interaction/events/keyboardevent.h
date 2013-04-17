#ifndef IVW_KEYBOARDEVENT_H
#define IVW_KEYBOARDEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

class IVW_CORE_API KeyboardEvent : public Event {
public:
    KeyboardEvent();
    ~KeyboardEvent();
};

} // namespace

#endif // IVW_KEYBOARDEVENT_H