#include "entryexitpoints.h"
#include <modules/opengl/geometry/geometrygl.h>
#include <inviwo/core/interaction/trackball.h>

namespace inviwo {

ProcessorClassName(EntryExitPoints, "EntryExitPoints"); 
ProcessorCategory(EntryExitPoints, "Geometry Rendering");
ProcessorCodeState(EntryExitPoints, CODE_STATE_STABLE);

EntryExitPoints::EntryExitPoints()
    : ProcessorGL(),
    geometryPort_("geometry"),
    entryPort_("entry-points"),
    exitPort_("exit-points"),
    camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
{
    addPort(geometryPort_);
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");

    addProperty(camera_);
	addInteractionHandler(new Trackball(&camera_));
    entryPort_.addResizeEventListener(&camera_);
}

EntryExitPoints::~EntryExitPoints() {}

void EntryExitPoints::initialize() {
    ProcessorGL::initialize();
}

void EntryExitPoints::deinitialize() {
    ProcessorGL::deinitialize();
}

void EntryExitPoints::process() {
    const GeometryGL* geom = geometryPort_.getData()->getRepresentation<GeometryGL>();

    glEnable(GL_CULL_FACE);    

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()));

    // generate entry points
    activateAndClearTarget(entryPort_);
    glCullFace(GL_BACK);
    geom->render();
    deactivateCurrentTarget();

    // generate exit points
    activateAndClearTarget(exitPort_);
    glCullFace(GL_FRONT);
    geom->render();
    deactivateCurrentTarget();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glLoadIdentity();   
    
    glDisable(GL_CULL_FACE);
}

} // namespace
