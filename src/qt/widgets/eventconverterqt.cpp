#include <inviwo/qt/widgets/eventconverterqt.h>

namespace inviwo {

EventConverterQt::EventConverterQt() {}
EventConverterQt::~EventConverterQt() {}

MouseEvent::MouseButton inviwo::EventConverterQt::getMouseButton( QMouseEvent* e ) {
    if (e->buttons() == Qt::LeftButton) {
        return MouseEvent::MOUSE_BUTTON_LEFT;
    } else if (e->buttons() == Qt::RightButton) {
        return MouseEvent::MOUSE_BUTTON_RIGHT;
    } else if (e->buttons() == Qt::MiddleButton) {
        return MouseEvent::MOUSE_BUTTON_MIDDLE;
    }
    return MouseEvent::MOUSE_BUTTON_NONE;
}

Event::Modifier inviwo::EventConverterQt::getModifier( QInputEvent* e ) {
    if (e->modifiers() == Qt::ShiftModifier) {
        return Event::MODIFIER_SHIFT;
    } else if (e->modifiers() == Qt::ControlModifier) {
        return Event::MODIFIER_CTRL;
    } else if (e->modifiers() == Qt::AltModifier) {
        return Event::MODIFIER_ALT;
    }
    return Event::MODIFIER_NONE;
}

} // namespace