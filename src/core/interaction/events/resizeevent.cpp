#include <inviwo/core/interaction/events/resizeevent.h>

namespace inviwo {

    ResizeEvent::ResizeEvent( uvec2 canvasSize)
        : Event(),
          canvasSize_(canvasSize) {}

    ResizeEvent::~ResizeEvent() {}

} // namespace