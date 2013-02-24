#ifndef IVW_EVENTHANDLER_H
#define IVW_EVENTHANDLER_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/interaction/events/eventlistener.h>

namespace inviwo {

    class IVW_CORE_API EventHandler {

    public:
        EventHandler();
        virtual ~EventHandler();

        void broadcast(Event* event);

    private:
        std::vector<EventListener*> eventListeners_;        
    };

} // namespace

#endif // IVW_EVENTHANDLER_H