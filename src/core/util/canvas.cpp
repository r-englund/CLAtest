#include <inviwo/core/util/canvas.h>

namespace inviwo {

EventHandler* eventHandler_();

Canvas::Canvas(uvec2 dimensions)
               : dimensions_(dimensions),
                 processorNetworkEvaluator_(0)
{}

Canvas::~Canvas() {}

void Canvas::initialize() {}

void Canvas::deinitialize() {}

void Canvas::render(const Image* im){}

void Canvas::repaint() {}

void Canvas::activate() {}

void Canvas::resize(uvec2 size) {
    dimensions_ = size;
}

void Canvas::update() {}


} // namespace
