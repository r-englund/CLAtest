#include "inviwo/core/interaction/events/eventhandler.h"

namespace inviwo {

    EventHandler::EventHandler() {
        eventListeners_.clear();
    }

    EventHandler::~EventHandler() {
    }

    void EventHandler::broadcast(Event* event) {
        for (size_t i=0; i<eventListeners_.size(); i++)
            eventListeners_[i]->invokeEvent(event);
    }

} // namespace