#ifndef IVW_EVENTCONVERTERQT_H
#define IVW_EVENTCONVERTERQT_H

#include <QMouseEvent>
#include <QInputEvent>
#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

class EventConverterQt {
public:
    EventConverterQt();
    ~EventConverterQt();

    static MouseEvent::MouseButton getMouseButton(QMouseEvent* e);
    static Event::Modifier getModifier(QInputEvent* e);
	static char getKeyButton(QKeyEvent* e);
};
} // namespace
#endif // IVW_EVENTCONVERTERQT_H