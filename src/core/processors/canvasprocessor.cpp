#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/processors/canvasprocessor.h>

namespace inviwo {

CanvasProcessor::CanvasProcessor()
    : Processor(),
    inport_(Port::INPORT, "inport"),
    canvas_(0)
{
    addPort(inport_);
}


CanvasProcessor::~CanvasProcessor() {
    if(processorWidget_) {
        delete processorWidget_;
        processorWidget_ = 0;
    }    
}

Processor* CanvasProcessor::create() const {
    return new CanvasProcessor();
}

void CanvasProcessor::initialize() {
    Processor::initialize();
}

void CanvasProcessor::deinitialize() {
    processorWidget_->hide();
    delete processorWidget_;
    processorWidget_ = 0;
    Processor::deinitialize();
}

void CanvasProcessor::process() {
    Processor::process();
    if(canvas_) canvas_->switchContext();
}

} // namespace
