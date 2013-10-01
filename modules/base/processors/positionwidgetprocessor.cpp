#include "positionwidgetprocessor.h"
#include <inviwo/core/interaction/pickingmanager.h>
#include <inviwo/core/datastructures/geometry/basemeshcreator.h>

namespace inviwo {

ProcessorClassName(PositionWidgetProcessor, "PositionWidget"); 
ProcessorCategory(PositionWidgetProcessor, "Geometry Rendering");
ProcessorCodeState(PositionWidgetProcessor, CODE_STATE_EXPERIMENTAL); 

PositionWidgetProcessor::PositionWidgetProcessor()
    : ProcessorGL(),
      inport_("inport"),
      outport_("outport", COLOR_DEPTH_PICKING),
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

    widgetPickingObject_ = PickingManager::instance()->registerPickingCallback(this, &PositionWidgetProcessor::updateWidgetPositionFromPicking);

    vec3 posLLF = vec3(0.5f);
    vec3 posURB = vec3(1.5f);

    widget_ = new Geometry(BaseMeshCreator::rectangularPrism(posLLF, posURB, posLLF, posURB, vec4(1.0f), vec4(1.0f)));
}

PositionWidgetProcessor::~PositionWidgetProcessor() {}

void PositionWidgetProcessor::initialize() {
    ProcessorGL::initialize();
}

void PositionWidgetProcessor::deinitialize() {
    ProcessorGL::deinitialize();
}

void PositionWidgetProcessor::updateWidgetPositionFromPicking(){
    LogInfo("Found Picking Object with ID : (" << widgetPickingObject_->getPickingId() << ")");
}

void PositionWidgetProcessor::process() {    
    updateAndActivateTarget(outport_, inport_);

    //widget_->getRepresentation<GeometryGL>()->render();

    deactivateCurrentTarget();
    
}

} // namespace
