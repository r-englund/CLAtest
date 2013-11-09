#include "canvasprocessorgl.h"
#include "canvasgl.h"

namespace inviwo {

ProcessorClassName(CanvasProcessorGL, "CanvasGL"); 
ProcessorCategory(CanvasProcessorGL, "Data Output");
ProcessorCodeState(CanvasProcessorGL, CODE_STATE_STABLE);

CanvasProcessorGL::CanvasProcessorGL()
    : CanvasProcessor()
{}

void CanvasProcessorGL::initialize() {
    CanvasProcessor::initialize();
}

void CanvasProcessorGL::deinitialize() {
    CanvasProcessor::deinitialize();
}

void CanvasProcessorGL::process() {
    CanvasProcessor::process();
    static_cast<CanvasGL*>(getCanvas())->render(inport_.getData(), visibleLayer_.get());
}

} // namespace
