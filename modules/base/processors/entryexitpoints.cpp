#include "entryexitpoints.h"
#include <modules/opengl/geometry/geometrygl.h>

namespace inviwo {

ProcessorClassName(EntryExitPoints, "EntryExitPoints"); 
ProcessorCategory(EntryExitPoints, "Geometry Rendering");
ProcessorCodeState(EntryExitPoints, CODE_STATE_STABLE);

EntryExitPoints::EntryExitPoints()
    : ProcessorGL(),
    geometryPort_("geometry"),
    entryPort_("entry-points"),
    exitPort_("exit-points"),
    camera_("camera", "Camera", vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 1.0f, 0.0f))
{
    addPort(geometryPort_);
    addPort(entryPort_);
    addPort(exitPort_);

    addProperty(camera_);
	addInteractionHandler(new Trackball(&camera_));
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
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()));

    // generate entry points
    activateTarget(entryPort_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
    geom->render();
    deactivateCurrentTarget();

    // generate exit points
    activateTarget(exitPort_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    geom->render();
    deactivateCurrentTarget();

    glPopMatrix();    
    
    glDisable(GL_CULL_FACE);
}

} // namespace
