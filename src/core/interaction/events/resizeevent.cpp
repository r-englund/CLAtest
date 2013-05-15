#include <inviwo/core/interaction/events/resizeevent.h>

namespace inviwo {

    ResizeEvent::ResizeEvent( uvec2 canvasSize)
        : Event(),
          size_(canvasSize) {}

    ResizeEvent::~ResizeEvent() {}

} // namespace