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

DrawLinesProcessor::~DrawLinesProcessor() {
}

void DrawLinesProcessor::initialize() {
    CompositeProcessorGL::initialize();

    lines_ = new Mesh(LINES, STRIP);
    lines_->initialize();
    lines_->addAttribute(new Position2dBuffer());

    addPoint(vec2(0.25f));
    addPoint(vec2(0.75f));

    lineRenderer_ = new MeshGLRenderer(lines_);
}

void DrawLinesProcessor::deinitialize() {
    CompositeProcessorGL::deinitialize();

    delete lineRenderer_;
    delete lines_;
}

void DrawLinesProcessor::process() {  
    activateAndClearTarget(outport_);

    lineRenderer_->render();

    deactivateCurrentTarget();

    compositePortsToOutport(outport_, inport_);
}

void DrawLinesProcessor::addPoint(vec2 p){
    lines_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->add(p);
}

void DrawLinesProcessor::clearPoints(){
    lines_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->clear();
}

} // namespace
