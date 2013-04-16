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

    MouseEvent::MouseButton getMouseButton(QMouseEvent* e);
    Event::Modifier getModifier(QInputEvent* e);
};
} // namespace
#endif // IVW_EVENTCONVERTERQT_H