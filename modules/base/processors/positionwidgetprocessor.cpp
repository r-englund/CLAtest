#include "positionwidgetprocessor.h"
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/interaction/pickingmanager.h>
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>

namespace inviwo {

ProcessorClassName(PositionWidgetProcessor, "PositionWidget"); 
ProcessorCategory(PositionWidgetProcessor, "Geometry Rendering");
ProcessorCodeState(PositionWidgetProcessor, CODE_STATE_EXPERIMENTAL); 

PositionWidgetProcessor::PositionWidgetProcessor()
    : CompositeProcessorGL(),
      inport_("inport"),
      outport_("outport", COLOR_DEPTH_PICKING),
      widgetType_("widgetType", "Widget Type"),
      position_("position", "Position", vec3(0.0f), vec3(-100.f), vec3(100.f)),
      camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
{
    addPort(inport_);
    addPort(outport_);

    widgetType_.addOption("cube", "Cube");
    widgetType_.addOption("sphere", "Sphere");
    widgetType_.set("sphere");
    widgetType_.onChange(this, &PositionWidgetProcessor::updateMeshWidget);
    addProperty(widgetType_);

    addProperty(position_);
    position_.setVisible(false);
    addProperty(camera_);
    addInteractionHandler(new Trackball(&camera_));

    widgetPickingObject_ = PickingManager::instance()->registerPickingCallback(this, &PositionWidgetProcessor::updateWidgetPositionFromPicking);

    widget_ = NULL;
}

PositionWidgetProcessor::~PositionWidgetProcessor() {}

void PositionWidgetProcessor::initialize() {
    CompositeProcessorGL::initialize();

    program_ = new Shader("picking.frag");
    program_->build();
}

void PositionWidgetProcessor::deinitialize() {
    CompositeProcessorGL::deinitialize();

    delete program_;
    program_ = NULL;

    delete widget_;
    widget_ = NULL;
}

void PositionWidgetProcessor::updateMeshWidget(){
    delete widget_;

    if(widgetType_.get() == "sphere"){
        widget_ = SimpleMeshCreator::sphere(0.5f, 8, 16);
    }
    else{
        vec3 posLLF = vec3(0.0f);
        vec3 posURB = vec3(1.0f);
        widget_ = SimpleMeshCreator::rectangularPrism(posLLF, posURB, posLLF, posURB, vec4(posLLF, 1.f), vec4(posURB, 1.f));
    }
}

void PositionWidgetProcessor::updateWidgetPositionFromPicking(){
    vec2 move = widgetPickingObject_->getPickingMove();
    if(move.x == 0.f && move.y == 0.f){
        return;
    }
    vec2 pos = widgetPickingObject_->getPickingPosition();
    float depth = widgetPickingObject_->getPickingDepth();
    vec3 startNdc = vec3((2.f*pos.x)-1.f, (2.f*pos.y)-1.f, depth);
    vec3 endNdc = vec3((2.f*(pos.x+move.x))-1.f, (2.f*(pos.y+move.y))-1.f, depth);
    vec3 startWorld = camera_.getWorldPosFromNormalizedDeviceCoords(startNdc);
    vec3 endWorld = camera_.getWorldPosFromNormalizedDeviceCoords(endNdc);
    position_.set(position_.get() + (endWorld-startWorld));
    invalidate(INVALID_OUTPUT);
}

void PositionWidgetProcessor::process() {  
    if(!widget_)
        updateMeshWidget();

    activateAndClearTarget(outport_);

    MeshRenderer renderer(widget_);

    program_->activate();
    program_->setUniform("pickingColor_", widgetPickingObject_->getPickingColor());

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()*glm::translate(position_.get())));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    renderer.render();

    glDepthFunc(GL_LESS);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glLoadIdentity();

    program_->deactivate();

    deactivateCurrentTarget();

    compositePortsToOutport(outport_, inport_);
}

} // namespace
