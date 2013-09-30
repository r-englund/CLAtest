#include "positionwidgetprocessor.h"

namespace inviwo {

ProcessorClassName(PositionWidgetProcessor, "PositionWidget"); 
ProcessorCategory(PositionWidgetProcessor, "Geometry Rendering");
ProcessorCodeState(PositionWidgetProcessor, CODE_STATE_EXPERIMENTAL); 

PositionWidgetProcessor::PositionWidgetProcessor()
    : ProcessorGL(),
      inport_("inport"),
      outport_("outport", &inport_, COLOR_DEPTH_PICKING),
      widgetType_("widgetType", "Widget Type"),
      position_("position", "Position", vec3(0.0f), vec3(-1.f), vec3(1.f))      
{
    addPort(inport_);
    addPort(outport_);

    widgetType_.addOption("cube", "Cube");
    widgetType_.addOption("sphere", "Sphere");
    widgetType_.set("sphere");
    addProperty(widgetType_);

    addProperty(position_);
}

PositionWidgetProcessor::~PositionWidgetProcessor() {}

void PositionWidgetProcessor::initialize() {
    ProcessorGL::initialize();
}

void PositionWidgetProcessor::deinitialize() {
    ProcessorGL::deinitialize();
}

void PositionWidgetProcessor::process() {    
    activateTarget(outport_);

    renderImagePlaneRect();

    deactivateCurrentTarget();
    
}

} // namespace
