#ifndef IVW_EVENTLISTENER_H
#define IVW_EVENTLISTENER_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"
#include "inviwo/core/interaction/events/event.h"

namespace inviwo {

    class IVW_CORE_API EventListener {

    public:
        EventListener();
        virtual ~EventListener();

        virtual void invokeEvent(Event* event);
    };

} // namespace

#endif // IVW_EVENTLISTENER_H