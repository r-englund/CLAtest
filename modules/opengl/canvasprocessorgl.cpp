#include "canvasprocessorgl.h"
#include "canvasgl.h"

namespace inviwo {

CanvasProcessorGL::CanvasProcessorGL()
    : CanvasProcessor(),
      dimensions_("dimensions", "Dimensions", ivec2(256,256), ivec2(128,128), ivec2(4096,4096))
{
    dimensions_.onChange(this, &CanvasProcessorGL::resizeCanvas);
    addProperty(dimensions_);
}

Processor* CanvasProcessorGL::create() const {
    return new CanvasProcessorGL();
}

void CanvasProcessorGL::resizeCanvas() {
    //std::cout << "onChange" << std::endl;
    //getCanvas()->resize(uvec2(dimensions_.get()));
}

void CanvasProcessorGL::initialize() {
    CanvasProcessor::initialize();
}

void CanvasProcessorGL::deinitialize() {
    CanvasProcessor::deinitialize();
}

void CanvasProcessorGL::process() {
    CanvasProcessor::process();
    static_cast<CanvasGL*>(getCanvas())->render(inport_.getData());
}

} // namespace
