#include "inviwo/core/processors/canvasprocessor.h"

namespace inviwo {

    CanvasProcessor::CanvasProcessor()
        : Processor(),
        inport_(Port::INPORT, "inport")
    {
        addPort(inport_);
    }

    void CanvasProcessor::initialize() {
        Processor::initialize();
        //canvas_->initialize();
    }
    
    void CanvasProcessor::deinitialize() {
        //canvas_->deinitialize();
        Processor::deinitialize();
    }

} // namespace
