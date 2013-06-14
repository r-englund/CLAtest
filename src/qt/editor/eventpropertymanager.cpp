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
	std::vector<EventProperty*> eventProperties = eventPropertyMap_[activeProcessor_];
	for (size_t i = 0; i < eventProperties.size(); ++i) {
		if (eventProperties.at(i)->getEvent().button() == button) {
			eventProperties.at(i)->setEvent(MouseEvent(MouseEvent::MOUSE_BUTTON_NONE, Event::MODIFIER_NONE));
		}
	}
	
	// Do the remapping
	for (size_t i = 0; i < eventProperties.size(); ++i) {
		if (eventProperty->getIdentifier() == eventProperties.at(i)->getIdentifier()) {
			eventProperties.at(i)->setEvent(MouseEvent(button, Event::MODIFIER_NONE));
			break;
		}
	}
}

void EventPropertyManager::setEventPropertyMap(std::map<std::string, std::vector<EventProperty*>> eventPropertyMap) {
	eventPropertyMap_ = eventPropertyMap;
}

std::vector<EventProperty*> EventPropertyManager::getEventPropertiesFromMap() {
	std::map<std::string, std::vector<EventProperty*>>::iterator it;
	it = eventPropertyMap_.find(activeProcessor_);
	return it->second;
}

} // namespace