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
    uvec2 previousDimensions = dimensions_;
    dimensions_ = size;
    if (processorNetworkEvaluator_) {
        ResizeEvent* resizeEvent = new ResizeEvent(dimensions_);
        resizeEvent->setPreviousSize(previousDimensions);        
        processorNetworkEvaluator_->propagateResizeEvent(this, resizeEvent);
        processorNetworkEvaluator_->evaluate();
    }
}

void Canvas::update() {}

} // namespace
