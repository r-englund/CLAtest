#include "inviwo/core/util/canvas.h"

namespace inviwo {

EventHandler* eventHandler_();

ProcessorNetworkEvaluator* Canvas::processorNetworkEvaluator_ = 0;

Canvas::Canvas(ivec2 dimensions)
               : dimensions_(dimensions) {
}

Canvas::~Canvas() {}

void Canvas::initialize() {}

void Canvas::deinitialize() {}

void Canvas::repaint() {}

void Canvas::switchContext() {}

void Canvas::resize(ivec2 size) {
    dimensions_ = size;
}

void Canvas::update() {}


} // namespace
