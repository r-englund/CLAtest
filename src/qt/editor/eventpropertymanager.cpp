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

// For testing
void EventPropertyManager::print(){
	if (eventProperties_.size() > 0) {
		std::cout << "----" << std::endl;	
		for (size_t i = 0; i < eventProperties_.size(); ++i)
			std::cout << eventProperties_.at(i)->getDisplayName() << std::endl;
	}
}

} // namespace