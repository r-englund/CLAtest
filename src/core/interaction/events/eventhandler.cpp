#include <inviwo/core/interaction/events/eventhandler.h>
#include <algorithm>

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

bool EventHandler::addEventListener(EventListener* listener){
    std::vector<EventListener*>::iterator it = std::find(eventListeners_.begin(), eventListeners_.end(), listener);
    if(it == eventListeners_.end()){
        eventListeners_.push_back(listener);
        return true;
    }
    return false;
}

bool EventHandler::removeEventListener(EventListener* listener){
    std::vector<EventListener*>::iterator it = std::find(eventListeners_.begin(), eventListeners_.end(), listener);
    if(it != eventListeners_.end()){
        eventListeners_.erase(it);
        return true;
    }
    return false;
}

} // namespace