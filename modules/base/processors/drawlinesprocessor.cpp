#include "drawlinesprocessor.h"

namespace inviwo {

ProcessorClassName(DrawLinesProcessor, "DrawLines"); 
ProcessorCategory(DrawLinesProcessor, "Line Rendering");
ProcessorCodeState(DrawLinesProcessor, CODE_STATE_EXPERIMENTAL); 

DrawLinesProcessor::DrawLinesProcessor()
    : CompositeProcessorGL(),
      inport_("inport"),
      outport_("outport", &inport_, COLOR_ONLY)
{
    addPort(inport_);
    addPort(outport_);
}

DrawLinesProcessor::~DrawLinesProcessor() {}

void DrawLinesProcessor::initialize() {
    CompositeProcessorGL::initialize();

    lines_ = new Mesh();
    lines_->initialize();
}

void DrawLinesProcessor::deinitialize() {
    CompositeProcessorGL::deinitialize();
}

void DrawLinesProcessor::process() {  
    activateAndClearTarget(outport_);

    deactivateCurrentTarget();

    compositePortsToOutport(outport_, inport_);
}

} // namespace
