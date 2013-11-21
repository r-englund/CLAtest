#include <inviwo/qt/widgets/eventpropertymanager.h>

namespace inviwo {

EventPropertyManager::EventPropertyManager() : VoidObservable() {}
EventPropertyManager::~EventPropertyManager() {}

// Remap with a mouse event
void EventPropertyManager::changeMouseMapping( EventProperty* eventProperty, MouseEvent::MouseButton button, InteractionEvent::Modifier modifier) {
	if (dynamic_cast<MouseEvent*>(eventProperty->getEvent())) {
		// Look for event conflicts
		std::vector<EventProperty*> eventProperties = eventPropertyMap_[activeProcessor_];
		for (size_t i = 0; i < eventProperties.size(); ++i) {
			if (eventProperties.at(i)->getEvent()->button() == button && eventProperties.at(i)->getEvent()->modifier() == modifier) {
				eventProperties.at(i)->setEvent(new MouseEvent(MouseEvent::MOUSE_BUTTON_NONE, InteractionEvent::MODIFIER_NONE));
			}
		}
		
		// Do the remapping
		for (size_t i = 0; i < eventProperties.size(); ++i) {
			if (eventProperty->getIdentifier() == eventProperties.at(i)->getIdentifier()) {
				eventProperties.at(i)->setEvent(new MouseEvent(button, modifier));
				break;
			}
		}		
	} else {
		LogWarn("Can only be bound with a keyboard event");
	}
	notifyObservers();
}

// Remap with a keyboard event
void EventPropertyManager::changeKeyMapping( EventProperty* eventProperty, char button, InteractionEvent::Modifier modifier) {
	if (dynamic_cast<KeyboardEvent*>(eventProperty->getEvent())) {
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
				eventProperties.at(i)->setEvent(new KeyboardEvent(button, modifier, KeyboardEvent::KEY_STATE_PRESS));
				break;
			}
		}		
	} else {
		LogWarn("Can only be bound with a mouse event");
	}
	notifyObservers();
}

void EventPropertyManager::setEventPropertyMap(std::map<std::string, std::vector<EventProperty*> > eventPropertyMap) {
	eventPropertyMap_ = eventPropertyMap;
}

std::vector<EventProperty*> EventPropertyManager::getEventPropertiesFromMap() {
	std::map<std::string, std::vector<EventProperty*> >::iterator it;
	it = eventPropertyMap_.find(activeProcessor_);
	return it->second;
}

void EventPropertyManager::setActiveProcessor(std::string processorIdentifier) {
	activeProcessor_ = processorIdentifier;
	notifyObservers();
}

bool EventPropertyManager::isEmpty() {
	if (eventPropertyMap_.size() > 0) {
		return false;
	} else {
		return true;
	}
}

} // namespace