#include <inviwo/qt/editor/eventpropertymanager.h>

namespace inviwo {

EventPropertyManager::EventPropertyManager() {}
EventPropertyManager::~EventPropertyManager() {}

// Remap with a mouse event
void EventPropertyManager::changeMouseMapping( EventProperty* eventProperty, MouseEvent::MouseButton button, Event::Modifier modifier) {
	// Look for event conflicts
	std::vector<EventProperty*> eventProperties = eventPropertyMap_[activeProcessor_];
	for (size_t i = 0; i < eventProperties.size(); ++i) {
		if (eventProperties.at(i)->getEvent()->button() == button && eventProperties.at(i)->getEvent()->modifier() == modifier) {
			eventProperties.at(i)->setEvent(new MouseEvent(MouseEvent::MOUSE_BUTTON_NONE, Event::MODIFIER_NONE));
		}
	}
	
	// Do the remapping
	for (size_t i = 0; i < eventProperties.size(); ++i) {
		if (eventProperty->getIdentifier() == eventProperties.at(i)->getIdentifier()) {
			eventProperties.at(i)->setEvent(new MouseEvent(button, modifier));
			break;
		}
	}
}

// Remap with a keyboard event
void EventPropertyManager::changeKeyMapping( EventProperty* eventProperty, char button, Event::Modifier modifier) {
	// Look for event conflicts
	std::vector<EventProperty*> eventProperties = eventPropertyMap_[activeProcessor_];
	for (size_t i = 0; i < eventProperties.size(); ++i) {
		if (eventProperties.at(i)->getEvent()->button() == button && eventProperties.at(i)->getEvent()->modifier() == modifier) {
			eventProperties.at(i)->setEvent(new KeyboardEvent());
		}
	}

	// Do the remapping
	for (size_t i = 0; i < eventProperties.size(); ++i) {
		if (eventProperty->getIdentifier() == eventProperties.at(i)->getIdentifier()) {
			eventProperties.at(i)->setEvent(new KeyboardEvent(button, modifier));
			break;
		}
	}
}

void EventPropertyManager::setEventPropertyMap(std::map<std::string, std::vector<EventProperty*> > eventPropertyMap) {
	eventPropertyMap_ = eventPropertyMap;
}

std::vector<EventProperty*> EventPropertyManager::getEventPropertiesFromMap() {
	std::map<std::string, std::vector<EventProperty*> >::iterator it;
	it = eventPropertyMap_.find(activeProcessor_);
	return it->second;
}

} // namespace