#include "inviwo/core/interaction/events/eventlistener.h"

namespace inviwo {

    EventListener::EventListener() {}

    EventListener::~EventListener() {
    }

    void EventListener::invokeEvent(Event* /*event*/) {
        std::cout << "EventListener::invokeEvent()" << std::endl;
    }

} // namespace