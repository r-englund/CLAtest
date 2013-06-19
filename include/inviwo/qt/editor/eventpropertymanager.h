#ifndef IVW_EVENTPROPERTYMANAGER_H
#define IVW_EVENTPROPERTYMANAGER_H

#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <vector>

namespace inviwo {

class EventPropertyManager {

public:
	EventPropertyManager();
	~EventPropertyManager();

	std::vector<EventProperty*> getEventPropertiesFromMap();

	void setEventPropertyMap(std::map<std::string, std::vector<EventProperty*> > eventPropertyMap);
	void setActiveProcessor(std::string processorIdentifier){ activeProcessor_ = processorIdentifier; };

	void changeMouseMapping(EventProperty* eventProperty, MouseEvent::MouseButton button, Event::Modifier modifier);
	void changeKeyMapping(EventProperty* eventProperty, char button, Event::Modifier modifier);

private:
	std::map<std::string, std::vector<EventProperty*> > eventPropertyMap_;
	std::string activeProcessor_;
};

} // namespace

#endif // IVW_EVENTPROPERTYMANAGER_H