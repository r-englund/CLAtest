#include <inviwo/qt/editor/eventpropertymanager.h>

namespace inviwo {

EventPropertyManager::EventPropertyManager() {}
EventPropertyManager::~EventPropertyManager() {}

std::vector<EventProperty*> EventPropertyManager::getEventProperties() {
	return eventProperties_;
}

void EventPropertyManager::setEventProperties( std::vector<EventProperty*> eventProperties ) {
	eventProperties_ = eventProperties;
}

void EventPropertyManager::changeKeybinding( EventProperty* eventProperty, MouseEvent::MouseButton button ) {
	// Look for event conflicts
	for (size_t i = 0; i < eventProperties_.size(); ++i) {
		if (eventProperties_.at(i)->getEvent().button() == button) {
			eventProperties_.at(i)->setEvent(MouseEvent(MouseEvent::MOUSE_BUTTON_NONE, Event::MODIFIER_NONE));
		}
	}
	
	// Do the remapping
	for (size_t i = 0; i < eventProperties_.size(); ++i) {
		if (eventProperty->getIdentifier() == eventProperties_.at(i)->getIdentifier()) {
			eventProperties_.at(i)->setEvent(MouseEvent(button, Event::MODIFIER_NONE));
			break;
		}
	}
}

} // namespace