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

	std::vector<EventProperty*> getEventProperties();
	void setEventProperties(std::vector<EventProperty*> eventProperties);
	void changeKeybinding(EventProperty* eventProperty, MouseEvent::MouseButton button);

private:
	std::vector<EventProperty*> eventProperties_;
};

} // namespace

#endif // IVW_EVENTPROPERTYMANAGER_H