#ifndef IVW_INTERACTIONHANDLER_H
#define IVW_INTERACTIONHANDLER_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/interaction/events/eventlistener.h>

namespace inviwo {

    class IVW_CORE_API InteractionHandler : public EventListener {

    public:
        InteractionHandler();
        virtual ~InteractionHandler();
    };

} // namespace

#endif // IVW_INTERACTIONHANDLER_H