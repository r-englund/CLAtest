#ifndef IVW_TIMEREVENT_H
#define IVW_TIMEREVENT_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/interaction/events/event.h"

namespace inviwo {

    class IVW_CORE_API TimerEvent : public Event {

    public:
        TimerEvent();
        virtual ~TimerEvent();
    };

} // namespace

#endif // IVW_TIMEREVENT_H