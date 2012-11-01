#include "inviwo/core/interaction/events/resizeevent.h"

namespace inviwo {

    ResizeEvent::ResizeEvent( ivec2 canvasSize)
        : Event(),
          canvasSize_(canvasSize) {}

    ResizeEvent::~ResizeEvent() {}

} // namespace