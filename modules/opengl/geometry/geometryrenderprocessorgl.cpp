#include "geometryrenderprocessorgl.h"
#include <modules/opengl/geometry/geometrygl.h>

namespace inviwo {

ProcessorClassName(GeometryRenderProcessorGL, "GeometryRenderProcessorGL"); 
ProcessorCategory(GeometryRenderProcessorGL, "Geometry Rendering");
ProcessorCodeState(GeometryRenderProcessorGL, CODE_STATE_STABLE);

GeometryRenderProcessorGL::GeometryRenderProcessorGL()
    : ProcessorGL(),
      inport_("geometry.inport"),
      outport_("image.outport"),
      camera_("camera", "Camera", vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 1.0f, 0.0f))
{
    addPort(inport_);
    addPort(outport_);

    addProperty(camera_);
}

void GeometryRenderProcessorGL::process() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()));
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));

    activateAndClearTarget(outport_);

    inport_.getData()->getRepresentation<GeometryGL>()->render();

    deactivateCurrentTarget();

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

} // namespace
