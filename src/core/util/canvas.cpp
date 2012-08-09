#include "inviwo/core/util/canvas.h"

namespace inviwo {

EventHandler* eventHandler_();

Canvas::Canvas(ivec2 dimensions)
               : dimensions_(dimensions) {
}

Canvas::~Canvas() {}

void Canvas::initialize() {}

void Canvas::deinitialize() {}

} // namespace
