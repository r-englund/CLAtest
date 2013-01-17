#include "inviwo/core/inviwoapplication.h"
#include "inviwo/core/processors/canvasprocessor.h"
#include "inviwo/qt/widgets/inviwoapplicationqt.h"

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

void CanvasProcessor::createProcessorWidget() {
    InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
    if (app) {
        QWidget* parent = app->getMainWindow();
        processorWidget_ = new CanvasProcessorWidget(this, parent);
    }
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
