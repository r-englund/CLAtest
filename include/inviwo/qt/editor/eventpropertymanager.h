#ifndef IVW_EVENTPROPERTYMANAGER_H
#define IVW_EVENTPROPERTYMANAGER_H

#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QWidget>
#include <vector>

namespace inviwo {

class EventPropertyManager : public QWidget {

public:
	EventPropertyManager();
	~EventPropertyManager();

	std::vector<EventProperty*> getEventPropertiesFromMap();

	void setEventPropertyMap(std::map<std::string, std::vector<EventProperty*> > eventPropertyMap);
	void setActiveProcessor(std::string processorIdentifier){ activeProcessor_ = processorIdentifier; };

	void changeKeybinding(EventProperty* eventProperty, MouseEvent::MouseButton button);

private:
	std::map<std::string, std::vector<EventProperty*> > eventPropertyMap_;
	std::string activeProcessor_;
};

} // namespace

#endif // IVW_EVENTPROPERTYMANAGER_H