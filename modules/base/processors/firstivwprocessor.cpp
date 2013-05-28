#include "firstivwprocessor.h"

namespace inviwo {

ProcessorClassName(FirstIvwProcessor, "FirstIVWProcessor"); 
ProcessorCategory(FirstIvwProcessor, "Dummy Processors");
ProcessorCodeState(FirstIvwProcessor, CODE_STATE_STABLE);


FirstIvwProcessor::FirstIvwProcessor()
    : ProcessorGL(),
      outport_("outport"),
      color_("color", "Color", vec3(1.0f), vec3(0.0f), vec3(1.0f), vec3(0.1f))
{
    addProperty(color_);
    addPort(outport_);
}

void FirstIvwProcessor::process() {
    activateAndClearTarget(outport_);

    glLoadIdentity();
    glColor4f(color_.get().x, color_.get().y, color_.get().z, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f,  1.0f, 1.0f);
        glVertex3f( 1.0f,  1.0f, 1.0f);
        glVertex3f( 1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    glColor4f(0.0, 1.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f( 0.0f,  1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f( 1.0f, -1.0f, 0.0f);
    glEnd();
    
    deactivateCurrentTarget();
}

} // namespace
