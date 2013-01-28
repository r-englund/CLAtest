#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

    MouseEvent::MouseEvent(ivec2 position, MouseEvent::MouseButton button,
                           MouseEvent::MouseState state, Event::Modifier modifier, ivec2 canvasSize)
        : Event(),
          position_(position),
          button_(button),
          state_(state),
          modifier_(modifier),
          canvasSize_(canvasSize) {}

    MouseEvent::~MouseEvent() {}

} // namespace