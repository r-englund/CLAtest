#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/canvasprocessor.h>

namespace inviwo {

CanvasProcessor::CanvasProcessor()
    : Processor(),
    inport_("inport"),
	snapshot_("snapshot", "Create Snapshot"),
    canvas_(0)
{
    addPort(inport_);
	addProperty(snapshot_);
}


CanvasProcessor::~CanvasProcessor() {
    if (processorWidget_) {
        delete processorWidget_;
        processorWidget_ = 0;
    }
}

void CanvasProcessor::takeSnapshot(const char *filePath){
    const Image* img = inport_.getData();
    snapshot_.setImage(inport_.getData());
    if(img != NULL)
        snapshot_.saveSnapshot(filePath, img);
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

    if (canvas_) canvas_->activate();
}

} // namespace
