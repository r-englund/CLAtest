#include "inviwo/core/inviwoapplication.h"
#include "inviwo/core/processors/canvasprocessor.h"
#include "inviwo/qt/widgets/inviwoapplicationqt.h"

namespace inviwo {

CanvasProcessor::CanvasProcessor()
    : Processor(),
    inport_(Port::INPORT, "inport")
{
    addPort(inport_);
}

Processor* CanvasProcessor::create() const {
    return new CanvasProcessor();
}

void CanvasProcessor::createProcessorWidget() {
    InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::app());
    if (app) {
        QWidget* parent = app->getMainWindow();
        processorWidget_ = new CanvasProcessorWidget(this, parent);
    }
}

void CanvasProcessor::initialize() {
    Processor::initialize();
    //canvas_->initialize();
}

void CanvasProcessor::deinitialize() {
    //canvas_->deinitialize();
    Processor::deinitialize();
}

void CanvasProcessor::process() {
    Processor::process();
}

} // namespace
